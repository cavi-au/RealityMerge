/**************************************************************************/
/* usdj_mediator.cpp                                                      */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             RealityMerge                               */
/*                          https://cavi.au.dk/                           */
/**************************************************************************/
/* Copyright (c) 2023-present RealityMerge contributors (see AUTHORS.md). */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include <filesystem>
#include <optional>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

// third-party
extern "C" {

#include <automerge-c/automerge.h>
}

// regional
#include <core/config/project_settings.h>
#include <core/error/error_macros.h>
#include <core/io/dir_access.h>
#include <core/io/json.h>
#include <core/io/resource_loader.h>
#include <core/templates/vector.h>
#include <core/variant/dictionary.h>

// local
#include "usdj_body_updater.h"
#include "usdj_mediator.h"
#include "usdj_static_body_3d.h"
#include "uuid.h"

namespace {

static int const BUFFER_SIZE = (1 << 23) - 1;

static int MAX_QUEUED_MESSAGES() {
    static std::optional<int> value{};

    if (!value)
        value.emplace(static_cast<int>(GLOBAL_GET("network/limits/debugger/max_queued_messages")));
    return *value;
}

static String PING_TEXT() {
    static std::optional<String> text{};

    if (!text) {
        Dictionary request{};
        request["ping"] = "ping";
        Ref<JSON> json;
        json.instantiate();
        text = json->stringify(request, "\t", false);
    }
    return *text;
}

static char const* const RESOURCE_TYPE_NAME = "AutomergeResource";

static String RESOURCE_EXTENSION() {
    static std::optional<String> extension{};

    if (!extension) {
        List<String> extensions{};
        ResourceLoader::get_recognized_extensions_for_type(RESOURCE_TYPE_NAME, &extensions);
        if (!extensions.is_empty())
            extension = *extensions.begin();
    }
    return *extension;
}

String find_document(String const& basename) {
    List<String> extensions{};
    ResourceLoader::get_recognized_extensions_for_type(RESOURCE_TYPE_NAME, &extensions);
    auto dir = DirAccess::open("res://", nullptr);
    if (!dir.is_null()) {
        dir->list_dir_begin();
        auto filename = dir->get_next();
        while (!filename.is_empty()) {
            if (!dir->current_is_dir() && filename.get_basename() == basename &&
                extensions.find(filename.get_extension())) {
                dir->list_dir_end();
                return dir->get_current_dir().path_join(filename);
            }
            filename = dir->get_next();
        }
        dir->list_dir_end();
    }
    return String{};
}

}  // namespace

UsdjMediator::UsdjMediator() : m_document_scan{false}, m_init_result{nullptr, nullptr}, m_server_sync{false} {}

UsdjMediator::~UsdjMediator() {}

void UsdjMediator::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_document_path"), &UsdjMediator::get_document_path);
    ClassDB::bind_method(D_METHOD("get_document_resource"), &UsdjMediator::get_document_resource);
    ClassDB::bind_method(D_METHOD("get_document_scan"), &UsdjMediator::get_document_scan);
    ClassDB::bind_method(D_METHOD("get_server_domain_name"), &UsdjMediator::get_server_domain_name);
    ClassDB::bind_method(D_METHOD("get_server_path"), &UsdjMediator::get_server_path);
    ClassDB::bind_method(D_METHOD("get_server_sync"), &UsdjMediator::get_server_sync);
    ClassDB::bind_method(D_METHOD("set_document_path"), &UsdjMediator::set_document_path);
    ClassDB::bind_method(D_METHOD("set_document_resource"), &UsdjMediator::set_document_resource);
    ClassDB::bind_method(D_METHOD("set_document_scan"), &UsdjMediator::set_document_scan);
    ClassDB::bind_method(D_METHOD("set_server_domain_name"), &UsdjMediator::set_server_domain_name);
    ClassDB::bind_method(D_METHOD("set_server_path"), &UsdjMediator::set_server_path);
    ClassDB::bind_method(D_METHOD("set_server_sync"), &UsdjMediator::set_server_sync);

    ADD_GROUP("Document", "document_");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "document_resource", PROPERTY_HINT_RESOURCE_TYPE, RESOURCE_TYPE_NAME),
                 "set_document_resource", "get_document_resource");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "document_path"), "set_document_path", "get_document_path");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "document_scan"), "set_document_scan", "get_document_scan");
    ADD_GROUP("Server", "server_");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "server_domain_name"), "set_server_domain_name",
                 "get_server_domain_name");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "server_path"), "set_server_path", "get_server_path");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "server_sync"), "set_server_sync", "get_server_sync");
}

void UsdjMediator::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_PROCESS: {
            if (receive_changes()) {
                update_bodies();
            } else {
                // Keep the connection, if there is one, alive.
                send_ping();
            }
            break;
        }
        case NOTIFICATION_READY: {
            update_bodies();
            set_process(true);
            break;
        }
    }
}

Error UsdjMediator::connect_to_server() {
    if (m_server_socket.is_null()) {
        m_server_socket = Ref<WebSocketPeer>(WebSocketPeer::create());
        ERR_FAIL_COND_V(m_server_socket.is_null(), ERR_CANT_CREATE);
        Vector<String> protocols;
        protocols.push_back("binary");
        m_server_socket->set_supported_protocols(protocols);
        m_server_socket->set_max_queued_packets(MAX_QUEUED_MESSAGES());
        m_server_socket->set_inbound_buffer_size(BUFFER_SIZE);
        m_server_socket->set_outbound_buffer_size(BUFFER_SIZE);
        m_server_socket->set_no_delay(true);
    }
    auto ready_state = m_server_socket->get_ready_state();
    if (ready_state == WebSocketPeer::STATE_OPEN) {
        return OK;
    }
    auto const url = String{"wss://"} + m_server_domain_name;
    ERR_FAIL_COND_V(m_server_socket->connect_to_url(url, TLSOptions::client()) != OK, ERR_CANT_CONNECT);
    m_server_socket->poll();
    ready_state = m_server_socket->get_ready_state();
    if (ready_state != WebSocketPeer::STATE_CONNECTING && ready_state != WebSocketPeer::STATE_OPEN) {
        m_server_socket.unref();
        ERR_FAIL_V_MSG(ERR_CANT_CONNECT,
                       vformat("Unable to connect to server \"%s\": state is %s.", m_server_domain_name, ready_state));
    }
    // Wait for the WebSocket connection to open.
    auto const time = OS::get_singleton()->get_ticks_msec();
    while (ready_state == WebSocketPeer::STATE_CONNECTING) {
        m_server_socket->poll();
        ready_state = m_server_socket->get_ready_state();
    }
    if (ready_state == WebSocketPeer::STATE_OPEN) {
        // Request updates of the Automerge document.
        Dictionary request{};
        request["wa"] = "open";
        Dictionary body{};
        body["strateId"] = m_server_path;
        if (m_server_peer_id.is_empty())
            m_server_peer_id = generate_uuidv4();
        body["peerId"] = m_server_peer_id;
        request["body"] = body;
        Ref<JSON> json;
        json.instantiate();
        ERR_FAIL_COND_V(m_server_socket->send_text(json->stringify(request, "\t", false)) != OK, ERR_QUERY_FAILED);
    }
    return OK;
}

PackedStringArray UsdjMediator::get_configuration_warnings() const {
    PackedStringArray warnings = Node::get_configuration_warnings();

    if (is_visible() && is_inside_tree()) {
        // Must reference an Automerge document resource.
        if (m_document_resource.is_null())
            warnings.push_back(RTR(get_class() + " must reference an " + RESOURCE_TYPE_NAME + "."));
        // Must specify a path within the Automerge document resource.
        if (m_document_path.is_empty())
            warnings.push_back(RTR(get_class() + " must specify a path within the " + RESOURCE_TYPE_NAME + "."));
        // Must specify the domain name of a server's URL.
        if (m_server_domain_name.is_empty())
            warnings.push_back(RTR(get_class() + " must specify the domain name of a server's URL."));
        // Must specify the path of a server's URL.
        if (m_server_path.is_empty())
            warnings.push_back(RTR(get_class() + " must specify the path of a server's URL."));
    }
    return warnings;
};

String UsdjMediator::get_document_path() const {
    return m_document_path;
}

Ref<AutomergeResource> UsdjMediator::get_document_resource() const {
    return m_document_resource;
}

bool UsdjMediator::get_document_scan() const {
    return m_document_scan;
}

String UsdjMediator::get_server_domain_name() const {
    return m_server_domain_name;
}

String UsdjMediator::get_server_path() const {
    return m_server_path;
}

bool UsdjMediator::get_server_sync() const {
    return m_server_sync;
}

Error UsdjMediator::send_ping() {
    if (!m_server_sync)
        return OK;
    connect_to_server();
    m_server_socket->poll();
    auto ready_state = m_server_socket->get_ready_state();
    // Wait for the WebSocket connection to open.
    while (ready_state == WebSocketPeer::STATE_CONNECTING) {
        m_server_socket->poll();
        ready_state = m_server_socket->get_ready_state();
    }
    if (ready_state == WebSocketPeer::STATE_OPEN) {
        // Send a ping message.
        ERR_FAIL_COND_V(m_server_socket->send_text(PING_TEXT()) != OK, FAILED);
    }
    return OK;
}

void UsdjMediator::set_document_path(String const& p_path) {
    if (p_path != m_document_path) {
        m_document_path = p_path;
        if (m_document_path.is_empty())
            update_bodies();
        /// \note The user must reactivate document scanning to indicate when
        ///       they're finished editing the document path.
        set_document_scan(false);
    }
}

void UsdjMediator::set_document_resource(Ref<AutomergeResource> const& p_resource) {
    if (p_resource != m_document_resource) {
        m_document_resource = p_resource;
        if (!m_document_resource.is_null()) {
            // Reset the Automerge document's associated synchronization state.
            m_init_result = ResultPtr{AMsyncStateInit(), AMresultFree};
        } else {
            // There is no document to synchronize at this point.
            m_init_result.reset();
            set_server_sync(false);
        }
        /// \note The user must reactivate document scanning to indicate when
        ///       they're finished editing the document resource.
        set_document_scan(false);
    }
}

void UsdjMediator::set_document_scan(bool const p_scan) {
    if (p_scan != m_document_scan) {
        m_document_scan = p_scan && !(m_document_resource.is_null() || m_document_path.is_empty());
        if (m_document_scan)
            update_bodies();
    }
}

void UsdjMediator::set_server_domain_name(String const& p_domain_name) {
    if (p_domain_name != m_server_domain_name) {
        m_server_domain_name = p_domain_name;
        /// \note The user must reactivate server synchronization to indicate
        ///       when they're finished editing the domain name of the URL.
        set_server_sync(false);
    }
}

void UsdjMediator::set_server_path(String const& p_path) {
    if (p_path != m_server_path) {
        m_server_path = p_path;
        /// \note The user must reactivate server synchronization to indicate
        ///       when they're finished editing the path of the URL.
        set_server_sync(false);
    }
}

void UsdjMediator::set_server_sync(bool const p_sync) {
    namespace fs = std::filesystem;
    using cavi::usdj_am::utils::Document;

    if (p_sync != m_server_sync) {
        m_server_sync = p_sync && !(m_server_domain_name.is_empty() || m_server_path.is_empty());
        if (!m_server_sync && !m_server_socket.is_null()) {
            m_server_socket->close();
            m_server_socket.unref();
        } else if (m_server_sync && m_document_resource.is_null()) {
            // Search for an Automerge document named after the server path.
            auto path = find_document(m_server_path);
            if (path.is_empty()) {
                // Create a new empty Automerge document named after the server path.
                path = String{"res://"}.path_join(m_server_path) + "." + RESOURCE_EXTENSION();
                try {
                    auto const document = Document{ResultPtr{AMcreate(nullptr), AMresultFree}};
                    auto const global_path = ProjectSettings::get_singleton()->globalize_path(path);
                    auto const buffer = global_path.to_utf8_buffer();
                    fs::path const filename{std::string{reinterpret_cast<std::string::const_pointer>(buffer.ptr()),
                                                        static_cast<std::string::size_type>(buffer.size())}};
                    document.save(filename);
                } catch (std::invalid_argument const& thrown) {
                    ERR_FAIL_MSG(thrown.what());
                }
            }
            // Load the Automerge document.
            set_document_resource(ResourceLoader::load(path));
        }
    }
}

bool UsdjMediator::receive_changes() {
    if (!m_server_sync)
        return false;
    connect_to_server();
    m_server_socket->poll();
    auto const ready_state = m_server_socket->get_ready_state();
    switch (ready_state) {
        case WebSocketPeer::STATE_OPEN: {
            std::vector<std::uint8_t> server_buffer{};
            auto packet_count = m_server_socket->get_available_packet_count();
            while (packet_count && m_server_socket->get_ready_state() == WebSocketPeer::STATE_OPEN) {
                std::uint8_t const* r_buffer = nullptr;
                int r_buffer_size = 0;
                auto const error = m_server_socket->get_packet(&r_buffer, r_buffer_size);
                ERR_FAIL_COND_V(error != OK, false);
                ERR_FAIL_COND_V(r_buffer_size <= 0, false);
                server_buffer.insert(server_buffer.end(), r_buffer, r_buffer + r_buffer_size);
                --packet_count;
            }
            if (!server_buffer.empty()) {
                auto document = m_document_resource->get_document();
                AMsyncState* client_state = nullptr;
                ERR_FAIL_COND_V(!AMitemToSyncState(AMresultItem(m_init_result.get()), &client_state), false);
                // Ignore a JSON message.
                Ref<JSON> json;
                json.instantiate();
                String const json_string{reinterpret_cast<char const*>(server_buffer.data()),
                                         static_cast<int>(server_buffer.size())};
                if (json->parse(json_string) != OK) {
                    // Ignore the prepended message type signifier byte.
                    ResultPtr const decode_result{
                        AMsyncMessageDecode(server_buffer.data() + 1, server_buffer.size() - 1), AMresultFree};
                    AMsyncMessage const* server_message = nullptr;
                    ERR_FAIL_COND_V(!AMitemToSyncMessage(AMresultItem(decode_result.get()), &server_message), false);
                    ResultPtr const receive_result{AMreceiveSyncMessage(document->get(), client_state, server_message),
                                                   AMresultFree};
                    ERR_FAIL_COND_V(AMresultStatus(receive_result.get()) != AM_STATUS_OK, ERR_BUG);
                    return true;
                }
                // Send a sync message regardless of what type of message the server sent.
                ResultPtr const generate_result{AMgenerateSyncMessage(document->get(), client_state), AMresultFree};
                AMsyncMessage const* client_message = nullptr;
                if (AMitemToSyncMessage(AMresultItem(generate_result.get()), &client_message)) {
                    ResultPtr const encode_result{AMsyncMessageEncode(client_message), AMresultFree};
                    AMbyteSpan client_message_bytes = {0};
                    ERR_FAIL_COND_V(!AMitemToBytes(AMresultItem(encode_result.get()), &client_message_bytes), false);
                    // Prepend a message type signifier byte.
                    std::vector<std::uint8_t> client_buffer{0};
                    client_buffer.reserve(client_buffer.size() + client_message_bytes.count);
                    client_buffer.insert(client_buffer.end(), client_message_bytes.src,
                                         client_message_bytes.src + client_message_bytes.count);
                    ERR_FAIL_COND_V(m_server_socket->put_packet(client_buffer.data(), client_buffer.size()) != OK,
                                    false);
                }
            }
            break;
        }
        case WebSocketPeer::STATE_CLOSING:
            break;
        case WebSocketPeer::STATE_CLOSED: {
            auto const code = m_server_socket->get_close_code();
            auto const reason = m_server_socket->get_close_reason();
            WARN_PRINT(vformat("WebSocket closed with code %d and reason \"%s\".", code, reason));
            m_server_socket.unref();
            break;
        }
    }
    return false;
}

void UsdjMediator::update_bodies() {
    auto parent = get_parent();
    if (!parent)
        return;
    auto physics_bodies = parent->find_children("*", "PhysicsBody3D", false, false);
    if (!m_document_scan) {
        // Remove all bodies constructed by a previous update.
        for (int pos = 0; pos != physics_bodies.size(); ++pos) {
            if (UsdjStaticBody3D* body = Object::cast_to<UsdjStaticBody3D>(physics_bodies[pos]))
                parent->call_deferred(SNAME("remove_child"), body);
        }
        return;
    }
    auto document = m_document_resource->get_document();
    if (document) {
        auto updater = UsdjBodyUpdater{physics_bodies};
        auto const buffer = m_document_path.to_utf8_buffer();
        auto const path = std::string{reinterpret_cast<std::string::const_pointer>(buffer.ptr()),
                                      static_cast<std::string::size_type>(buffer.size())};
        auto updates = updater(document->get(), path);
        for (auto const& item : updates) {
            switch (item.first) {
                case UsdjBodyUpdater::Action::ADD: {
                    // It's a physics body that wasn't described by the USDJ
                    // previously.
                    parent->call_deferred(SNAME("add_child"), item.second);
                    item.second->call_deferred(SNAME("set_owner"), parent);
                    break;
                }
                case UsdjBodyUpdater::Action::KEEP: {
                    // It's a physics body that's still described by the USDJ.
                    item.second->call_deferred(SNAME("revise"));
                    break;
                }
                case UsdjBodyUpdater::Action::REMOVE: {
                    // It's a physics body that's no longer described by the
                    // USDJ.
                    parent->call_deferred(SNAME("remove_child"), item.second);
                    break;
                }
            }
        }
    }
}

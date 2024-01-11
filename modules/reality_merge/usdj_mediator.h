/**************************************************************************/
/* usdj_mediator.h                                                        */
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

#ifndef REALITY_MERGE_USDJ_MEDIATOR_H
#define REALITY_MERGE_USDJ_MEDIATOR_H

#include <cstdint>
#include <memory>

// third-party
#include <cavi/usdj_am/utils/document.hpp>

// regional
#include <core/error/error_list.h>
#include <core/object/ref_counted.h>
#include <core/string/ustring.h>
#include <core/variant/variant.h>
#include <modules/websocket/websocket_peer.h>
#include <scene/3d/node_3d.h>

// local
#include "automerge_resource.h"

struct AMresult;

class UsdjMediator : public Node3D {
    GDCLASS(UsdjMediator, Node3D);

public:
    static std::uint64_t const HANDSHAKE_TIMEOUT_MSECS = 6000;

    UsdjMediator();

    ~UsdjMediator();

    PackedStringArray get_configuration_warnings() const override;

    /// \returns The POSIX path to a map object within the Automerge document.
    String get_document_path() const;

    /// \returns The Automerge document resource.
    Ref<AutomergeResource> get_document_resource() const;

    /// \returns The Automerge document scan toggle.
    bool get_document_scan() const;

    /// \returns The server's URL domain name component.
    String get_server_domain_name() const;

    /// \returns The server's URL path component.
    String get_server_path() const;

    /// \returns The server synchronization toggle.
    bool get_server_sync() const;

    /// \param[in] p_path A POSIX path to a map object within an Automerge
    ///                   document.
    void set_document_path(String const& p_path);

    /// \param[in] p_resource An Automerge document resource.
    void set_document_resource(Ref<AutomergeResource> const& p_resource);

    /// \param[in] p_scan A document scan toggle.
    void set_document_scan(bool const p_scan);

    /// \param[in] p_domain_name A server's URL domain name component.
    void set_server_domain_name(String const& p_domain_name);

    /// \param[in] p_path A server's URL path component.
    void set_server_path(String const& p_path);

    /// \param[in] p_sync A server synchronization toggle.
    void set_server_sync(bool const p_sync);

protected:
    static void _bind_methods();

    void _notification(int p_what);

    /// \brief Ensures that there's a connection to the server.
    ///
    /// \returns `Error::OK` if a connection exists.
    Error ensure_connection();

    bool receive_changes();

    Error send_ping();

    void update_bodies();

private:
    using ResultPtr = cavi::usdj_am::utils::Document::ResultPtr;

    String m_document_path;
    Ref<AutomergeResource> m_document_resource;
    bool m_document_scan;
    ResultPtr m_init_result;
    bool m_init_syncing;
    String m_server_domain_name;
    String m_server_path;
    String m_server_peer_id;
    Ref<WebSocketPeer> m_server_socket;
    bool m_server_sync;
};

#endif  // REALITY_MERGE_USDJ_MEDIATOR_H

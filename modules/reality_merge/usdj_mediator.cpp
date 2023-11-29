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

#include <memory>
#include <stdexcept>
#include <string>

// local
#include "typed_array_iterator.h"
#include "usdj_body_updater.h"
#include "usdj_mediator.h"

namespace {

static char const* const RESOURCE_TYPE_NAME = "AutomergeResource";

}  // namespace

UsdjMediator::~UsdjMediator(){};

void UsdjMediator::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_automerge_resource"), &UsdjMediator::get_automerge_resource);
    ClassDB::bind_method(D_METHOD("get_usdj_path"), &UsdjMediator::get_usdj_path);
    ClassDB::bind_method(D_METHOD("set_automerge_resource"), &UsdjMediator::set_automerge_resource);
    ClassDB::bind_method(D_METHOD("set_usdj_path"), &UsdjMediator::set_usdj_path);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "automerge_resource", PROPERTY_HINT_RESOURCE_TYPE, RESOURCE_TYPE_NAME),
                 "set_automerge_resource", "get_automerge_resource");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "usdj_path"), "set_usdj_path", "get_usdj_path");
}

void UsdjMediator::_update_bodies() {
    if (m_automerge_resource.is_null() || m_usdj_path.is_empty()) {
        return;
    }
    auto document = m_automerge_resource->get_document();
    auto parent = get_parent();
    if (document && parent) {
        auto physics_bodies = parent->find_children("*", "PhysicsBody3D", false, false);
        auto updater = UsdjBodyUpdater{physics_bodies};
        auto const buffer = m_usdj_path.to_utf8_buffer();
        auto const path = std::string{reinterpret_cast<std::string::const_pointer>(buffer.ptr()),
                                      static_cast<std::string::size_type>(buffer.size())};
        auto updates = updater(document->get(), path);
        for (auto const& item : updates) {
            switch (item.first) {
                case UsdjBodyUpdater::Action::ADD: {
                    // It's a physics body that wasn't described by the USDJ
                    // previously.
                    parent->add_child(item.second);
                    break;
                }
                case UsdjBodyUpdater::Action::IGNORE: {
                    // It's a physics body that was never described by the USDJ.
                    break;
                }
                case UsdjBodyUpdater::Action::KEEP: {
                    // It's a physics body that's still described by the USDJ.
                    break;
                }
                case UsdjBodyUpdater::Action::REMOVE: {
                    // It's a physics body that's no longer described by the
                    // USDJ.
                    parent->remove_child(item.second);
                    break;
                }
            }
        }
    }
}

PackedStringArray UsdjMediator::get_configuration_warnings() const {
    PackedStringArray warnings = Node::get_configuration_warnings();

    if (is_visible() && is_inside_tree()) {
        // Must reference an Automerge resource.
        if (m_automerge_resource.is_null())
            warnings.push_back(RTR(get_class() + " must reference an " + RESOURCE_TYPE_NAME + "."));
        // Must specify a path to a "USDA_File" node within the Automerge resource.
        if (m_usdj_path.is_empty())
            warnings.push_back(RTR(get_class() + " must specify a path within the " + RESOURCE_TYPE_NAME + "."));
    }
    return warnings;
};

Ref<AutomergeResource> UsdjMediator::get_automerge_resource() const {
    return m_automerge_resource;
}

void UsdjMediator::set_automerge_resource(Ref<AutomergeResource> const& p_automerge_resource) {
    m_automerge_resource = p_automerge_resource;
    _update_bodies();
}

String UsdjMediator::get_usdj_path() const {
    return m_usdj_path;
}

void UsdjMediator::set_usdj_path(String const& p_usdj_path) {
    m_usdj_path = p_usdj_path;
    _update_bodies();
}

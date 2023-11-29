/**************************************************************************/
/* usdj_box_shape_3d.cpp                                                  */
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

#include <sstream>
#include <stdexcept>
#include <typeinfo>

// third-party
#include <cavi/usdj_am/definition.hpp>

// regional
#include <servers/physics_server_3d.h>

// local
#include "usdj_box_shape_3d.h"
#include "usdj_box_size_extractor.h"

Vector<Vector3> UsdjBoxShape3D::get_debug_mesh_lines() const {
    Vector<Vector3> lines;
    AABB aabb;
    auto const size = get_size();
    aabb.position = -size / 2;
    aabb.size = size;

    for (int i = 0; i < 12; i++) {
        Vector3 a, b;
        aabb.get_edge(i, a, b);
        lines.push_back(a);
        lines.push_back(b);
    }

    return lines;
}

real_t UsdjBoxShape3D::get_enclosing_radius() const {
    return get_size().length() / 2;
}

void UsdjBoxShape3D::_update_shape() {
    PhysicsServer3D::get_singleton()->shape_set_data(get_shape(), get_size() / 2);
    Shape3D::_update_shape();
}

#ifndef DISABLE_DEPRECATED
bool UsdjBoxShape3D::_set(const StringName& p_name, const Variant& p_value) {
    if (p_name == "extents") {  // Compatibility with Godot 3.x.
        // Convert to `size`, twice as big.
        set_size((Vector3)p_value * 2);
        return true;
    }
    return false;
}

bool UsdjBoxShape3D::_get(const StringName& p_name, Variant& r_property) const {
    if (p_name == "extents") {  // Compatibility with Godot 3.x.
        // Convert to `extents`, half as big.
        r_property = get_size() / 2;
        return true;
    }
    return false;
}
#endif  // DISABLE_DEPRECATED

void UsdjBoxShape3D::set_size(Vector3 const& p_size) {
    // ERR_FAIL_COND_MSG(p_size.x < 0 || p_size.y < 0 || p_size.z < 0, "UsdjBoxShape3D size cannot be negative.");
    // size = p_size;
    // _update_shape();
    // notify_change_to_owners();
    std::ostringstream msg;
    msg << typeid(*this).name() << "::" << __func__ << "(p_size) is not implemented.";
    ERR_FAIL_MSG(msg.str().c_str());
}

Vector3 UsdjBoxShape3D::get_size() const {
    Vector3 size{1, 1, 1};
    auto const definition = m_definition.lock();
    if (definition)
        size = UsdjBoxSizeExtractor{definition}().value_or(size);
    return size;
}

void UsdjBoxShape3D::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_size", "size"), &UsdjBoxShape3D::set_size);
    ClassDB::bind_method(D_METHOD("get_size"), &UsdjBoxShape3D::get_size);

    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "size", PROPERTY_HINT_NONE, "suffix:m"), "set_size", "get_size");
}

UsdjBoxShape3D::UsdjBoxShape3D(std::shared_ptr<cavi::usdj_am::Definition> const& p_definition)
    : Shape3D{PhysicsServer3D::get_singleton()->shape_create(PhysicsServer3D::SHAPE_BOX)}, m_definition{p_definition} {
    std::ostringstream args;
    if (!p_definition) {
        args << "p_definition == nullptr";
    }
    if (!args.str().empty()) {
        std::ostringstream msg;
        msg << typeid(*this).name() << "::" << __func__ << "(" << args.str() << ")";
        ERR_FAIL_MSG(msg.str().c_str());
    }
}

/**************************************************************************/
/* usdj_static_body_3d.cpp                                                */
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
#include <cavi/usdj_am/definition_type.hpp>
#include <cavi/usdj_am/usd/physics/token_type.hpp>

// regional
#include <core/core_string_names.h>
#include <core/object/ref_counted.h>
#include <core/os/memory.h>
#include <scene/3d/collision_shape_3d.h>
#include <scene/3d/mesh_instance_3d.h>

// local
#include "usdj_geometry_extractor.h"
#include "usdj_static_body_3d.h"
#include "usdj_velocity_extractor.h"

void UsdjStaticBody3D::set_physics_material_override(const Ref<PhysicsMaterial>& p_physics_material_override) {
    if (physics_material_override.is_valid()) {
        if (physics_material_override->is_connected(
                CoreStringNames::get_singleton()->changed,
                callable_mp(this, &UsdjStaticBody3D::_reload_physics_characteristics))) {
            physics_material_override->disconnect(
                CoreStringNames::get_singleton()->changed,
                callable_mp(this, &UsdjStaticBody3D::_reload_physics_characteristics));
        }
    }

    physics_material_override = p_physics_material_override;

    if (physics_material_override.is_valid()) {
        physics_material_override->connect(CoreStringNames::get_singleton()->changed,
                                           callable_mp(this, &UsdjStaticBody3D::_reload_physics_characteristics));
    }
    _reload_physics_characteristics();
}

Ref<PhysicsMaterial> UsdjStaticBody3D::get_physics_material_override() const {
    return physics_material_override;
}

void UsdjStaticBody3D::set_constant_linear_velocity(const Vector3& p_vel) {
    // constant_linear_velocity = p_vel;

    // PhysicsServer3D::get_singleton()->body_set_state(get_rid(), PhysicsServer3D::BODY_STATE_LINEAR_VELOCITY,
    //                                                  constant_linear_velocity);
    std::ostringstream msg;
    msg << typeid(*this).name() << "::" << __func__ << "(...) not implemented";
    ERR_FAIL_MSG(msg.str().c_str());
}

void UsdjStaticBody3D::set_constant_angular_velocity(const Vector3& p_vel) {
    // constant_angular_velocity = p_vel;

    // PhysicsServer3D::get_singleton()->body_set_state(get_rid(), PhysicsServer3D::BODY_STATE_ANGULAR_VELOCITY,
    //                                                  constant_angular_velocity);
    std::ostringstream msg;
    msg << typeid(*this).name() << "::" << __func__ << "(...) not implemented";
    ERR_FAIL_MSG(msg.str().c_str());
}

Vector3 UsdjStaticBody3D::get_constant_linear_velocity() const {
    using cavi::usdj_am::usd::physics::TokenType;

    return UsdjVelocityExtractor{m_definition}(TokenType::PHYSICS_VELOCITY).value_or(Vector3{});
}

Vector3 UsdjStaticBody3D::get_constant_angular_velocity() const {
    using cavi::usdj_am::usd::physics::TokenType;

    return UsdjVelocityExtractor{m_definition}(TokenType::PHYSICS_ANGULAR_VELOCITY).value_or(Vector3{});
}

void UsdjStaticBody3D::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_constant_linear_velocity", "vel"),
                         &UsdjStaticBody3D::set_constant_linear_velocity);
    ClassDB::bind_method(D_METHOD("set_constant_angular_velocity", "vel"),
                         &UsdjStaticBody3D::set_constant_angular_velocity);
    ClassDB::bind_method(D_METHOD("get_constant_linear_velocity"), &UsdjStaticBody3D::get_constant_linear_velocity);
    ClassDB::bind_method(D_METHOD("get_constant_angular_velocity"), &UsdjStaticBody3D::get_constant_angular_velocity);

    ClassDB::bind_method(D_METHOD("set_physics_material_override", "physics_material_override"),
                         &UsdjStaticBody3D::set_physics_material_override);
    ClassDB::bind_method(D_METHOD("get_physics_material_override"), &UsdjStaticBody3D::get_physics_material_override);

    ADD_PROPERTY(
        PropertyInfo(Variant::OBJECT, "physics_material_override", PROPERTY_HINT_RESOURCE_TYPE, "PhysicsMaterial"),
        "set_physics_material_override", "get_physics_material_override");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "constant_linear_velocity", PROPERTY_HINT_NONE, "suffix:m/s"),
                 "set_constant_linear_velocity", "get_constant_linear_velocity");
    ADD_PROPERTY(
        PropertyInfo(Variant::VECTOR3, "constant_angular_velocity", PROPERTY_HINT_NONE, U"radians,suffix:\u00B0/s"),
        "set_constant_angular_velocity", "get_constant_angular_velocity");
}

UsdjStaticBody3D::UsdjStaticBody3D(std::unique_ptr<cavi::usdj_am::Definition>&& p_definition,
                                   PhysicsServer3D::BodyMode p_mode)
    : PhysicsBody3D(p_mode), m_definition(std::move(p_definition)) {
    using cavi::usdj_am::DefinitionType;

    std::ostringstream args;
    auto const sub_type = m_definition->get_sub_type();
    if (sub_type != DefinitionType::DEF) {
        args << "p_definition.get_sub_type() == " << sub_type << ", ...";
    } else {
        auto geometry = UsdjGeometryExtractor{m_definition}();
        if (!geometry.first) {
            args << "p_definition: no mesh found, ...";
        } else {
            auto mesh_instance_3d = memnew(MeshInstance3D);
            mesh_instance_3d->set_mesh(Ref<Mesh>{geometry.first.release()});
            add_child(mesh_instance_3d);
            if (geometry.second) {
                auto collision_shape_3d = memnew(CollisionShape3D);
                collision_shape_3d->set_shape(Ref<Shape3D>(geometry.second.release()));
                add_child(collision_shape_3d);
            }
        }
    }
    if (!args.str().empty()) {
        std::ostringstream what;
        what << typeid(*this).name() << "::" << __func__ << "(" << args.str() << ")";
        throw std::invalid_argument(what.str());
    }
}

void UsdjStaticBody3D::_reload_physics_characteristics() {
    if (physics_material_override.is_null()) {
        PhysicsServer3D::get_singleton()->body_set_param(get_rid(), PhysicsServer3D::BODY_PARAM_BOUNCE, 0);
        PhysicsServer3D::get_singleton()->body_set_param(get_rid(), PhysicsServer3D::BODY_PARAM_FRICTION, 1);
    } else {
        PhysicsServer3D::get_singleton()->body_set_param(get_rid(), PhysicsServer3D::BODY_PARAM_BOUNCE,
                                                         physics_material_override->computed_bounce());
        PhysicsServer3D::get_singleton()->body_set_param(get_rid(), PhysicsServer3D::BODY_PARAM_FRICTION,
                                                         physics_material_override->computed_friction());
    }
}

AMobjId const* UsdjStaticBody3D::get_object_id() const {
    return (m_definition) ? m_definition->get_object_id() : nullptr;
}

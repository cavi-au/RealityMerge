/**************************************************************************/
/* usdj_static_body_3d.h                                                  */
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

#ifndef REALITY_MERGE_USDJ_STATIC_BODY_3D_H
#define REALITY_MERGE_USDJ_STATIC_BODY_3D_H

#include <optional>

// third-party
#include <cavi/usdj_am/definition.hpp>

// regional
#include <scene/3d/physics_body_3d.h>
#include <scene/resources/physics_material.h>
#include <servers/physics_server_3d.h>

struct AMobjId;

class UsdjStaticBody3D : public PhysicsBody3D {
    GDCLASS(UsdjStaticBody3D, PhysicsBody3D);

private:
    // Vector3 constant_linear_velocity;
    // Vector3 constant_angular_velocity;

    Ref<PhysicsMaterial> physics_material_override;

protected:
    static void _bind_methods();

public:
    void set_physics_material_override(const Ref<PhysicsMaterial>& p_physics_material_override);
    Ref<PhysicsMaterial> get_physics_material_override() const;

    void set_constant_linear_velocity(const Vector3& p_vel);
    void set_constant_angular_velocity(const Vector3& p_vel);

    Vector3 get_constant_linear_velocity() const;
    Vector3 get_constant_angular_velocity() const;

    UsdjStaticBody3D(PhysicsServer3D::BodyMode p_mode = PhysicsServer3D::BODY_MODE_STATIC);

    /// \throws std::invalid_argument
    UsdjStaticBody3D(cavi::usdj_am::Definition&& p_definition,
                     PhysicsServer3D::BodyMode p_mode = PhysicsServer3D::BODY_MODE_STATIC);

    UsdjStaticBody3D(UsdjStaticBody3D const&) = delete;

    UsdjStaticBody3D(UsdjStaticBody3D&&) = default;

    UsdjStaticBody3D& operator=(UsdjStaticBody3D&&) = default;

    AMobjId const* get_object_id() const;

    /// \brief Update properties extracted from the "USDA_Definition" that had
    ///        to be cached.
    void revise();

private:
    std::optional<cavi::usdj_am::Definition> m_definition;

    void _reload_physics_characteristics();
};

#endif  // REALITY_MERGE_USDJ_STATIC_BODY_3D_H

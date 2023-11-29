/**************************************************************************/
/* usd/physics/token_type.cpp                                             */
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

#include <map>
#include <ostream>
#include <sstream>
#include <stdexcept>

// local
#include "detail/enum_string.hpp"
#include "usd/physics/token_type.hpp"
#include "value.hpp"

namespace {

using cavi::usdj_am::usd::physics::TokenType;

static std::map<std::string_view, TokenType> const TAGS = {
    {"acceleration", TokenType::ACCELERATION},
    {"angular", TokenType::ANGULAR},
    {"boundingCube", TokenType::BOUNDING_CUBE},
    {"boundingSphere", TokenType::BOUNDING_SPHERE},
    {"colliders", TokenType::COLLIDERS},
    {"convexDecomposition", TokenType::CONVEX_DECOMPOSITION},
    {"convexHull", TokenType::CONVEX_HULL},
    {"distance", TokenType::DISTANCE},
    {"drive", TokenType::DRIVE},
    {"drive:__INSTANCE_NAME__:physics:damping", TokenType::DRIVE_MULTIPLE_APPLY_TEMPLATE_PHYSICS_DAMPING},
    {"drive:__INSTANCE_NAME__:physics:maxForce", TokenType::DRIVE_MULTIPLE_APPLY_TEMPLATE_PHYSICS_MAX_FORCE},
    {"drive:__INSTANCE_NAME__:physics:stiffness", TokenType::DRIVE_MULTIPLE_APPLY_TEMPLATE_PHYSICS_STIFFNESS},
    {"drive:__INSTANCE_NAME__:physics:targetPosition",
     TokenType::DRIVE_MULTIPLE_APPLY_TEMPLATE_PHYSICS_TARGET_POSITION},
    {"drive:__INSTANCE_NAME__:physics:targetVelocity",
     TokenType::DRIVE_MULTIPLE_APPLY_TEMPLATE_PHYSICS_TARGET_VELOCITY},
    {"drive:__INSTANCE_NAME__:physics:type", TokenType::DRIVE_MULTIPLE_APPLY_TEMPLATE_PHYSICS_TYPE},
    {"force", TokenType::FORCE},
    {"kilogramsPerUnit", TokenType::KILOGRAMS_PER_UNIT},
    {"limit", TokenType::LIMIT},
    {"limit:__INSTANCE_NAME__:physics:high", TokenType::LIMIT_MULTIPLE_APPLY_TEMPLATE_PHYSICS_HIGH},
    {"limit:__INSTANCE_NAME__:physics:low", TokenType::LIMIT_MULTIPLE_APPLY_TEMPLATE_PHYSICS_LOW},
    {"linear", TokenType::LINEAR},
    {"meshSimplification", TokenType::MESH_SIMPLIFICATION},
    {"none", TokenType::NONE},
    {"physics:angularVelocity", TokenType::PHYSICS_ANGULAR_VELOCITY},
    {"physics:approximation", TokenType::PHYSICS_APPROXIMATION},
    {"physics:axis", TokenType::PHYSICS_AXIS},
    {"physics:body0", TokenType::PHYSICS_BODY_0},
    {"physics:body1", TokenType::PHYSICS_BODY_1},
    {"physics:breakForce", TokenType::PHYSICS_BREAK_FORCE},
    {"physics:breakTorque", TokenType::PHYSICS_BREAK_TORQUE},
    {"physics:centerOfMass", TokenType::PHYSICS_CENTER_OF_MASS},
    {"physics:collisionEnabled", TokenType::PHYSICS_COLLISION_ENABLED},
    {"physics:coneAngle0Limit", TokenType::PHYSICS_CONE_ANGLE_0_LIMIT},
    {"physics:coneAngle1Limit", TokenType::PHYSICS_CONE_ANGLE_1_LIMIT},
    {"physics:density", TokenType::PHYSICS_DENSITY},
    {"physics:diagonalInertia", TokenType::PHYSICS_DIAGONAL_INERTIA},
    {"physics:dynamicFriction", TokenType::PHYSICS_DYNAMIC_FRICTION},
    {"physics:excludeFromArticulation", TokenType::PHYSICS_EXCLUDE_FROM_ARTICULATION},
    {"physics:filteredGroups", TokenType::PHYSICS_FILTERED_GROUPS},
    {"physics:filteredPairs", TokenType::PHYSICS_FILTERED_PAIRS},
    {"physics:gravityDirection", TokenType::PHYSICS_GRAVITY_DIRECTION},
    {"physics:gravityMagnitude", TokenType::PHYSICS_GRAVITY_MAGNITUDE},
    {"physics:invertFilteredGroups", TokenType::PHYSICS_INVERT_FILTERED_GROUPS},
    {"physics:jointEnabled", TokenType::PHYSICS_JOINT_ENABLED},
    {"physics:kinematicEnabled", TokenType::PHYSICS_KINEMATIC_ENABLED},
    {"physics:localPos0", TokenType::PHYSICS_LOCAL_POS_0},
    {"physics:localPos1", TokenType::PHYSICS_LOCAL_POS_1},
    {"physics:localRot0", TokenType::PHYSICS_LOCAL_ROT_0},
    {"physics:localRot1", TokenType::PHYSICS_LOCAL_ROT_1},
    {"physics:lowerLimit", TokenType::PHYSICS_LOWER_LIMIT},
    {"physics:mass", TokenType::PHYSICS_MASS},
    {"physics:maxDistance", TokenType::PHYSICS_MAX_DISTANCE},
    {"physics:mergeGroup", TokenType::PHYSICS_MERGE_GROUP},
    {"physics:minDistance", TokenType::PHYSICS_MIN_DISTANCE},
    {"physics:principalAxes", TokenType::PHYSICS_PRINCIPAL_AXES},
    {"physics:restitution", TokenType::PHYSICS_RESTITUTION},
    {"physics:rigidBodyEnabled", TokenType::PHYSICS_RIGID_BODY_ENABLED},
    {"physics:simulationOwner", TokenType::PHYSICS_SIMULATION_OWNER},
    {"physics:startsAsleep", TokenType::PHYSICS_STARTS_ASLEEP},
    {"physics:staticFriction", TokenType::PHYSICS_STATIC_FRICTION},
    {"physics:upperLimit", TokenType::PHYSICS_UPPER_LIMIT},
    {"physics:velocity", TokenType::PHYSICS_VELOCITY},
    {"rotX", TokenType::ROT_X},
    {"rotY", TokenType::ROT_Y},
    {"rotZ", TokenType::ROT_Z},
    {"transX", TokenType::TRANS_X},
    {"transY", TokenType::TRANS_Y},
    {"transZ", TokenType::TRANS_Z},
    {"X", TokenType::X},
    {"Y", TokenType::Y},
    {"Z", TokenType::Z},
    {"PhysicsArticulationRootAPI", TokenType::PHYSICS_ARTICULATION_ROOT_API},
    {"PhysicsCollisionAPI", TokenType::PHYSICS_COLLISION_API},
    {"PhysicsCollisionGroup", TokenType::PHYSICS_COLLISION_GROUP},
    {"PhysicsDistanceJoint", TokenType::PHYSICS_DISTANCE_JOINT},
    {"PhysicsDriveAPI", TokenType::PHYSICS_DRIVE_API},
    {"PhysicsFilteredPairsAPI", TokenType::PHYSICS_FILTERED_PAIRS_API},
    {"PhysicsFixedJoint", TokenType::PHYSICS_FIXED_JOINT},
    {"PhysicsJoint", TokenType::PHYSICS_JOINT},
    {"PhysicsLimitAPI", TokenType::PHYSICS_LIMIT_API},
    {"PhysicsMassAPI", TokenType::PHYSICS_MASS_API},
    {"PhysicsMaterialAPI", TokenType::PHYSICS_MATERIAL_API},
    {"PhysicsMeshCollisionAPI", TokenType::PHYSICS_MESH_COLLISION_API},
    {"PhysicsPrismaticJoint", TokenType::PHYSICS_PRISMATIC_JOINT},
    {"PhysicsRevoluteJoint", TokenType::PHYSICS_REVOLUTE_JOINT},
    {"PhysicsRigidBodyAPI", TokenType::PHYSICS_RIGIDBODY_API},
    {"PhysicsScene", TokenType::PHYSICS_SCENE},
    {"PhysicsSphericalJoint", TokenType::PHYSICS_SPHERICAL_JOINT}};

}  // namespace

namespace cavi {
namespace usdj_am {
namespace usd {
namespace physics {

/// \todo Also parse tokens containing substitutions for "__INSTANCE_NAME__".
std::optional<TokenType> extract_TokenType(std::string_view const& view) {
    return detail::extract_enum_tag<TokenType>(TAGS, view);
}

/// \todo Also parse tokens containing substitutions for "__INSTANCE_NAME__".
TokenType to_TokenType(std::string_view const& view) {
    auto const result = detail::extract_enum_tag<TokenType>(TAGS, view);
    if (!result) {
        std::ostringstream what;
        what << __func__ << "(view == \"" << view << "\")";
        throw std::invalid_argument(what.str());
    }
    return *result;
}

TokenTypeSet extract_TokenTypeSet(Value const& value) {
    return detail::extract_enum_tag_set<TokenType>(TAGS, value);
}

/// \todo Also parse tokens containing substitutions for "__INSTANCE_NAME__".
std::istream& operator>>(std::istream& is, TokenType& out) {
    return detail::operator>><TokenType>(is, std::make_pair(std::cref(TAGS), std::ref(out)));
}

std::ostream& operator<<(std::ostream& os, TokenType const& in) {
    os << detail::extract_enum_string<TokenType>(TAGS, in).value_or("???");
    return os;
}

}  // namespace physics
}  // namespace usd
}  // namespace usdj_am
}  // namespace cavi

/**************************************************************************/
/* usd/physics/token_type.hpp                                             */
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

#ifndef CAVI_USDJ_AM_USD_PHYSICS_TOKEN_TYPE_HPP
#define CAVI_USDJ_AM_USD_PHYSICS_TOKEN_TYPE_HPP

#include <cstdint>
#include <iosfwd>
#include <optional>
#include <set>
#include <string_view>

// boost::python::class_<UsdPhysicsTokensType, boost::noncopyable>
//     cls("Tokens", boost::python::no_init);
// _AddToken(cls, "acceleration", UsdPhysicsTokens->acceleration);
// _AddToken(cls, "angular", UsdPhysicsTokens->angular);
// _AddToken(cls, "boundingCube", UsdPhysicsTokens->boundingCube);
// _AddToken(cls, "boundingSphere", UsdPhysicsTokens->boundingSphere);
// _AddToken(cls, "colliders", UsdPhysicsTokens->colliders);
// _AddToken(cls, "convexDecomposition", UsdPhysicsTokens->convexDecomposition);
// _AddToken(cls, "convexHull", UsdPhysicsTokens->convexHull);
// _AddToken(cls, "distance", UsdPhysicsTokens->distance);
// _AddToken(cls, "drive", UsdPhysicsTokens->drive);
// _AddToken(cls, "drive_MultipleApplyTemplate_PhysicsDamping",
// UsdPhysicsTokens->drive_MultipleApplyTemplate_PhysicsDamping); _AddToken(cls,
// "drive_MultipleApplyTemplate_PhysicsMaxForce", UsdPhysicsTokens->drive_MultipleApplyTemplate_PhysicsMaxForce);
// _AddToken(cls, "drive_MultipleApplyTemplate_PhysicsStiffness",
// UsdPhysicsTokens->drive_MultipleApplyTemplate_PhysicsStiffness); _AddToken(cls,
// "drive_MultipleApplyTemplate_PhysicsTargetPosition",
// UsdPhysicsTokens->drive_MultipleApplyTemplate_PhysicsTargetPosition); _AddToken(cls,
// "drive_MultipleApplyTemplate_PhysicsTargetVelocity",
// UsdPhysicsTokens->drive_MultipleApplyTemplate_PhysicsTargetVelocity); _AddToken(cls,
// "drive_MultipleApplyTemplate_PhysicsType", UsdPhysicsTokens->drive_MultipleApplyTemplate_PhysicsType); _AddToken(cls,
// "force", UsdPhysicsTokens->force); _AddToken(cls, "kilogramsPerUnit", UsdPhysicsTokens->kilogramsPerUnit);
// _AddToken(cls, "limit", UsdPhysicsTokens->limit);
// _AddToken(cls, "limit_MultipleApplyTemplate_PhysicsHigh", UsdPhysicsTokens->limit_MultipleApplyTemplate_PhysicsHigh);
// _AddToken(cls, "limit_MultipleApplyTemplate_PhysicsLow", UsdPhysicsTokens->limit_MultipleApplyTemplate_PhysicsLow);
// _AddToken(cls, "linear", UsdPhysicsTokens->linear);
// _AddToken(cls, "meshSimplification", UsdPhysicsTokens->meshSimplification);
// _AddToken(cls, "none", UsdPhysicsTokens->none);
// _AddToken(cls, "physicsAngularVelocity", UsdPhysicsTokens->physicsAngularVelocity);
// _AddToken(cls, "physicsApproximation", UsdPhysicsTokens->physicsApproximation);
// _AddToken(cls, "physicsAxis", UsdPhysicsTokens->physicsAxis);
// _AddToken(cls, "physicsBody0", UsdPhysicsTokens->physicsBody0);
// _AddToken(cls, "physicsBody1", UsdPhysicsTokens->physicsBody1);
// _AddToken(cls, "physicsBreakForce", UsdPhysicsTokens->physicsBreakForce);
// _AddToken(cls, "physicsBreakTorque", UsdPhysicsTokens->physicsBreakTorque);
// _AddToken(cls, "physicsCenterOfMass", UsdPhysicsTokens->physicsCenterOfMass);
// _AddToken(cls, "physicsCollisionEnabled", UsdPhysicsTokens->physicsCollisionEnabled);
// _AddToken(cls, "physicsConeAngle0Limit", UsdPhysicsTokens->physicsConeAngle0Limit);
// _AddToken(cls, "physicsConeAngle1Limit", UsdPhysicsTokens->physicsConeAngle1Limit);
// _AddToken(cls, "physicsDensity", UsdPhysicsTokens->physicsDensity);
// _AddToken(cls, "physicsDiagonalInertia", UsdPhysicsTokens->physicsDiagonalInertia);
// _AddToken(cls, "physicsDynamicFriction", UsdPhysicsTokens->physicsDynamicFriction);
// _AddToken(cls, "physicsExcludeFromArticulation", UsdPhysicsTokens->physicsExcludeFromArticulation);
// _AddToken(cls, "physicsFilteredGroups", UsdPhysicsTokens->physicsFilteredGroups);
// _AddToken(cls, "physicsFilteredPairs", UsdPhysicsTokens->physicsFilteredPairs);
// _AddToken(cls, "physicsGravityDirection", UsdPhysicsTokens->physicsGravityDirection);
// _AddToken(cls, "physicsGravityMagnitude", UsdPhysicsTokens->physicsGravityMagnitude);
// _AddToken(cls, "physicsInvertFilteredGroups", UsdPhysicsTokens->physicsInvertFilteredGroups);
// _AddToken(cls, "physicsJointEnabled", UsdPhysicsTokens->physicsJointEnabled);
// _AddToken(cls, "physicsKinematicEnabled", UsdPhysicsTokens->physicsKinematicEnabled);
// _AddToken(cls, "physicsLocalPos0", UsdPhysicsTokens->physicsLocalPos0);
// _AddToken(cls, "physicsLocalPos1", UsdPhysicsTokens->physicsLocalPos1);
// _AddToken(cls, "physicsLocalRot0", UsdPhysicsTokens->physicsLocalRot0);
// _AddToken(cls, "physicsLocalRot1", UsdPhysicsTokens->physicsLocalRot1);
// _AddToken(cls, "physicsLowerLimit", UsdPhysicsTokens->physicsLowerLimit);
// _AddToken(cls, "physicsMass", UsdPhysicsTokens->physicsMass);
// _AddToken(cls, "physicsMaxDistance", UsdPhysicsTokens->physicsMaxDistance);
// _AddToken(cls, "physicsMergeGroup", UsdPhysicsTokens->physicsMergeGroup);
// _AddToken(cls, "physicsMinDistance", UsdPhysicsTokens->physicsMinDistance);
// _AddToken(cls, "physicsPrincipalAxes", UsdPhysicsTokens->physicsPrincipalAxes);
// _AddToken(cls, "physicsRestitution", UsdPhysicsTokens->physicsRestitution);
// _AddToken(cls, "physicsRigidBodyEnabled", UsdPhysicsTokens->physicsRigidBodyEnabled);
// _AddToken(cls, "physicsSimulationOwner", UsdPhysicsTokens->physicsSimulationOwner);
// _AddToken(cls, "physicsStartsAsleep", UsdPhysicsTokens->physicsStartsAsleep);
// _AddToken(cls, "physicsStaticFriction", UsdPhysicsTokens->physicsStaticFriction);
// _AddToken(cls, "physicsUpperLimit", UsdPhysicsTokens->physicsUpperLimit);
// _AddToken(cls, "physicsVelocity", UsdPhysicsTokens->physicsVelocity);
// _AddToken(cls, "rotX", UsdPhysicsTokens->rotX);
// _AddToken(cls, "rotY", UsdPhysicsTokens->rotY);
// _AddToken(cls, "rotZ", UsdPhysicsTokens->rotZ);
// _AddToken(cls, "transX", UsdPhysicsTokens->transX);
// _AddToken(cls, "transY", UsdPhysicsTokens->transY);
// _AddToken(cls, "transZ", UsdPhysicsTokens->transZ);
// _AddToken(cls, "x", UsdPhysicsTokens->x);
// _AddToken(cls, "y", UsdPhysicsTokens->y);
// _AddToken(cls, "z", UsdPhysicsTokens->z);
// _AddToken(cls, "PhysicsArticulationRootAPI", UsdPhysicsTokens->PhysicsArticulationRootAPI);
// _AddToken(cls, "PhysicsCollisionAPI", UsdPhysicsTokens->PhysicsCollisionAPI);
// _AddToken(cls, "PhysicsCollisionGroup", UsdPhysicsTokens->PhysicsCollisionGroup);
// _AddToken(cls, "PhysicsDistanceJoint", UsdPhysicsTokens->PhysicsDistanceJoint);
// _AddToken(cls, "PhysicsDriveAPI", UsdPhysicsTokens->PhysicsDriveAPI);
// _AddToken(cls, "PhysicsFilteredPairsAPI", UsdPhysicsTokens->PhysicsFilteredPairsAPI);
// _AddToken(cls, "PhysicsFixedJoint", UsdPhysicsTokens->PhysicsFixedJoint);
// _AddToken(cls, "PhysicsJoint", UsdPhysicsTokens->PhysicsJoint);
// _AddToken(cls, "PhysicsLimitAPI", UsdPhysicsTokens->PhysicsLimitAPI);
// _AddToken(cls, "PhysicsMassAPI", UsdPhysicsTokens->PhysicsMassAPI);
// _AddToken(cls, "PhysicsMaterialAPI", UsdPhysicsTokens->PhysicsMaterialAPI);
// _AddToken(cls, "PhysicsMeshCollisionAPI", UsdPhysicsTokens->PhysicsMeshCollisionAPI);
// _AddToken(cls, "PhysicsPrismaticJoint", UsdPhysicsTokens->PhysicsPrismaticJoint);
// _AddToken(cls, "PhysicsRevoluteJoint", UsdPhysicsTokens->PhysicsRevoluteJoint);
// _AddToken(cls, "PhysicsRigidBodyAPI", UsdPhysicsTokens->PhysicsRigidBodyAPI);
// _AddToken(cls, "PhysicsScene", UsdPhysicsTokens->PhysicsScene);
// _AddToken(cls, "PhysicsSphericalJoint", UsdPhysicsTokens->PhysicsSphericalJoint);

namespace cavi {
namespace usdj_am {

struct Value;

namespace usd {
namespace physics {

/// \brief An enum representing a `UsdPhysicsTokensType` tag stored as a
///        string within a "USDA_Declaration" node in a syntax tree that was
///        parsed out of a USDA document, encoded as JSON and stored within an
///        Automerge document.
enum class TokenType : std::uint8_t {
    BEGIN__ = 1,
    ACCELERATION = BEGIN__,
    ANGULAR,
    BOUNDING_CUBE,
    BOUNDING_SPHERE,
    COLLIDERS,
    CONVEX_DECOMPOSITION,
    CONVEX_HULL,
    DISTANCE,
    DRIVE,
    DRIVE_MULTIPLE_APPLY_TEMPLATE_PHYSICS_DAMPING,
    DRIVE_MULTIPLE_APPLY_TEMPLATE_PHYSICS_MAX_FORCE,
    DRIVE_MULTIPLE_APPLY_TEMPLATE_PHYSICS_STIFFNESS,
    DRIVE_MULTIPLE_APPLY_TEMPLATE_PHYSICS_TARGET_POSITION,
    DRIVE_MULTIPLE_APPLY_TEMPLATE_PHYSICS_TARGET_VELOCITY,
    DRIVE_MULTIPLE_APPLY_TEMPLATE_PHYSICS_TYPE,
    FORCE,
    KILOGRAMS_PER_UNIT,
    LIMIT,
    LIMIT_MULTIPLE_APPLY_TEMPLATE_PHYSICS_HIGH,
    LIMIT_MULTIPLE_APPLY_TEMPLATE_PHYSICS_LOW,
    LINEAR,
    MESH_SIMPLIFICATION,
    NONE,
    PHYSICS_ANGULAR_VELOCITY,
    PHYSICS_APPROXIMATION,
    PHYSICS_AXIS,
    PHYSICS_BODY_0,
    PHYSICS_BODY_1,
    PHYSICS_BREAK_FORCE,
    PHYSICS_BREAK_TORQUE,
    PHYSICS_CENTER_OF_MASS,
    PHYSICS_COLLISION_ENABLED,
    PHYSICS_CONE_ANGLE_0_LIMIT,
    PHYSICS_CONE_ANGLE_1_LIMIT,
    PHYSICS_DENSITY,
    PHYSICS_DIAGONAL_INERTIA,
    PHYSICS_DYNAMIC_FRICTION,
    PHYSICS_EXCLUDE_FROM_ARTICULATION,
    PHYSICS_FILTERED_GROUPS,
    PHYSICS_FILTERED_PAIRS,
    PHYSICS_GRAVITY_DIRECTION,
    PHYSICS_GRAVITY_MAGNITUDE,
    PHYSICS_INVERT_FILTERED_GROUPS,
    PHYSICS_JOINT_ENABLED,
    PHYSICS_KINEMATIC_ENABLED,
    PHYSICS_LOCAL_POS_0,
    PHYSICS_LOCAL_POS_1,
    PHYSICS_LOCAL_ROT_0,
    PHYSICS_LOCAL_ROT_1,
    PHYSICS_LOWER_LIMIT,
    PHYSICS_MASS,
    PHYSICS_MAX_DISTANCE,
    PHYSICS_MERGE_GROUP,
    PHYSICS_MIN_DISTANCE,
    PHYSICS_PRINCIPAL_AXES,
    PHYSICS_RESTITUTION,
    PHYSICS_RIGID_BODY_ENABLED,
    PHYSICS_SIMULATION_OWNER,
    PHYSICS_STARTS_ASLEEP,
    PHYSICS_STATIC_FRICTION,
    PHYSICS_UPPER_LIMIT,
    PHYSICS_VELOCITY,
    ROT_X,
    ROT_Y,
    ROT_Z,
    TRANS_X,
    TRANS_Y,
    TRANS_Z,
    X,
    Y,
    Z,
    PHYSICS_ARTICULATION_ROOT_API,
    PHYSICS_COLLISION_API,
    PHYSICS_COLLISION_GROUP,
    PHYSICS_DISTANCE_JOINT,
    PHYSICS_DRIVE_API,
    PHYSICS_FILTERED_PAIRS_API,
    PHYSICS_FIXED_JOINT,
    PHYSICS_JOINT,
    PHYSICS_LIMIT_API,
    PHYSICS_MASS_API,
    PHYSICS_MATERIAL_API,
    PHYSICS_MESH_COLLISION_API,
    PHYSICS_PRISMATIC_JOINT,
    PHYSICS_REVOLUTE_JOINT,
    PHYSICS_RIGIDBODY_API,
    PHYSICS_SCENE,
    PHYSICS_SPHERICAL_JOINT,
    END__,
    SIZE__ = END__ - BEGIN__
};

/// \brief Extracts a `TokenType` enum tag from a string view.
///
/// \param[in] view A UTF-8 string view.
/// \returns The `TokenType` enum tag whose serialized form is \p view or
///          `std::nullopt`.
std::optional<TokenType> extract_TokenType(std::string_view const& view);

/// \brief Converts a string view into a `TokenType` enum tag.
///
/// \param[in] view A UTF-8 string view.
/// \returns The `TokenType` enum tag whose serialized form is \p view.
/// \throws std::invalid_argument
TokenType to_TokenType(std::string_view const& view);

using TokenTypeSet = std::set<TokenType>;

/// \brief Extracts a `TokenTypeSet` from a `Value`.
///
/// \param[in] value A `Value` in which to search for a `TokenTypeSet`.
/// \returns A `TokenTypeSet` that may be empty.
TokenTypeSet extract_TokenTypeSet(Value const& value);

std::istream& operator>>(std::istream& is, TokenType& out);

std::ostream& operator<<(std::ostream& os, TokenType const& in);

}  // namespace physics
}  // namespace usd
}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_USD_PHYSICS_TOKEN_TYPE_HPP

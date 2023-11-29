/**************************************************************************/
/* usd/geom/xform_op_type.hpp                                             */
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

#ifndef CAVI_USDJ_AM_USD_GEOM_XFORM_OP_TYPE_HPP
#define CAVI_USDJ_AM_USD_GEOM_XFORM_OP_TYPE_HPP

#include <cstdint>
#include <iosfwd>
#include <optional>
#include <string_view>
#include <vector>

// #define USDGEOM_XFORM_OP_TYPES \
//     (translate) \
//     (scale) \
//     (rotateX) \
//     (rotateY) \
//     (rotateZ) \
//     (rotateXYZ) \
//     (rotateXZY) \
//     (rotateYXZ) \
//     (rotateYZX) \
//     (rotateZXY) \
//     (rotateZYX) \
//     (orient) \
//     (transform) \
//     ((resetXformStack, "!resetXformStack!"))

namespace cavi {
namespace usdj_am {
namespace usd {
namespace geom {

/// \brief An enum representing a `UsdGeomXformOpTypes::Type` tag stored as a
///        string within a "USDA_Declaration" node in a syntax tree that was
///        parsed out of a USDA document, encoded as JSON and stored within an
///        Automerge document.
enum class XformOpType : std::uint8_t {
    BEGIN__ = 1,
    /// XYZ translation.
    TRANSLATE = BEGIN__,
    /// XYZ scale.
    SCALE,
    /// Rotation about the X axis in degrees.
    ROTATE_X,
    /// Rotation about the Y axis in degrees.
    ROTATE_Y,
    /// Rotation about the Z axis in degrees.
    ROTATE_Z,
    /// A set of three canonical Euler rotations packed into a single 3D vector
    /// equivalent to `ROTATE_Z`, `ROTATE_Y` and `ROTATE_X`.
    ROTATE_XYZ,
    /// A set of three canonical Euler rotations packed into a single 3D vector
    /// equivalent to `ROTATE_Y`, `ROTATE_Z` and `ROTATE_X`.
    ROTATE_XZY,
    /// A set of three canonical Euler rotations packed into a single 3D vector
    /// equivalent to `ROTATE_Z`, `ROTATE_X` and `ROTATE_Y`.
    ROTATE_YXZ,
    /// A set of three canonical Euler rotations packed into a single 3D vector
    /// equivalent to `ROTATE_X`, `ROTATE_Z` and `ROTATE_Y`.
    ROTATE_YZX,
    /// A set of three canonical Euler rotations packed into a single 3D vector
    /// equivalent to `ROTATE_Y`, `ROTATE_X` and `ROTATE_Z`.
    ROTATE_ZXY,
    /// A set of three canonical Euler rotations packed into a single 3D vector
    /// equivalent to `ROTATE_X`, `ROTATE_Y` and `ROTATE_Z`.
    ROTATE_ZYX,
    /// Arbitrary axis/angle rotation expressed as a quaternion.
    ORIENT,
    /// 4X4 matrix transformation.
    TRANSFORM,
    /// Instructs the client that the transform stack should be cleared of any
    /// inherited transformation prior to processing subsequent operations.
    /// \warning This tag can only appear as the first in an `XformOpTypeOrder`.
    RESET_XFORM_STACK,
    END__,
    SIZE__ = END__ - BEGIN__
};

/// \brief Extracts a `XformOpType` enum tag from a string view.
///
/// \param[in] view A UTF-8 string view.
/// \returns The `XformOpType` enum tag whose serialized form is \p view or
///          `std::nullopt`.
std::optional<XformOpType> extract_XformOpType(std::string_view const& view);

/// \brief Converts a string view into a `XformOpType` enum tag.
///
/// \param[in] view A UTF-8 string view.
/// \returns The `XformOpType` enum tag whose serialized form is \p view.
/// \throws std::invalid_argument
XformOpType to_XformOpType(std::string_view const& view);

std::istream& operator>>(std::istream& is, XformOpType& out);

std::ostream& operator<<(std::ostream& os, XformOpType const& in);

/// \brief An ordered sequence of `XformOpType` tags.
using XformOpTypeOrder = std::vector<XformOpType>;

/// \brief Extracts an `XformOpTypeOrder` from a `Value`.
///
/// \param[in] value A `Value` in which to search for an `XformOpTypeOrder`.
/// \returns An `XformOpTypeOrder` that may be empty.
XformOpTypeOrder extract_XformOpTypeOrder(Value const& value);

}  // namespace geom
}  // namespace usd
}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_USD_GEOM_XFORM_OP_TYPE_HPP

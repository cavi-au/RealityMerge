/**************************************************************************/
/* usd/sdf/value_type_name.hpp                                            */
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

#ifndef CAVI_USDJ_AM_USD_SDF_VALUE_TYPE_NAME_HPP
#define CAVI_USDJ_AM_USD_SDF_VALUE_TYPE_NAME_HPP

#include <cstdint>
#include <iosfwd>
#include <string_view>

namespace cavi {
namespace usdj_am {
namespace usd {
namespace sdf {

// class Sdf_ValueTypeNamesType {
// public:
//     SdfValueTypeName Bool;
//     SdfValueTypeName UChar, Int, UInt, Int64, UInt64;
//     SdfValueTypeName Half, Float, Double, TimeCode;
//     SdfValueTypeName String, Token, Asset;
//     SdfValueTypeName Int2,     Int3,     Int4;
//     SdfValueTypeName Half2,    Half3,    Half4;
//     SdfValueTypeName Float2,   Float3,   Float4;
//     SdfValueTypeName Double2,  Double3,  Double4;
//     SdfValueTypeName Point3h,  Point3f,  Point3d;
//     SdfValueTypeName Vector3h, Vector3f, Vector3d;
//     SdfValueTypeName Normal3h, Normal3f, Normal3d;
//     SdfValueTypeName Color3h,  Color3f,  Color3d;
//     SdfValueTypeName Color4h,  Color4f,  Color4d;
//     SdfValueTypeName Quath,    Quatf,    Quatd;
//     SdfValueTypeName Matrix2d, Matrix3d, Matrix4d;
//     SdfValueTypeName Frame4d;
//     SdfValueTypeName TexCoord2h, TexCoord2f, TexCoord2d;
//     SdfValueTypeName TexCoord3h, TexCoord3f, TexCoord3d;
//     SdfValueTypeName Opaque;
//     SdfValueTypeName Group;

//     SdfValueTypeName BoolArray;
//     SdfValueTypeName UCharArray, IntArray, UIntArray, Int64Array, UInt64Array;
//     SdfValueTypeName HalfArray, FloatArray, DoubleArray, TimeCodeArray;
//     SdfValueTypeName StringArray, TokenArray, AssetArray;
//     SdfValueTypeName Int2Array,     Int3Array,     Int4Array;
//     SdfValueTypeName Half2Array,    Half3Array,    Half4Array;
//     SdfValueTypeName Float2Array,   Float3Array,   Float4Array;
//     SdfValueTypeName Double2Array,  Double3Array,  Double4Array;
//     SdfValueTypeName Point3hArray,  Point3fArray,  Point3dArray;
//     SdfValueTypeName Vector3hArray, Vector3fArray, Vector3dArray;
//     SdfValueTypeName Normal3hArray, Normal3fArray, Normal3dArray;
//     SdfValueTypeName Color3hArray,  Color3fArray,  Color3dArray;
//     SdfValueTypeName Color4hArray,  Color4fArray,  Color4dArray;
//     SdfValueTypeName QuathArray,    QuatfArray,    QuatdArray;
//     SdfValueTypeName Matrix2dArray, Matrix3dArray, Matrix4dArray;
//     SdfValueTypeName Frame4dArray;
//     SdfValueTypeName TexCoord2hArray, TexCoord2fArray, TexCoord2dArray;
//     SdfValueTypeName TexCoord3hArray, TexCoord3fArray, TexCoord3dArray;

/// \brief An enum representing an `SdfValueTypeName` constant stored as a
///        string within a "USDA_Declaration" node in a syntax tree that was
///        parsed out of a USDA document, encoded as JSON and stored within an
///        Automerge document.
enum class ValueTypeName : std::uint8_t {
    BEGIN__ = 1,
    ///
    BOOL = BEGIN__,
    ///
    UCHAR,
    ///
    INT,
    ///
    UINT,
    ///
    INT64,
    ///
    UINT64,
    ///
    HALF,
    ///
    FLOAT,
    ///
    DOUBLE,
    ///
    TIME_CODE,
    ///
    STRING,
    ///
    TOKEN,
    ///
    ASSET,
    ///
    INT_2,
    ///
    INT_3,
    ///
    INT_4,
    ///
    HALF_2,
    ///
    HALF_3,
    ///
    HALF_4,
    ///
    FLOAT_2,
    ///
    FLOAT_3,
    ///
    FLOAT_4,
    ///
    DOUBLE_2,
    ///
    DOUBLE_3,
    ///
    DOUBLE_4,
    ///
    POINT_3H,
    ///
    POINT_3F,
    ///
    POINT_3D,
    ///
    VECTOR_3H,
    ///
    VECTOR_3F,
    ///
    VECTOR_3D,
    ///
    NORMAL_3H,
    ///
    NORMAL_3F,
    ///
    NORMAL_3D,
    ///
    COLOR_3H,
    ///
    COLOR_3F,
    ///
    COLOR_3D,
    ///
    COLOR_4H,
    ///
    COLOR_4F,
    ///
    COLOR_4D,
    ///
    QUAT_H,
    ///
    QUAT_F,
    ///
    QUAT_D,
    ///
    MATRIX_2D,
    ///
    MATRIX_3D,
    ///
    MATRIX_4D,
    ///
    FRAME_4D,
    ///
    TEX_COORD_2H,
    ///
    TEX_COORD_2F,
    ///
    TEX_COORD_2D,
    ///
    TEX_COORD_3H,
    ///
    TEX_COORD_3F,
    ///
    TEX_COORD_3D,
    ///
    OPAQUE,
    ///
    GROUP,
    ///
    BOOL_ARRAY,
    ///
    UCHAR_ARRAY,
    ///
    INT_ARRAY,
    ///
    UINT_ARRAY,
    ///
    INT64_ARRAY,
    ///
    UINT64_ARRAY,
    ///
    HALF_ARRAY,
    ///
    FLOAT_ARRAY,
    ///
    DOUBLE_ARRAY,
    ///
    TIME_CODE_ARRAY,
    ///
    STRING_ARRAY,
    ///
    TOKEN_ARRAY,
    ///
    ASSET_ARRAY,
    ///
    INT_2_ARRAY,
    ///
    INT_3_ARRAY,
    ///
    INT_4_ARRAY,
    ///
    HALF_2_ARRAY,
    ///
    HALF_3_ARRAY,
    ///
    HALF_4_ARRAY,
    ///
    FLOAT_2_ARRAY,
    ///
    FLOAT_3_ARRAY,
    ///
    FLOAT_4_ARRAY,
    ///
    DOUBLE_2_ARRAY,
    ///
    DOUBLE_3_ARRAY,
    ///
    DOUBLE_4_ARRAY,
    ///
    POINT_3H_ARRAY,
    ///
    POINT_3F_ARRAY,
    ///
    POINT_3D_ARRAY,
    ///
    VECTOR_3H_ARRAY,
    ///
    VECTOR_3F_ARRAY,
    ///
    VECTOR_3D_ARRAY,
    ///
    NORMAL_3H_ARRAY,
    ///
    NORMAL_3F_ARRAY,
    ///
    NORMAL_3D_ARRAY,
    ///
    COLOR_3H_ARRAY,
    ///
    COLOR_3F_ARRAY,
    ///
    COLOR_3D_ARRAY,
    ///
    COLOR_4H_ARRAY,
    ///
    COLOR_4F_ARRAY,
    ///
    COLOR_4D_ARRAY,
    ///
    QUAT_H_ARRAY,
    ///
    QUAT_F_ARRAY,
    ///
    QUAT_D_ARRAY,
    ///
    MATRIX_2D_ARRAY,
    ///
    MATRIX_3D_ARRAY,
    ///
    MATRIX_4D_ARRAY,
    ///
    FRAME_4D_ARRAY,
    ///
    TEX_COORD_2H_ARRAY,
    ///
    TEX_COORD_2F_ARRAY,
    ///
    TEX_COORD_2D_ARRAY,
    ///
    TEX_COORD_3H_ARRAY,
    ///
    TEX_COORD_3F_ARRAY,
    ///
    TEX_COORD_3D_ARRAY,
    END__,
    SIZE__ = END__ - BEGIN__
};

/// \brief Extracts a `ValueTypeName` enum tag from a string view.
///
/// \param[in] view A UTF-8 string view.
/// \returns The `ValueTypeName` enum tag whose serialized form is \p view or
///          `std::nullopt`.
std::optional<ValueTypeName> extract_ValueTypeName(std::string_view const& view);

/// \brief Converts a string view into a `ValueTypeName` enum tag.
///
/// \param[in] view A UTF-8 string view.
/// \returns The `ValueTypeName` enum tag whose serialized form is \p view.
/// \throws std::invalid_argument
ValueTypeName to_ValueTypeName(std::string_view const& view);

std::istream& operator>>(std::istream& is, ValueTypeName& out);

std::ostream& operator<<(std::ostream& os, ValueTypeName const& in);

}  // namespace sdf
}  // namespace usd
}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_USD_SDF_VALUE_TYPE_NAME_HPP

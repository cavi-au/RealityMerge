/**************************************************************************/
/* usd/sdf/value_type_name.cpp                                            */
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
#include "usd/sdf/value_type_name.hpp"

namespace {

using cavi::usdj_am::usd::sdf::ValueTypeName;

static std::map<std::string_view, ValueTypeName> const TAGS = {
    {"bool", ValueTypeName::BOOL},
    {"uchar", ValueTypeName::UCHAR},
    {"int", ValueTypeName::INT},
    {"uint", ValueTypeName::UINT},
    {"int64", ValueTypeName::INT64},
    {"uint64", ValueTypeName::UINT64},
    {"half", ValueTypeName::HALF},
    {"float", ValueTypeName::FLOAT},
    {"double", ValueTypeName::DOUBLE},
    {"timecode", ValueTypeName::TIME_CODE},
    {"string", ValueTypeName::STRING},
    {"token", ValueTypeName::TOKEN},
    {"asset", ValueTypeName::ASSET},
    {"int2", ValueTypeName::INT_2},
    {"int3", ValueTypeName::INT_3},
    {"int4", ValueTypeName::INT_4},
    {"half2", ValueTypeName::HALF_2},
    {"half3", ValueTypeName::HALF_3},
    {"half4", ValueTypeName::HALF_4},
    {"float2", ValueTypeName::FLOAT_2},
    {"float3", ValueTypeName::FLOAT_3},
    {"float4", ValueTypeName::FLOAT_4},
    {"double2", ValueTypeName::DOUBLE_2},
    {"double3", ValueTypeName::DOUBLE_3},
    {"double4", ValueTypeName::DOUBLE_4},
    {"point3h", ValueTypeName::POINT_3H},
    {"point3f", ValueTypeName::POINT_3F},
    {"point3d", ValueTypeName::POINT_3D},
    {"vector3h", ValueTypeName::VECTOR_3H},
    {"vector3f", ValueTypeName::VECTOR_3F},
    {"vector3d", ValueTypeName::VECTOR_3D},
    {"normal3h", ValueTypeName::NORMAL_3H},
    {"normal3f", ValueTypeName::NORMAL_3F},
    {"normal3d", ValueTypeName::NORMAL_3D},
    {"color3h", ValueTypeName::COLOR_3H},
    {"color3f", ValueTypeName::COLOR_3F},
    {"color3d", ValueTypeName::COLOR_3D},
    {"color4h", ValueTypeName::COLOR_4H},
    {"color4f", ValueTypeName::COLOR_4F},
    {"color4d", ValueTypeName::COLOR_4D},
    {"quath", ValueTypeName::QUAT_H},
    {"quatf", ValueTypeName::QUAT_F},
    {"quatd", ValueTypeName::QUAT_D},
    {"matrix2d", ValueTypeName::MATRIX_2D},
    {"matrix3d", ValueTypeName::MATRIX_3D},
    {"matrix4d", ValueTypeName::MATRIX_4D},
    {"frame4d", ValueTypeName::FRAME_4D},
    {"texCoord2h", ValueTypeName::TEX_COORD_2H},
    {"texCoord2f", ValueTypeName::TEX_COORD_2F},
    {"texCoord2d", ValueTypeName::TEX_COORD_2D},
    {"texCoord3h", ValueTypeName::TEX_COORD_3H},
    {"texCoord3f", ValueTypeName::TEX_COORD_3F},
    {"texCoord3d", ValueTypeName::TEX_COORD_3D},
    {"opaque", ValueTypeName::OPAQUE},
    //    {"", ValueTypeName::GROUP},
    {"bool[]", ValueTypeName::BOOL_ARRAY},
    {"uchar[]", ValueTypeName::UCHAR_ARRAY},
    {"int[]", ValueTypeName::INT_ARRAY},
    {"uint[]", ValueTypeName::UINT_ARRAY},
    {"int64[]", ValueTypeName::INT64_ARRAY},
    {"uint64[]", ValueTypeName::UINT64_ARRAY},
    {"half[]", ValueTypeName::HALF_ARRAY},
    {"float[]", ValueTypeName::FLOAT_ARRAY},
    {"double[]", ValueTypeName::DOUBLE_ARRAY},
    {"timecode[]", ValueTypeName::TIME_CODE_ARRAY},
    {"string[]", ValueTypeName::STRING_ARRAY},
    {"token[]", ValueTypeName::TOKEN_ARRAY},
    {"asset[]", ValueTypeName::ASSET_ARRAY},
    {"int2[]", ValueTypeName::INT_2_ARRAY},
    {"int3[]", ValueTypeName::INT_3_ARRAY},
    {"int4[]", ValueTypeName::INT_4_ARRAY},
    {"half2[]", ValueTypeName::HALF_2_ARRAY},
    {"half3[]", ValueTypeName::HALF_3_ARRAY},
    {"half4[]", ValueTypeName::HALF_4_ARRAY},
    {"float2[]", ValueTypeName::FLOAT_2_ARRAY},
    {"float3[]", ValueTypeName::FLOAT_3_ARRAY},
    {"float4[]", ValueTypeName::FLOAT_4_ARRAY},
    {"double2[]", ValueTypeName::DOUBLE_2_ARRAY},
    {"double3[]", ValueTypeName::DOUBLE_3_ARRAY},
    {"double4[]", ValueTypeName::DOUBLE_4_ARRAY},
    {"point3h[]", ValueTypeName::POINT_3H_ARRAY},
    {"point3f[]", ValueTypeName::POINT_3F_ARRAY},
    {"point3d[]", ValueTypeName::POINT_3D_ARRAY},
    {"vector3h[]", ValueTypeName::VECTOR_3H_ARRAY},
    {"vector3f[]", ValueTypeName::VECTOR_3F_ARRAY},
    {"vector3d[]", ValueTypeName::VECTOR_3D_ARRAY},
    {"normal3h[]", ValueTypeName::NORMAL_3H_ARRAY},
    {"normal3f[]", ValueTypeName::NORMAL_3F_ARRAY},
    {"normal3d[]", ValueTypeName::NORMAL_3D_ARRAY},
    {"color3h[]", ValueTypeName::COLOR_3H_ARRAY},
    {"color3f[]", ValueTypeName::COLOR_3F_ARRAY},
    {"color3d[]", ValueTypeName::COLOR_3D_ARRAY},
    {"color4h[]", ValueTypeName::COLOR_4H_ARRAY},
    {"color4f[]", ValueTypeName::COLOR_4F_ARRAY},
    {"color4d[]", ValueTypeName::COLOR_4D_ARRAY},
    {"quath[]", ValueTypeName::QUAT_H_ARRAY},
    {"quatf[]", ValueTypeName::QUAT_F_ARRAY},
    {"quatd[]", ValueTypeName::QUAT_D_ARRAY},
    {"matrix2d[]", ValueTypeName::MATRIX_2D_ARRAY},
    {"matrix3d[]", ValueTypeName::MATRIX_3D_ARRAY},
    {"matrix4d[]", ValueTypeName::MATRIX_4D_ARRAY},
    {"frame4d[]", ValueTypeName::FRAME_4D_ARRAY},
    {"texCoord2h[]", ValueTypeName::TEX_COORD_2H_ARRAY},
    {"texCoord2f[]", ValueTypeName::TEX_COORD_2F_ARRAY},
    {"texCoord2d[]", ValueTypeName::TEX_COORD_2D_ARRAY},
    {"texCoord3h[]", ValueTypeName::TEX_COORD_3H_ARRAY},
    {"texCoord3f[]", ValueTypeName::TEX_COORD_3F_ARRAY},
    {"texCoord3d[]", ValueTypeName::TEX_COORD_3D_ARRAY},
};

}  // namespace

namespace cavi {
namespace usdj_am {
namespace usd {
namespace sdf {

std::optional<ValueTypeName> extract_ValueTypeName(std::string_view const& view) {
    return detail::extract_enum_tag<ValueTypeName>(TAGS, view);
}

ValueTypeName to_ValueTypeName(std::string_view const& view) {
    auto const result = detail::extract_enum_tag<ValueTypeName>(TAGS, view);
    if (!result) {
        std::ostringstream what;
        what << __func__ << "(view == \"" << view << "\")";
        throw std::invalid_argument(what.str());
    }
    return *result;
}

std::istream& operator>>(std::istream& is, ValueTypeName& out) {
    return detail::operator>><ValueTypeName>(is, std::make_pair(std::cref(TAGS), std::ref(out)));
}

std::ostream& operator<<(std::ostream& os, ValueTypeName const& in) {
    os << detail::extract_enum_string<ValueTypeName>(TAGS, in).value_or("???");
    return os;
}

}  // namespace sdf
}  // namespace usd
}  // namespace usdj_am
}  // namespace cavi

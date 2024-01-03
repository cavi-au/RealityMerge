/**************************************************************************/
/* usdj_value.cpp                                                         */
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

// third-party
#include <cavi/usdj_am/declaration.hpp>
#include <cavi/usdj_am/usd/sdf/value_type_name.hpp>
#include <cavi/usdj_am/value.hpp>

// local
#include "usdj_basis.h"
#include "usdj_color.h"
#include "usdj_projection.h"
#include "usdj_quaternion.h"
#include "usdj_real.h"
#include "usdj_value.h"
#include "usdj_vector.h"

std::optional<UsdjValue> extract_UsdjValue(cavi::usdj_am::Declaration const& declaration) {
    using cavi::usdj_am::Declaration;
    using cavi::usdj_am::ValueRange;
    using cavi::usdj_am::usd::sdf::extract_ValueTypeName;
    using cavi::usdj_am::usd::sdf::ValueTypeName;

    std::optional<UsdjValue> usd_value{};
    auto const value_type = extract_ValueTypeName(declaration.get_define_type());
    if (value_type) {
        switch (*value_type) {
            // case ValueTypeName::BOOL:
            // case ValueTypeName::UCHAR:
            // case ValueTypeName::INT:
            // case ValueTypeName::UINT:
            // case ValueTypeName::INT64:
            // case ValueTypeName::UINT64:
            case ValueTypeName::HALF:
            case ValueTypeName::FLOAT:
            case ValueTypeName::DOUBLE: {
                usd_value.emplace(to_real(declaration.get_value()));
                break;
            }
            // case ValueTypeName::TIME_CODE:
            // case ValueTypeName::STRING:
            // case ValueTypeName::TOKEN:
            // case ValueTypeName::ASSET:
            // case ValueTypeName::INT_2:
            case ValueTypeName::INT_3: {
                usd_value.emplace(to_Vector<Vector3i, std::int32_t>(declaration.get_value()));
                break;
            }
            case ValueTypeName::INT_4: {
                usd_value.emplace(to_Vector<Vector4i, std::int32_t>(declaration.get_value()));
                break;
            }
            // case ValueTypeName::HALF_2:
            case ValueTypeName::HALF_3:
            case ValueTypeName::FLOAT_3:
            case ValueTypeName::DOUBLE_3:
            case ValueTypeName::POINT_3H:
            case ValueTypeName::POINT_3F:
            case ValueTypeName::POINT_3D:
            case ValueTypeName::VECTOR_3H:
            case ValueTypeName::VECTOR_3F:
            case ValueTypeName::VECTOR_3D:
            case ValueTypeName::NORMAL_3H:
            case ValueTypeName::NORMAL_3F:
            case ValueTypeName::NORMAL_3D: {
                usd_value.emplace(to_Vector<Vector3, real_t>(declaration.get_value()));
                break;
            }
            case ValueTypeName::COLOR_3H:
            case ValueTypeName::COLOR_3F:
            case ValueTypeName::COLOR_3D:
            case ValueTypeName::COLOR_4H:
            case ValueTypeName::COLOR_4F:
            case ValueTypeName::COLOR_4D: {
                usd_value.emplace(to_Color(declaration.get_value()));
                break;
            }
            case ValueTypeName::QUAT_H:
            case ValueTypeName::QUAT_F:
            case ValueTypeName::QUAT_D: {
                usd_value.emplace(to_Quaternion(declaration.get_value()));
                break;
            }
            // case ValueTypeName::MATRIX_2D:
            case ValueTypeName::MATRIX_3D: {
                usd_value.emplace(to_Basis(declaration.get_value()));
                break;
            }
            case ValueTypeName::MATRIX_4D:
            case ValueTypeName::FRAME_4D: {
                usd_value.emplace(to_Projection(declaration.get_value()));
                break;
            }
            // case ValueTypeName::TEX_COORD_2H:
            // case ValueTypeName::TEX_COORD_2F:
            // case ValueTypeName::TEX_COORD_2D:
            // case ValueTypeName::TEX_COORD_3H:
            // case ValueTypeName::TEX_COORD_3F:
            // case ValueTypeName::TEX_COORD_3D:
            // case ValueTypeName::OPAQUE:
            // case ValueTypeName::GROUP:
            // case ValueTypeName::BOOL_ARRAY:
            // case ValueTypeName::UCHAR_ARRAY:
            // case ValueTypeName::INT_ARRAY:
            // case ValueTypeName::UINT_ARRAY:
            // case ValueTypeName::INT64_ARRAY:
            // case ValueTypeName::UINT64_ARRAY:
            case ValueTypeName::HALF_ARRAY:
            case ValueTypeName::FLOAT_ARRAY:
            case ValueTypeName::DOUBLE_ARRAY: {
                usd_value.emplace(to_reals(declaration.get_value()));
                break;
            }
            // case ValueTypeName::TIME_CODE_ARRAY:
            // case ValueTypeName::STRING_ARRAY:
            // case ValueTypeName::TOKEN_ARRAY:
            // case ValueTypeName::ASSET_ARRAY:
            // case ValueTypeName::INT_2_ARRAY:
            // case ValueTypeName::INT_3_ARRAY:
            // case ValueTypeName::INT_4_ARRAY:
            // case ValueTypeName::HALF_2_ARRAY:
            // case ValueTypeName::HALF_3_ARRAY:
            // case ValueTypeName::HALF_4_ARRAY:
            // case ValueTypeName::FLOAT_2_ARRAY:
            // case ValueTypeName::FLOAT_3_ARRAY:
            // case ValueTypeName::FLOAT_4_ARRAY:
            // case ValueTypeName::DOUBLE_2_ARRAY:
            // case ValueTypeName::DOUBLE_3_ARRAY:
            // case ValueTypeName::DOUBLE_4_ARRAY:
            // case ValueTypeName::POINT_3H_ARRAY:
            // case ValueTypeName::POINT_3F_ARRAY:
            // case ValueTypeName::POINT_3D_ARRAY:
            // case ValueTypeName::VECTOR_3H_ARRAY:
            // case ValueTypeName::VECTOR_3F_ARRAY:
            // case ValueTypeName::VECTOR_3D_ARRAY:
            // case ValueTypeName::NORMAL_3H_ARRAY:
            // case ValueTypeName::NORMAL_3F_ARRAY:
            // case ValueTypeName::NORMAL_3D_ARRAY:
            case ValueTypeName::COLOR_3H_ARRAY:
            case ValueTypeName::COLOR_3F_ARRAY:
            case ValueTypeName::COLOR_3D_ARRAY:
            case ValueTypeName::COLOR_4H_ARRAY:
            case ValueTypeName::COLOR_4F_ARRAY:
            case ValueTypeName::COLOR_4D_ARRAY: {
                usd_value.emplace(to_Color(declaration.get_value()));
                break;
            }
            // case ValueTypeName::QUAT_H_ARRAY:
            // case ValueTypeName::QUAT_F_ARRAY:
            // case ValueTypeName::QUAT_D_ARRAY:
            // case ValueTypeName::MATRIX_2D_ARRAY:
            // case ValueTypeName::MATRIX_3D_ARRAY:
            // case ValueTypeName::MATRIX_4D_ARRAY:
            // case ValueTypeName::FRAME_4D_ARRAY:
            // case ValueTypeName::TEX_COORD_2H_ARRAY:
            // case ValueTypeName::TEX_COORD_2F_ARRAY:
            // case ValueTypeName::TEX_COORD_2D_ARRAY:
            // case ValueTypeName::TEX_COORD_3H_ARRAY:
            // case ValueTypeName::TEX_COORD_3F_ARRAY:
            // case ValueTypeName::TEX_COORD_3D_ARRAY:
            default: {
                std::ostringstream what;
                what << __func__ << "((" << typeid(value_type).name()
                     << ")declaration.get_define_type() == " << *value_type << ") not yet implemented.";
                throw std::logic_error(what.str());
                break;
            }
        }
    }
    return usd_value;
}

UsdjValue to_UsdjValue(cavi::usdj_am::Declaration const& declaration) {
    std::optional<UsdjValue> usd_value;
    try {
        usd_value = extract_UsdjValue(declaration);
    } catch (std::logic_error const&) {
        throw;
    }
    if (!usd_value) {
        std::ostringstream what;
        what << __func__ << "(declaration.get_define_type() == \"" << declaration.get_define_type() << "\")";
    }
    return *usd_value;
}

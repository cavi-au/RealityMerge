/**************************************************************************/
/* usdj_transform_3d_extractor.cpp                                        */
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
#include <stdexcept>
#include <type_traits>

// third-party
#include <cavi/usdj_am/declaration.hpp>
#include <cavi/usdj_am/definition.hpp>
#include <cavi/usdj_am/definition_statement.hpp>
#include <cavi/usdj_am/string_.hpp>
#include <cavi/usdj_am/usd/geom/token_type.hpp>
#include <cavi/usdj_am/usd/geom/xform_op_type.hpp>
#include <cavi/usdj_am/usd/sdf/value_type_name.hpp>
#include <cavi/usdj_am/value.hpp>

// regional
#include <core/math/transform_3d.h>

// local
#include "usdj_transform_3d_extractor.h"
#include "usdj_value.h"

struct UsdjTransform3dExtractor::Data {
    cavi::usdj_am::usd::geom::XformOpTypeOrder ops;
    std::map<cavi::usdj_am::usd::geom::XformOpType, UsdjValue> values;
};

UsdjTransform3dExtractor::UsdjTransform3dExtractor(cavi::usdj_am::Definition const& p_definition)
    : m_definition{p_definition} {}

UsdjTransform3dExtractor::~UsdjTransform3dExtractor() {}

std::optional<Transform3D> UsdjTransform3dExtractor::operator()() {
    using cavi::usdj_am::usd::geom::XformOpType;

    std::optional<Transform3D> result;
    if (!m_data) {
        m_data = std::make_unique<Data>();
        m_definition.accept(*this);
    }
    if (m_data) {
        Transform3D xform{};
        for (auto const op : m_data->ops) {
            switch (op) {
                case XformOpType::ORIENT: {
                    auto const& quaternion = std::get<Quaternion>(m_data->values[op]);
                    xform.basis = Basis{quaternion} * xform.basis;
                    break;
                }
                case XformOpType::ROTATE_X: {
                    static Vector3 const AXIS{1.0, 0.0, 0.0};

                    auto const& angle = std::get<real_t>(m_data->values[op]);
                    xform = xform.rotated_local(AXIS, angle);
                    break;
                }
                case XformOpType::ROTATE_Y: {
                    static Vector3 const AXIS{0.0, 1.0, 0.0};

                    auto const& angle = std::get<real_t>(m_data->values[op]);
                    xform = xform.rotated_local(AXIS, angle);
                    break;
                }
                case XformOpType::ROTATE_Z: {
                    static Vector3 const AXIS{0.0, 0.0, 1.0};

                    auto const& angle = std::get<real_t>(m_data->values[op]);
                    xform = xform.rotated_local(AXIS, angle);
                    break;
                }
                case XformOpType::ROTATE_XYZ: {
                    auto const& euler = std::get<Vector3>(m_data->values[op]);
                    xform.basis = Basis::from_euler(euler, EulerOrder::XYZ) * xform.basis;
                    break;
                }
                case XformOpType::ROTATE_XZY: {
                    auto const& euler = std::get<Vector3>(m_data->values[op]);
                    xform.basis = Basis::from_euler(euler, EulerOrder::XZY) * xform.basis;
                    break;
                }
                case XformOpType::ROTATE_YXZ: {
                    auto const& euler = std::get<Vector3>(m_data->values[op]);
                    xform.basis = Basis::from_euler(euler, EulerOrder::YXZ) * xform.basis;
                    break;
                }
                case XformOpType::ROTATE_YZX: {
                    auto const& euler = std::get<Vector3>(m_data->values[op]);
                    xform.basis = Basis::from_euler(euler, EulerOrder::YZX) * xform.basis;
                    break;
                }
                case XformOpType::ROTATE_ZXY: {
                    auto const& euler = std::get<Vector3>(m_data->values[op]);
                    xform.basis = Basis::from_euler(euler, EulerOrder::ZXY) * xform.basis;
                    break;
                }
                case XformOpType::ROTATE_ZYX: {
                    auto const& euler = std::get<Vector3>(m_data->values[op]);
                    xform.basis = Basis::from_euler(euler, EulerOrder::ZYX) * xform.basis;
                    break;
                }
                case XformOpType::SCALE: {
                    auto const& scale = std::get<Vector3>(m_data->values[op]);
                    xform = xform.scaled_local(scale);
                    break;
                }
                case XformOpType::TRANSFORM: {
                    // 4x4 matrix
                    auto const& projection = std::get<Projection>(m_data->values[op]);
                    xform = projection * Projection{xform};
                    break;
                }
                case XformOpType::TRANSLATE: {
                    auto const& offset = std::get<Vector3>(m_data->values[op]);
                    xform = xform.translated_local(offset);
                    break;
                }
                case XformOpType::RESET_XFORM_STACK: {
                    /// \note We can't do anything with this because there aren't
                    ///       any previous transformations on the stack to ignore.
                    break;
                }
            }
        }
        result.emplace(xform);
    }
    return result;
}

void UsdjTransform3dExtractor::visit(cavi::usdj_am::Definition const& definition) {
    for (auto const& definition_statement : definition.get_statements()) {
        definition_statement.accept(*this);
    }
}

void UsdjTransform3dExtractor::visit(cavi::usdj_am::DefinitionStatement const& definition_statement) {
    using cavi::usdj_am::Declaration;

    std::visit(
        [this](auto const& alt) {
            using T = std::decay_t<decltype(alt)>;
            if constexpr (std::is_same_v<T, Declaration>)
                alt.accept(*this);
        },
        definition_statement);
}

void UsdjTransform3dExtractor::visit(cavi::usdj_am::Declaration const& declaration) {
    using cavi::usdj_am::DeclarationKeyword;
    using cavi::usdj_am::usd::geom::extract_TokenType;
    using cavi::usdj_am::usd::geom::extract_XformOpType;
    using cavi::usdj_am::usd::geom::extract_XformOpTypeOrder;
    using cavi::usdj_am::usd::geom::TokenType;
    using cavi::usdj_am::usd::geom::XformOpType;
    using cavi::usdj_am::usd::sdf::extract_ValueTypeName;
    using cavi::usdj_am::usd::sdf::ValueTypeName;

    if (!declaration.get_descriptor()) {
        auto const keyword = declaration.get_keyword();
        auto const reference = declaration.get_reference();
        if (!keyword) {
            auto key = extract_XformOpType(reference);
            auto value = extract_UsdjValue(declaration);
            if (key && value)
                m_data->values.insert(std::make_pair(std::move(*key), std::move(*value)));
        } else if (*keyword == DeclarationKeyword::UNIFORM &&
                   extract_TokenType(reference).value_or(TokenType{}) == TokenType::XFORM_OP_ORDER &&
                   extract_ValueTypeName(declaration.get_define_type()).value_or(ValueTypeName{}) ==
                       ValueTypeName::TOKEN_ARRAY) {
            m_data->ops = extract_XformOpTypeOrder(declaration.get_value());
        }
    }
}

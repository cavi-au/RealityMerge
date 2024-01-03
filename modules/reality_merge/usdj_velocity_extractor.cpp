/**************************************************************************/
/* usdj_velocity_extractor.cpp                                            */
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
#include <type_traits>

// third-party
#include <cavi/usdj_am/declaration.hpp>
#include <cavi/usdj_am/definition.hpp>
#include <cavi/usdj_am/definition_statement.hpp>

// regional
#include <core/math/vector3.h>
#include <core/math/vector3i.h>

// local
#include "usdj_value.h"
#include "usdj_velocity_extractor.h"

UsdjVelocityExtractor::UsdjVelocityExtractor(cavi::usdj_am::Definition const& p_definition)
    : m_definition{p_definition} {}

UsdjVelocityExtractor::~UsdjVelocityExtractor() {}

std::optional<Vector3> UsdjVelocityExtractor::operator()(cavi::usdj_am::usd::physics::TokenType const reference) {
    using cavi::usdj_am::usd::physics::TokenType;

    if (!(reference == TokenType::PHYSICS_ANGULAR_VELOCITY || reference == TokenType::PHYSICS_VELOCITY)) {
        std::ostringstream what;
        what << typeid(*this).name() << "::" << __func__ << "(reference == " << reference << ")";
        throw std::invalid_argument(what.str());
    }
    m_reference.emplace(reference);
    m_definition.accept(*this);
    return m_velocity;
}

void UsdjVelocityExtractor::visit(cavi::usdj_am::Declaration const& declaration) {
    using cavi::usdj_am::usd::physics::extract_TokenType;
    using cavi::usdj_am::usd::physics::TokenType;

    if (!(declaration.get_keyword() || declaration.get_descriptor()) &&
        extract_TokenType(declaration.get_reference()).value_or(TokenType{}) == *m_reference) {
        auto const usd_value = extract_UsdjValue(declaration);
        if (usd_value) {
            std::visit(
                [this](auto const& alt) {
                    using T = std::decay_t<decltype(alt)>;
                    if constexpr (std::is_same_v<T, Vector3> || std::is_same_v<T, Vector3i>)
                        this->m_velocity.emplace(alt);
                },
                *usd_value);
        }
    }
}

void UsdjVelocityExtractor::visit(cavi::usdj_am::Definition const& definition) {
    for (auto const& definition_statement : definition.get_statements()) {
        if (m_velocity)
            break;
        definition_statement.accept(*this);
    }
}

void UsdjVelocityExtractor::visit(cavi::usdj_am::DefinitionStatement const& definition_statement) {
    using cavi::usdj_am::Declaration;

    std::visit(
        [this](auto const& alt) {
            using T = std::decay_t<decltype(alt)>;
            if constexpr (std::is_same_v<T, Declaration>)
                alt.accept(*this);
        },
        definition_statement);
}

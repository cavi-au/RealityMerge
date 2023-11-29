/**************************************************************************/
/* usdj_color_extractor.cpp                                               */
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

#include <stdexcept>
#include <type_traits>

// third-party
#include <cavi/usdj_am/declaration.hpp>
#include <cavi/usdj_am/definition.hpp>
#include <cavi/usdj_am/definition_statement.hpp>
#include <cavi/usdj_am/usd/geom/token_type.hpp>

// regional
#include <core/math/color.h>

// local
#include "usdj_color_extractor.h"
#include "usdj_value.h"

UsdjColorExtractor::UsdjColorExtractor(std::shared_ptr<cavi::usdj_am::Definition> const& definition)
    : m_definition{definition} {}

UsdjColorExtractor::~UsdjColorExtractor() {}

std::optional<Color> UsdjColorExtractor::operator()() {
    auto const definition = m_definition.lock();
    if (definition && m_components.empty()) {
        definition->accept(*this);
    }
    std::optional<Color> color;
    switch (m_components.size()) {
        case 3: {
            color.emplace(
                Color{m_components.at(Component::R), m_components.at(Component::G), m_components.at(Component::B)});
            break;
        }
        case 4: {
            color.emplace(Color{m_components.at(Component::R), m_components.at(Component::G),
                                m_components.at(Component::B), m_components.at(Component::A)});
            break;
        }
    }
    return color;
}

void UsdjColorExtractor::visit(cavi::usdj_am::Declaration const& declaration) {
    using cavi::usdj_am::usd::geom::extract_TokenType;
    using cavi::usdj_am::usd::geom::TokenType;

    if (!(declaration.get_descriptor() || declaration.get_keyword())) {
        auto const usd_value = extract_UsdjValue(declaration);
        if (usd_value) {
            switch (extract_TokenType(declaration.get_reference()).value_or(TokenType{})) {
                case TokenType::PRIMVARS_DISPLAY_COLOR: {
                    std::visit(
                        [&](auto const& alt) {
                            using T = std::decay_t<decltype(alt)>;
                            if constexpr (std::is_same_v<T, Color>) {
                                m_components[Component::R] = alt.r;
                                m_components[Component::G] = alt.g;
                                m_components[Component::B] = alt.b;
                                if (alt.a != 1.0) {
                                    m_components[Component::A] = alt.a;
                                }
                            }
                        },
                        *usd_value);
                    break;
                }
                case TokenType::PRIMVARS_DISPLAY_OPACITY: {
                    std::visit(
                        [&](auto const& alt) {
                            using T = std::decay_t<decltype(alt)>;
                            if constexpr (std::is_same_v<T, Reals>)
                                m_components[Component::A] = alt.at(0);
                        },
                        *usd_value);
                    break;
                }
            }
        }
    }
}

void UsdjColorExtractor::visit(cavi::usdj_am::Definition const& definition) {
    for (auto const& definition_statement : definition.get_statements()) {
        if (m_components.size() == 4)
            break;
        definition_statement.accept(*this);
    }
}

void UsdjColorExtractor::visit(cavi::usdj_am::DefinitionStatement const& definition_statement) {
    using cavi::usdj_am::Declaration;

    std::visit(
        [this](auto const& alt) {
            using T = std::decay_t<decltype(alt)>;
            if constexpr (std::is_same_v<T, std::unique_ptr<Declaration>>)
                alt->accept(*this);
        },
        definition_statement);
}

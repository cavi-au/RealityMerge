/**************************************************************************/
/* usdj_box_size_extractor.cpp                                            */
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
#include <cavi/usdj_am/assignment.hpp>
#include <cavi/usdj_am/declaration.hpp>
#include <cavi/usdj_am/definition.hpp>
#include <cavi/usdj_am/definition_statement.hpp>
#include <cavi/usdj_am/external_reference.hpp>
#include <cavi/usdj_am/reference_file.hpp>
#include <cavi/usdj_am/usd/geom/token_type.hpp>

// regional
#include <core/math/vector3.h>
#include <core/math/vector3i.h>

// local
#include "usdj_box_size_extractor.h"
#include "usdj_value.h"

UsdjBoxSizeExtractor::UsdjBoxSizeExtractor(std::shared_ptr<cavi::usdj_am::Definition> const& definition)
    : m_definition{definition} {}

UsdjBoxSizeExtractor::~UsdjBoxSizeExtractor() {}

std::optional<Vector3> UsdjBoxSizeExtractor::operator()() {
    auto const definition = m_definition.lock();
    if (definition)
        definition->accept(*this);
    return m_size;
}

void UsdjBoxSizeExtractor::visit(cavi::usdj_am::Assignment const& assignment) {
    using cavi::usdj_am::AssignmentKeyword;
    using cavi::usdj_am::ExternalReference;

    if (assignment.get_keyword().value_or(AssignmentKeyword{}) == AssignmentKeyword::PREPEND &&
        assignment.get_identifier() == "references") {
        std::visit(
            [this](auto const& alt) {
                using T = std::decay_t<decltype(alt)>;
                if constexpr (std::is_same_v<T, std::unique_ptr<ExternalReference>>) {
                    alt->accept(*this);
                }
            },
            assignment.get_value());
    }
}

void UsdjBoxSizeExtractor::visit(cavi::usdj_am::Declaration const& declaration) {
    using cavi::usdj_am::usd::geom::extract_TokenType;
    using cavi::usdj_am::usd::geom::TokenType;

    if (!(declaration.get_descriptor() || declaration.get_keyword()) &&
        extract_TokenType(declaration.get_reference()).value_or(TokenType{}) == TokenType::SIZE) {
        auto const usd_value = extract_UsdjValue(declaration);
        if (usd_value) {
            std::visit(
                [this](auto const& alt) {
                    using T = std::decay_t<decltype(alt)>;
                    if constexpr (std::is_same_v<T, real_t>)
                        this->m_size.emplace(Vector3{1.0, 1.0, 1.0} * alt);
                    else if constexpr (std::is_same_v<T, Vector3> || std::is_same_v<T, Vector3i>)
                        this->m_size.emplace(alt);
                },
                *usd_value);
        }
    }
}

void UsdjBoxSizeExtractor::visit(cavi::usdj_am::Definition const& definition) {
    using cavi::usdj_am::DefinitionType;
    using cavi::usdj_am::usd::geom::extract_TokenType;
    using cavi::usdj_am::usd::geom::TokenType;

    if (definition.get_sub_type() == DefinitionType::DEF) {
        auto const def_type = definition.get_def_type();
        auto const descriptor = definition.get_descriptor();
        if (!def_type && descriptor) {
            descriptor->accept(*this);
        } else if (def_type && extract_TokenType(*def_type) == TokenType::CUBE) {
            for (auto const& definition_statement : definition.get_statements()) {
                if (m_size)
                    break;
                definition_statement.accept(*this);
            }
        }
    }
}

void UsdjBoxSizeExtractor::visit(cavi::usdj_am::DefinitionStatement const& definition_statement) {
    using cavi::usdj_am::Declaration;

    std::visit(
        [this](auto const& alt) {
            using T = std::decay_t<decltype(alt)>;
            if constexpr (std::is_same_v<T, std::unique_ptr<Declaration>>)
                alt->accept(*this);
        },
        definition_statement);
}

void UsdjBoxSizeExtractor::visit(cavi::usdj_am::Descriptor const& descriptor) {
    for (auto const& assignment : descriptor.get_assignments()) {
        if (m_size)
            break;
        assignment.accept(*this);
    }
}

void UsdjBoxSizeExtractor::visit(cavi::usdj_am::ExternalReference const& external_reference) {
    if (!external_reference.get_to_import()) {
        external_reference.get_reference_file().accept(*this);
    }
}

void UsdjBoxSizeExtractor::visit(cavi::usdj_am::ReferenceFile const& reference_file) {
    if (!reference_file.get_descriptor()) {
        /// \todo Actually load the referenced file and extract the size
        ///       attribute from within it instead of assuming unit length.
        if (reference_file.get_src() == "/cube.usda") {
            m_size.emplace(Vector3{1.0, 1.0, 1.0});
        }
    }
}

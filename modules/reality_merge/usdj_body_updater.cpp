/**************************************************************************/
/* usdj_body_updater.cpp                                                  */
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

#include <algorithm>
#include <cassert>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>

// third_party
extern "C" {

#include <automerge-c/automerge.h>
}
#include <cavi/usdj_am/assignment.hpp>
#include <cavi/usdj_am/definition_statement.hpp>
#include <cavi/usdj_am/definition_type.hpp>
#include <cavi/usdj_am/descriptor.hpp>
#include <cavi/usdj_am/file.hpp>
#include <cavi/usdj_am/statement.hpp>
#include <cavi/usdj_am/usd/geom/token_type.hpp>
#include <cavi/usdj_am/utils/document.hpp>

// regional
#include <core/error/error_macros.h>
#include <core/object/object.h>
#include <core/os/memory.h>
#include <core/string/ustring.h>

// local
#include "usdj_body_updater.h"
#include "usdj_static_body_3d.h"

UsdjBodyUpdater::UsdjBodyUpdater(TypedArray<Node> const& nodes) : m_visited_default_prim{false} {
    for (int pos = 0; pos != nodes.size(); ++pos) {
        auto const body = Object::cast_to<Body>(nodes[pos]);
        if (body)
            m_bodies.push_back(body);
    }
}

UsdjBodyUpdater::~UsdjBodyUpdater() {}

UsdjBodyUpdater::Updates UsdjBodyUpdater::operator()(cavi::usdj_am::utils::Document const& document,
                                                     std::string const& path) {
    using cavi::usdj_am::File;

    auto const file = File{document, document.get_item(path)};
    file.accept(*this);
    // Any remaining bodies should be removed because they originated
    // from expired USD prims.
    while (!m_bodies.empty()) {
        auto const body = m_bodies.front();
        m_updates.insert(Updates::value_type{Action::REMOVE, body});
        m_bodies.pop_front();
    }
    return m_updates;
}

void UsdjBodyUpdater::visit(cavi::usdj_am::Assignment const& assignment) {
    using cavi::usdj_am::String;

    if (!assignment.get_keyword() && assignment.get_identifier() == "defaultPrim") {
        std::visit(
            [this](auto const& alt) {
                using T = std::decay_t<decltype(alt)>;
                if constexpr (std::is_same_v<T, String>)
                    this->m_default_prim.emplace(alt);
            },
            assignment.get_value());
    }
}

void UsdjBodyUpdater::visit(cavi::usdj_am::Definition const& definition) {
    using cavi::usdj_am::DefinitionType;
    using cavi::usdj_am::usd::geom::extract_TokenType;
    using cavi::usdj_am::usd::geom::TokenType;

    if (definition.get_sub_type() != DefinitionType::DEF) {
        return;
    }
    if (!m_visited_default_prim) {
        auto const def_type = definition.get_def_type();
        if (def_type && extract_TokenType(*def_type).value_or(TokenType{}) == TokenType::XFORM && m_default_prim &&
            definition.get_name() == *m_default_prim) {
            m_visited_default_prim = true;
            for (auto&& definition_statement : definition.get_statements()) {
                std::forward<decltype(definition_statement)>(definition_statement).accept(*this);
            }
        }
        return;
    }
    auto const descriptor = definition.get_descriptor();
    if (!descriptor) {
        return;
    }
    auto const body_id = definition.get_object_id();
    auto const match = std::find_if(m_bodies.begin(), m_bodies.end(), [body_id](auto const& body) {
        auto const static_body_3d = dynamic_cast<UsdjStaticBody3D*>(body);
        return static_body_3d && AMobjIdEqual(static_body_3d->get_object_id(), body_id);
    });
    if (match == m_bodies.end()) {
        auto const usd_body = memnew(UsdjStaticBody3D{std::move(m_definition.value())});
        m_updates.insert({Action::ADD, usd_body});
    } else {
        m_updates.insert({Action::KEEP, *match});
        m_bodies.erase(match);
    }
}

void UsdjBodyUpdater::visit(cavi::usdj_am::DefinitionStatement&& definition_statement) {
    using cavi::usdj_am::Statement;

    std::visit(
        [this](auto&& alt) {
            using T = std::decay_t<decltype(alt)>;
            if constexpr (std::is_same_v<T, Statement>)
                std::forward<Statement>(alt).accept(*this);
        },
        definition_statement);
}

void UsdjBodyUpdater::visit(cavi::usdj_am::Descriptor const& descriptor) {
    for (auto const& assignment : descriptor.get_assignments()) {
        if (m_default_prim)
            break;
        assignment.accept(*this);
    }
}

void UsdjBodyUpdater::visit(cavi::usdj_am::File const& file) {
    using cavi::usdj_am::Definition;
    using cavi::usdj_am::DefinitionType;
    using cavi::usdj_am::Statement;
    using cavi::usdj_am::String;

    auto const version = std::visit([](auto const& alt) { return static_cast<std::size_t>(alt); }, file.get_version());
    ERR_FAIL_COND_MSG(version != 1, "version != 1");
    auto const descriptor = file.get_descriptor();
    ERR_FAIL_COND_MSG(!descriptor, "File.descriptor == null");
    descriptor->accept(*this);
    ERR_FAIL_COND_MSG(!m_default_prim, "\"defaultPrim\" assignment not found!");
    for (auto&& statement : file.get_statements()) {
        std::forward<decltype(statement)>(statement).accept(*this);
    }
}

void UsdjBodyUpdater::visit(cavi::usdj_am::Statement&& statement) {
    using cavi::usdj_am::Definition;

    std::visit(
        [this](auto&& alt) {
            using T = std::decay_t<decltype(alt)>;
            if constexpr (std::is_same_v<T, Definition>) {
                // Enable the definition to be transferred to a new body.
                this->m_definition.emplace(std::move(alt));
                this->m_definition->accept(*this);
            }
        },
        statement);
    if (!m_definition) {
        // This statement was skipped.
    }
}

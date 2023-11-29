/**************************************************************************/
/* usdj_geometry_extractor.cpp                                            */
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

#include <memory>
#include <stdexcept>
#include <type_traits>

// third-party
#include <cavi/usdj_am/assignment.hpp>
#include <cavi/usdj_am/definition.hpp>
#include <cavi/usdj_am/descriptor.hpp>
#include <cavi/usdj_am/external_reference.hpp>
#include <cavi/usdj_am/reference_file.hpp>
#include <cavi/usdj_am/usd/geom/token_type.hpp>
#include <cavi/usdj_am/usd/physics/token_type.hpp>
#include <cavi/usdj_am/usd/token_type.hpp>

// regional
#include <core/os/memory.h>

// local
#include "usdj_box_mesh.h"
#include "usdj_box_shape_3d.h"
#include "usdj_geometry_extractor.h"

UsdjGeometryExtractor::UsdjGeometryExtractor(std::shared_ptr<cavi::usdj_am::Definition> const& definition)
    : m_definition{definition} {}

UsdjGeometryExtractor::~UsdjGeometryExtractor() {}

std::pair<UsdjGeometryExtractor::MeshPtr, UsdjGeometryExtractor::Shape3dPtr> UsdjGeometryExtractor::operator()() {
    namespace geom = cavi::usdj_am::usd::geom;
    namespace physics = cavi::usdj_am::usd::physics;

    auto const definition = m_definition.lock();
    if (definition)
        definition->accept(*this);
    auto geometry = std::make_pair(MeshPtr{nullptr, nullptr}, Shape3dPtr{nullptr, nullptr});
    switch (m_geom_type.value_or(geom::TokenType{})) {
        case geom::TokenType::CUBE: {
            geometry.first = MeshPtr{memnew(UsdjBoxMesh{definition}), DefaultAllocator::free};
            if (m_physics_apis.count(physics::TokenType::PHYSICS_COLLISION_API)) {
                geometry.second = Shape3dPtr{memnew(UsdjBoxShape3D{definition}), DefaultAllocator::free};
            }
            break;
        }
            /// \todo Handle other types of gprim.
    }
    return geometry;
}

void UsdjGeometryExtractor::visit(cavi::usdj_am::Assignment const& assignment) {
    using cavi::usdj_am::AssignmentKeyword;
    using cavi::usdj_am::ExternalReference;
    namespace physics = cavi::usdj_am::usd::physics;
    namespace usd = cavi::usdj_am::usd;

    if (assignment.get_keyword().value_or(AssignmentKeyword{}) == AssignmentKeyword::PREPEND) {
        if (usd::extract_TokenType(assignment.get_identifier()).value_or(usd::TokenType{}) ==
            usd::TokenType::API_SCHEMAS) {
            m_physics_apis = physics::extract_TokenTypeSet(assignment.get_value());
        } else if (assignment.get_identifier() == "references") {
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
}

void UsdjGeometryExtractor::visit(cavi::usdj_am::Definition const& definition) {
    using cavi::usdj_am::AssignmentKeyword;
    using cavi::usdj_am::usd::geom::extract_TokenType;
    using cavi::usdj_am::usd::geom::TokenType;

    auto const def_type = definition.get_def_type();
    if (def_type) {
        m_geom_type = extract_TokenType(*def_type);
    }
    auto const descriptor = definition.get_descriptor();
    if (descriptor) {
        descriptor->accept(*this);
    }
}

void UsdjGeometryExtractor::visit(cavi::usdj_am::Descriptor const& descriptor) {
    for (auto const& assignment : descriptor.get_assignments()) {
        if (m_geom_type && !m_physics_apis.empty())
            break;
        assignment.accept(*this);
    }
}

void UsdjGeometryExtractor::visit(cavi::usdj_am::ExternalReference const& external_reference) {
    if (!external_reference.get_to_import()) {
        external_reference.get_reference_file().accept(*this);
    }
}

void UsdjGeometryExtractor::visit(cavi::usdj_am::ReferenceFile const& reference_file) {
    using cavi::usdj_am::usd::geom::TokenType;

    if (!reference_file.get_descriptor()) {
        /// \todo Actually load the referenced file and extract the gprim and
        ///       API schemas from within it instead of assuming a cube and
        ///       no collision API.
        if (reference_file.get_src() == "/cube.usda") {
            m_geom_type.emplace(TokenType::CUBE);
        }
    }
}

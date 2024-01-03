/**************************************************************************/
/* usdj_geometry_extractor.h                                              */
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

#ifndef REALITY_MERGE_USDJ_GEOMETRY_EXTRACTOR_H
#define REALITY_MERGE_USDJ_GEOMETRY_EXTRACTOR_H

#include <memory>
#include <optional>
#include <utility>

// third-party
#include <cavi/usdj_am/usd/geom/token_type.hpp>
#include <cavi/usdj_am/usd/physics/token_type.hpp>
#include <cavi/usdj_am/visitor.hpp>

// regional
#include <core/object/ref_counted.h>

class Mesh;
class Shape3D;
struct Vector3;

/// \brief An extractor of a mesh and, optionally, a collision shape embedded
///        within a "USDA_Definition" node.
class UsdjGeometryExtractor : public cavi::usdj_am::Visitor {
public:
    using MeshPtr = Ref<Mesh>;
    using Shape3dPtr = Ref<Shape3D>;

    UsdjGeometryExtractor() = delete;

    UsdjGeometryExtractor(cavi::usdj_am::Definition const& p_definition);

    UsdjGeometryExtractor(UsdjGeometryExtractor const&) = delete;

    UsdjGeometryExtractor(UsdjGeometryExtractor&&) = default;

    ~UsdjGeometryExtractor();

    UsdjGeometryExtractor& operator=(UsdjGeometryExtractor const&) = delete;

    UsdjGeometryExtractor& operator=(UsdjGeometryExtractor&&) = default;

    std::pair<MeshPtr, Shape3dPtr> operator()();

    void visit(cavi::usdj_am::Assignment const& assignment) override;

    void visit(cavi::usdj_am::Definition const& definition) override;

    void visit(cavi::usdj_am::Descriptor const& descriptor) override;

    void visit(cavi::usdj_am::ExternalReference const& external_reference) override;

    void visit(cavi::usdj_am::ReferenceFile const& reference_file) override;

private:
    cavi::usdj_am::Definition const& m_definition;
    std::optional<cavi::usdj_am::usd::geom::TokenType> m_geom_type;
    cavi::usdj_am::usd::physics::TokenTypeSet m_physics_apis;
};

#endif  // REALITY_MERGE_USDJ_GEOMETRY_EXTRACTOR_H

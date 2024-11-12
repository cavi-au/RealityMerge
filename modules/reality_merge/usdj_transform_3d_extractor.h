/**************************************************************************/
/* usdj_transform_3d_extractor.h                                          */
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

#ifndef REALITY_MERGE_USDJ_TRANSFORM_3D_EXTRACTOR_H
#define REALITY_MERGE_USDJ_TRANSFORM_3D_EXTRACTOR_H

#include <memory>
#include <optional>

// third-party
#include <cavi/usdj_am/visitor.hpp>

struct Transform3D;

/// \brief An extractor of a transform value embedded within a "USDA_Definition"
///        node.
class UsdjTransform3dExtractor : public cavi::usdj_am::Visitor {
public:
    UsdjTransform3dExtractor() = delete;

    UsdjTransform3dExtractor(cavi::usdj_am::Definition const& p_definition);

    UsdjTransform3dExtractor(UsdjTransform3dExtractor const&) = delete;

    UsdjTransform3dExtractor(UsdjTransform3dExtractor&&) = default;

    ~UsdjTransform3dExtractor();

    UsdjTransform3dExtractor& operator=(UsdjTransform3dExtractor const&) = delete;

    UsdjTransform3dExtractor& operator=(UsdjTransform3dExtractor&&) = delete;

    std::optional<Transform3D> operator()();

    void visit(cavi::usdj_am::Definition const& definition) override;

    void visit(cavi::usdj_am::DefinitionStatement const& definition_statement) override;

    void visit(cavi::usdj_am::Declaration const& declaration) override;

private:
    struct Data;

    cavi::usdj_am::Definition const& m_definition;
    std::unique_ptr<Data> m_data;
};

#endif  // REALITY_MERGE_USDJ_TRANSFORM_3D_EXTRACTOR_H

/**************************************************************************/
/* usdj_box_size_extractor.h                                              */
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

#ifndef REALITY_MERGE_USDJ_BOX_SIZE_EXTRACTOR_H
#define REALITY_MERGE_USDJ_BOX_SIZE_EXTRACTOR_H

#include <memory>
#include <optional>

// third-party
#include <cavi/usdj_am/visitor.hpp>

struct Vector3;

/// \brief An extractor of a box's size value embedded within a "USDA_Definition"
///        node.
class UsdjBoxSizeExtractor : public cavi::usdj_am::Visitor {
public:
    UsdjBoxSizeExtractor() = delete;

    UsdjBoxSizeExtractor(cavi::usdj_am::Definition const& p_definition);

    UsdjBoxSizeExtractor(UsdjBoxSizeExtractor const&) = delete;

    UsdjBoxSizeExtractor(UsdjBoxSizeExtractor&&) = default;

    ~UsdjBoxSizeExtractor();

    UsdjBoxSizeExtractor& operator=(UsdjBoxSizeExtractor const&) = delete;

    UsdjBoxSizeExtractor& operator=(UsdjBoxSizeExtractor&&) = default;

    std::optional<Vector3> operator()();

    void visit(cavi::usdj_am::Assignment const& assignment) override;

    void visit(cavi::usdj_am::Declaration const& declaration) override;

    void visit(cavi::usdj_am::Definition const& definition) override;

    void visit(cavi::usdj_am::DefinitionStatement const& definition_statement) override;

    void visit(cavi::usdj_am::Descriptor const& descriptor) override;

    void visit(cavi::usdj_am::ExternalReference const& external_reference) override;

    void visit(cavi::usdj_am::ReferenceFile const& reference_file) override;

private:
    cavi::usdj_am::Definition const& m_definition;
    std::optional<Vector3> m_size;
};

#endif  // REALITY_MERGE_USDJ_BOX_SIZE_EXTRACTOR_H

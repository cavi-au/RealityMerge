/**************************************************************************/
/* usdj_color_extractor.h                                                 */
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

#ifndef REALITY_MERGE_USDJ_COLOR_EXTRACTOR_H
#define REALITY_MERGE_USDJ_COLOR_EXTRACTOR_H

#include <cstdint>
#include <map>
#include <memory>

// third-party
#include <cavi/usdj_am/visitor.hpp>

struct Color;

/// \brief An extractor of a color value embedded within a "USDA_Definition"
///        node.
class UsdjColorExtractor : public cavi::usdj_am::Visitor {
public:
    UsdjColorExtractor() = delete;

    UsdjColorExtractor(cavi::usdj_am::Definition const& p_definition);

    UsdjColorExtractor(UsdjColorExtractor const&) = delete;

    UsdjColorExtractor(UsdjColorExtractor&&) = default;

    ~UsdjColorExtractor();

    UsdjColorExtractor& operator=(UsdjColorExtractor const&) = delete;

    UsdjColorExtractor& operator=(UsdjColorExtractor&&) = delete;

    std::optional<Color> operator()();

    void visit(cavi::usdj_am::Declaration const& declaration) override;

    void visit(cavi::usdj_am::Definition const& definition) override;

    void visit(cavi::usdj_am::DefinitionStatement const& definition_statement) override;

private:
    enum class Component : std::uint8_t { BEGIN__ = 1, R = BEGIN__, G, B, A, END__, SIZE__ = END__ - BEGIN__ };

    cavi::usdj_am::Definition const& m_definition;
    std::map<Component, float> m_components;
};

#endif  // REALITY_MERGE_USDJ_COLOR_EXTRACTOR_H

/**************************************************************************/
/* usdj_velocity_extractor.h                                              */
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

#ifndef REALITY_MERGE_USDJ_VELOCITY_EXTRACTOR_H
#define REALITY_MERGE_USDJ_VELOCITY_EXTRACTOR_H

#include <optional>

// third-party
#include <cavi/usdj_am/usd/physics/token_type.hpp>
#include <cavi/usdj_am/visitor.hpp>

struct Vector3;

/// \brief An extractor of a velocity value embedded within a "USDA_Definition"
///        node.
class UsdjVelocityExtractor : public cavi::usdj_am::Visitor {
public:
    UsdjVelocityExtractor() = delete;

    UsdjVelocityExtractor(cavi::usdj_am::Definition const& p_definition);

    UsdjVelocityExtractor(UsdjVelocityExtractor const&) = delete;

    UsdjVelocityExtractor(UsdjVelocityExtractor&&) = default;

    ~UsdjVelocityExtractor();

    UsdjVelocityExtractor& operator=(UsdjVelocityExtractor const&) = delete;

    UsdjVelocityExtractor& operator=(UsdjVelocityExtractor&&) = default;

    /// \throws std::invalid_argument
    std::optional<Vector3> operator()(cavi::usdj_am::usd::physics::TokenType const reference);

    void visit(cavi::usdj_am::Declaration const& declaration) override;

    void visit(cavi::usdj_am::Definition const& definition) override;

    void visit(cavi::usdj_am::DefinitionStatement const& definition_statement) override;

private:
    cavi::usdj_am::Definition const& m_definition;
    std::optional<cavi::usdj_am::usd::physics::TokenType> m_reference;
    std::optional<Vector3> m_velocity;
};

#endif  // REALITY_MERGE_USDJ_VELOCITY_EXTRACTOR_H

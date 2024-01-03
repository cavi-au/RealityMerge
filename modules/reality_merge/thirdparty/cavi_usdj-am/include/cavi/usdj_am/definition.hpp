/**************************************************************************/
/* definition.hpp                                                         */
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

#ifndef CAVI_USDJ_AM_DEFINITION_HPP
#define CAVI_USDJ_AM_DEFINITION_HPP

#include <optional>

// local
#include "array_range.hpp"
#include "definition_type.hpp"
#include "descriptor.hpp"
#include "node.hpp"
#include "statement_type.hpp"
#include "string_.hpp"

// export interface USDA_Definition {
//     type: USDA_StatementType.Definition;
//     subType: USDA_DefinitionType;
//     defType: USDA_Typename | null;
//     name: string;
//     descriptor: USDA_Descriptor | null;
//     statements: USDA_DefinitionStatement[];
// }

namespace cavi {
namespace usdj_am {

template <typename T>
class ArrayInputIterator;

struct DefinitionStatement;

/// \brief Represents a "USDA_Definition" node in a syntax tree that was parsed
///        out of a USDA document, encoded as JSON and stored within an
///        Automerge document.
class Definition : public Node {
public:
    using Statements = ArrayInputRange<DefinitionStatement>;

    /// \param document[in] A pointer to a borrowed Automerge document.
    /// \param map_object[in] A pointer to a borrowed Automerge map object.
    /// \pre \p document `!= nullptr`
    /// \pre \p map_object `!= nullptr`
    /// \pre `AMitemValType(` \p map_object `) == AM_VAL_TYPE_OBJ_TYPE`
    /// \pre `AMobjObjType(` \p document `, AMitemObjId(` \p map_object `)) == AM_OBJ_TYPE_MAP`
    /// \pre `AMobjSize(` \p document `, AMitemObjId(` \p map_object `)) == 6`
    /// \throws std::invalid_argument
    Definition(AMdoc const* const document, AMitem const* const map_object);

    Definition(Definition const&) = delete;

    Definition(Definition&&) = default;

    Definition& operator=(Definition const&) = delete;

    Definition& operator=(Definition&&) = default;

    /// \brief Accepts a visitor that can only read this node.
    ///
    /// \param[in] visitor A node visitor.
    void accept(Visitor& visitor) const& override;

    /// \brief Accepts a visitor that can take ownership of this node.
    ///
    /// \param[in] visitor A node visitor.
    void accept(Visitor& visitor) && override;

    /// \brief Gets the `.defType` property.
    ///
    /// \throws std::invalid_argument
    std::optional<Typename> get_def_type() const;

    /// \brief Gets the `.descriptor` property.
    ///
    /// \throws std::invalid_argument
    std::optional<Descriptor> get_descriptor() const;

    /// \brief Gets the `.name` property.
    ///
    /// \throws std::invalid_argument
    String get_name() const;

    /// \brief Gets the `.statements` property.
    ///
    /// \throws std::invalid_argument
    Statements get_statements() const;

    /// \brief Gets the `.subType` property.
    ///
    /// \throws std::invalid_argument
    DefinitionType get_sub_type() const;

    /// \brief Gets the `.type` property.
    constexpr StatementType get_type() const;

private:
    Definition();

    friend class ArrayInputIterator<Definition>;
};

constexpr StatementType Definition::get_type() const {
    return StatementType::DEFINITION;
}

}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_DEFINITION_HPP

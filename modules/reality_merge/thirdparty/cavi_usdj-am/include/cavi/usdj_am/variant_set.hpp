/**************************************************************************/
/* variant_set.hpp                                                        */
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

#ifndef CAVI_USDJ_AM_VARIANT_SET_HPP
#define CAVI_USDJ_AM_VARIANT_SET_HPP

#include <optional>

// local
#include "input_range.hpp"
#include "node.hpp"
#include "statement_type.hpp"
#include "string_.hpp"
#include "variant_definition.hpp"

namespace cavi {
namespace usdj_am {

// export interface USDA_VariantSet {
//     type: USDA_StatementType.VariantSet;
//     name: string;
//     definitions: USDA_VariantDefinition[];
// }

/// \brief Represents a "USDA_VariantSet" node in a syntax tree that was parsed
///        out of a USDA document, encoded as JSON and stored within an
///        Automerge document.
class VariantSet : public Node {
public:
    using VariantDefinitions = ConstInputRange<VariantDefinition>;

    VariantSet() = delete;

    /// \param document[in] A pointer to a borrowed Automerge document.
    /// \param map_object[in] A pointer to a borrowed Automerge map object.
    /// \pre \p document `!= nullptr`
    /// \pre \p map_object `!= nullptr`
    /// \pre `AMitemValType(` \p map_object `) == AM_VAL_TYPE_OBJ_TYPE`
    /// \pre `AMobjObjType(` \p document `, AMitemObjId(` \p map_object `)) == AM_OBJ_TYPE_MAP`
    /// \pre `AMobjSize(` \p document `, AMitemObjId(` \p map_object `)) == 3`
    /// \throws std::invalid_argument
    VariantSet(AMdoc const* const document, AMitem const* const map_object);

    VariantSet(VariantSet const&) = default;

    VariantSet& operator=(VariantSet const&) = delete;

    /// \brief Accepts a node visitor.
    ///
    /// \param[in] visitor A node visitor.
    void accept(Visitor& visitor) const;

    /// \brief Gets the `.name` property.
    ///
    /// \throws std::invalid_argument
    String get_name() const;

    /// \brief Gets the `.definitions` property.
    ///
    /// \throws std::invalid_argument
    VariantDefinitions get_definitions() const;

    /// \brief Gets the `.type` property.
    constexpr StatementType get_type() const;
};

constexpr StatementType VariantSet::get_type() const {
    return StatementType::VARIANT_SET;
}

}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_VARIANT_SET_HPP

/**************************************************************************/
/* object_declaration.hpp                                                 */
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

#ifndef CAVI_USDJ_AM_OBJECT_DECLARATION_HPP
#define CAVI_USDJ_AM_OBJECT_DECLARATION_HPP

#include <optional>

// local
#include "declaration_keyword.hpp"
#include "node.hpp"
#include "string_.hpp"
#include "value.hpp"

// export type USDA_Reference = string;

// export type USDA_TypeReference = string;

// export type USDA_ObjectDeclaration<T extends USDA_ValueTypes> = {
//     keyword: USDA_DeclarationKeyword | null;
//     defineType: USDA_TypeReference;
//     reference: USDA_Reference;
//     value: T;
// };

namespace cavi {
namespace usdj_am {

template <typename T>
class ArrayInputIterator;

using Reference = String;

using TypeReference = String;

/// \brief Represents a "USDA_ObjectDeclaration" node in a syntax tree that was
///        parsed out of a USDA document, encoded as JSON and stored within
///        an Automerge document.
class ObjectDeclaration : public Node {
public:
    /// \param document[in] A pointer to a borrowed Automerge document.
    /// \param map_object[in] A pointer to a borrowed Automerge map object.
    /// \pre \p document `!= nullptr`
    /// \pre \p map_object `!= nullptr`
    /// \pre `AMitemValType(` \p map_object `) == AM_VAL_TYPE_OBJ_TYPE`
    /// \pre `AMobjObjType(` \p document `, AMitemObjId(` \p map_object `)) == AM_OBJ_TYPE_MAP`
    /// \pre `AMobjSize(` \p document `, AMitemObjId(` \p map_object `)) == 4`
    /// \throws std::invalid_argument
    ObjectDeclaration(AMdoc const* const document, AMitem const* const map_object);

    ObjectDeclaration(ObjectDeclaration const&) = delete;

    ObjectDeclaration(ObjectDeclaration&&) = default;

    ObjectDeclaration& operator=(ObjectDeclaration const&) = delete;

    ObjectDeclaration& operator=(ObjectDeclaration&&) = default;

    /// \brief Accepts a visitor that can only read this node.
    ///
    /// \param[in] visitor A node visitor.
    void accept(Visitor& visitor) const& override;

    /// \brief Accepts a visitor that can take ownership of this node.
    ///
    /// \param[in] visitor A node visitor.
    void accept(Visitor& visitor) && override;

    /// \brief Gets the `.defineType` property.
    ///
    /// \throws std::invalid_argument
    TypeReference get_define_type() const;

    /// \brief Gets the `.keyword` property.
    ///
    /// \throws std::invalid_argument
    std::optional<DeclarationKeyword> get_keyword() const;

    /// \brief Gets the `.reference` property.
    ///
    /// \throws std::invalid_argument
    Reference get_reference() const;

    /// \brief Gets the `.value` property.
    ///
    /// \throws std::invalid_argument
    Value get_value() const;

private:
    ObjectDeclaration() = default;

    friend class ArrayInputIterator<ObjectDeclaration>;
};

}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_OBJECT_DECLARATION_HPP

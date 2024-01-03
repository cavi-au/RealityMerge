/**************************************************************************/
/* object_declarations.hpp                                                */
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

#ifndef CAVI_USDJ_AM_OBJECT_DECLARATIONS_HPP
#define CAVI_USDJ_AM_OBJECT_DECLARATIONS_HPP

#include <variant>

// local
#include "object_declaration_entries.hpp"
#include "object_declaration_list.hpp"

// type PotentialObjectDeclarationList<T extends USDA_ValueTypes, TArrayItem extends USDA_ValueTypes | any> =
//     TArrayItem extends USDA_ValueTypes ?
//         USDA_ObjectDeclarationList<TArrayItem> :
//         SimpleObjectDeclarations<T>;

// type SimpleObjectDeclarations<T extends USDA_ValueTypes> = USDA_ObjectDeclarationList<T>
//     | USDA_ObjectDeclarationEntries<T>;

// export type USDA_ObjectDeclarations<T extends USDA_ValueTypes> =
//     T extends (infer TArrayItem)[] ?
//         PotentialObjectDeclarationList<T, TArrayItem> :
//         SimpleObjectDeclarations<T>

struct AMdoc;
struct AMitem;

namespace cavi {
namespace usdj_am {

class Visitor;

/// \brief A struct representing a "USDA_ObjectDeclarations" node in a syntax
///        tree that was parsed out of a USDA document, encoded as JSON and
///        stored within an Automerge document.
struct ObjectDeclarations : public std::variant<std::monostate, ObjectDeclarationList, ObjectDeclarationEntries> {
    using std::variant<std::monostate, ObjectDeclarationList, ObjectDeclarationEntries>::variant;

    /// \param document[in] A pointer to a borrowed Automerge document.
    /// \param map_object[in] A pointer to a borrowed Automerge map object.
    /// \pre \p document `!= nullptr`
    /// \pre \p map_object `!= nullptr`
    /// \pre `AMitemValType(` \p map_object `) == AM_VAL_TYPE_OBJ_TYPE`
    /// \pre `AMobjObjType(` \p document `, AMitemObjId(` \p map_object `)) == AM_OBJ_TYPE_MAP`
    /// \throws std::invalid_argument
    ObjectDeclarations(AMdoc const* const document, AMitem const* const map_object);

    ObjectDeclarations(ObjectDeclarations const&) = delete;
    ObjectDeclarations& operator=(ObjectDeclarations const&) = delete;

    ObjectDeclarations(ObjectDeclarations&&) = default;
    ObjectDeclarations& operator=(ObjectDeclarations&&) = default;

    /// \note An inlined destructor can't delete incomplete types.
    ~ObjectDeclarations();

    /// \brief Accepts a visitor that can only read this node.
    ///
    /// \param[in] visitor A node visitor.
    void accept(Visitor& visitor) const&;

    /// \brief Accepts a visitor that can take ownership of this node.
    ///
    /// \param[in] visitor A node visitor.
    void accept(Visitor& visitor) &&;

private:
    ObjectDeclarations() = default;
};

}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_OBJECT_DECLARATIONS_HPP

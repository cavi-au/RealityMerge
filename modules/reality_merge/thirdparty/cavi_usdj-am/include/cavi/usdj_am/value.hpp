/**************************************************************************/
/* value.hpp                                                              */
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

#ifndef CAVI_USDJ_AM_VALUE_HPP
#define CAVI_USDJ_AM_VALUE_HPP

#include <memory>
#include <variant>

// local
#include "input_range.hpp"
#include "number.hpp"

struct AMdoc;
struct AMitem;

namespace cavi {
namespace usdj_am {

// export type USDA_ValueTypes =
//     | string
//     | boolean
//     | number
//     | USDA_Array<any>
//     | Readonly<USDA_Array<any>>
//     | USDA_ExternalReferenceImport
//     | USDA_ExternalReference
//     | USDA_ObjectValue<any>
//     | null
//     | undefined

// export type USDA_Array<T extends USDA_ValueTypes>
//     = T[]

class ExternalReference;
class ExternalReferenceImport;
struct ObjectValue;
struct Value;
class Visitor;

/// \brief A struct representing a "USDA_ValueTypes" node in a syntax tree that
///        was parsed out of a USDA document, encoded as JSON and stored within
///        an Automerge document.
struct Value : public std::variant<std::monostate,           // undefined
                                   std::unique_ptr<String>,  // string
                                   bool,                     // boolean
                                   Number,                   // number
                                   // USDA_Array<any>
                                   std::unique_ptr<ConstInputRange<Value> >,  // Readonly<USDA_Array<any>>
                                   std::unique_ptr<ExternalReferenceImport>,  // USDA_ExternalReferenceImport
                                   std::unique_ptr<ExternalReference>,        // USDA_ExternalReference
                                   std::unique_ptr<ObjectValue>,              // USDA_ObjectValue<any>
                                   std::nullptr_t                             // null
                                   > {
    Value() = delete;

    /// \param document[in] A pointer to a borrowed Automerge document.
    /// \param item[in] A pointer to a borrowed Automerge item.
    /// \pre \p document `!= nullptr`
    /// \pre \p item `!= nullptr`
    /// \pre `AMitemValType(` \p item `) ∈ {AM_VAL_TYPE_BOOL, AM_VAL_TYPE_F64, AM_VAL_TYPE_INT, AM_VAL_TYPE_NULL,
    ///                                     AM_VAL_TYPE_OBJ_TYPE, AM_VAL_TYPE_STR, AM_VAL_TYPE_UINT, AM_VAL_TYPE_VOID}`
    /// \throws std::invalid_argument
    Value(AMdoc const* const document, AMitem const* const item);

    /// \note `std::unique_ptr<T>` isn't copyable.
    Value(Value const&) = delete;
    Value& operator=(Value const&) = delete;

    /// \note `std::unique_ptr<T>` is movable.
    Value(Value&&) = default;
    Value& operator=(Value&&) = default;

    /// \note An inlined destructor can't delete incomplete types.
    ~Value();

    /// \brief Accepts a node visitor.
    ///
    /// \param[in] visitor A node visitor.
    void accept(Visitor& visitor) const;
};

}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_VALUE_HPP

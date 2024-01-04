/**************************************************************************/
/* array_iterator.cpp                                                     */
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

#include <sstream>
#include <stdexcept>
#include <typeinfo>

// third-party
extern "C" {

#include <automerge-c/automerge.h>
#include <automerge-c/utils/enum_string.h>
}

// local
#include "array_iterator.hpp"
#include "assignment.hpp"
#include "class_declaration.hpp"
#include "class_definition.hpp"
#include "definition.hpp"
#include "definition_statement.hpp"
#include "external_reference.hpp"
#include "object_declaration.hpp"
#include "object_declaration_list_value.hpp"
#include "object_value.hpp"
#include "statement.hpp"
#include "value.hpp"
#include "variant_definition.hpp"
#include "variant_set.hpp"

namespace cavi {
namespace usdj_am {

template <typename T>
ArrayInputIterator<T>::ArrayInputIterator() : m_document{nullptr} {}

template <typename T>
ArrayInputIterator<T>::~ArrayInputIterator() {}

template <typename T>
ArrayInputIterator<T>::ArrayInputIterator(AMdoc const* const document, AMitem const* const list_object)
    : m_document{document} {
    std::ostringstream args;
    if (!document) {
        args << "document == nullptr, ..., ...";
    } else if (!list_object) {
        args << "..., list_object == nullptr, ...";
    } else {
        AMvalType const val_type = AMitemValType(list_object);
        if (val_type != AM_VAL_TYPE_OBJ_TYPE) {
            args << "..., "
                 << "AMitemValType(list_object) == " << AMvalTypeToString(val_type) << ", ...";
        } else {
            AMobjType const obj_type = AMobjObjType(document, AMitemObjId(list_object));
            if (obj_type != AM_OBJ_TYPE_LIST) {
                args << "AMobjObjType(document, AMitemObjId(list_object)) == " << AMobjTypeToString(obj_type)
                     << ", ...";
            }
        }
    }
    if (!args.str().empty()) {
        std::ostringstream what;
        what << typeid(*this).name() << "::" << __func__ << "(" << args.str() << ")";
        throw std::invalid_argument(what.str());
    }
    // Preserve the AMitem storing the list object's ID.
    m_results[OBJ_ID] = ResultPtr{AMitemResult(list_object), AMresultFree};
    // Preserve the AMitems storing the list object's contents.
    m_results[ITEMS] = ResultPtr{AMobjItems(document, AMitemObjId(list_object), nullptr), AMresultFree};
    m_items = AMresultItems(m_results[ITEMS].get());
}

template <typename T>
ArrayInputIterator<T>& ArrayInputIterator<T>::operator++() {
    if (m_items) {
        AMitemsAdvance(&*m_items, 1);
    }
    return *this;
}

template <typename T>
ArrayInputIterator<T> ArrayInputIterator<T>::operator++(int) {
    ArrayInputIterator<T> current{*this};
    ++(*this);
    return current;
}

template <typename T>
T ArrayInputIterator<T>::operator*() {
    AMitem const* const item = (m_items) ? AMitemsNext(&*m_items, 0) : nullptr;
    if (item) {
        return T{m_document, item};
    } else {
        return T{};
    }
}

template <typename T>
bool operator==(ArrayInputIterator<T> const& lhs, ArrayInputIterator<T> const& rhs) {
    AMitem const* const lhs_item =
        (lhs.m_items) ? AMitemsNext(const_cast<AMitems*>(lhs.m_items.operator->()), 0) : nullptr;
    AMitem const* const rhs_item =
        (rhs.m_items) ? AMitemsNext(const_cast<AMitems*>(rhs.m_items.operator->()), 0) : nullptr;
    /// \note `AMitemEqual(nullptr, nullptr) == false`.
    return (lhs_item == rhs_item) || AMitemEqual(lhs_item, rhs_item);
}

template <typename T>
bool operator!=(ArrayInputIterator<T> const& lhs, ArrayInputIterator<T> const& rhs) {
    return !operator==(lhs, rhs);
}

// Assignment
template class ArrayInputIterator<Assignment>;

template bool operator==(ArrayInputIterator<Assignment> const& lhs, ArrayInputIterator<Assignment> const& rhs);

template bool operator!=(ArrayInputIterator<Assignment> const& lhs, ArrayInputIterator<Assignment> const& rhs);

// ClassDeclaration
template class ArrayInputIterator<ClassDeclaration>;

template bool operator==(ArrayInputIterator<ClassDeclaration> const& lhs,
                         ArrayInputIterator<ClassDeclaration> const& rhs);

template bool operator!=(ArrayInputIterator<ClassDeclaration> const& lhs,
                         ArrayInputIterator<ClassDeclaration> const& rhs);

// Definition
template class ArrayInputIterator<Definition>;

template bool operator==(ArrayInputIterator<Definition> const& lhs, ArrayInputIterator<Definition> const& rhs);

template bool operator!=(ArrayInputIterator<Definition> const& lhs, ArrayInputIterator<Definition> const& rhs);

// DefinitionStatement
template class ArrayInputIterator<DefinitionStatement>;

template bool operator==(ArrayInputIterator<DefinitionStatement> const& lhs,
                         ArrayInputIterator<DefinitionStatement> const& rhs);

template bool operator!=(ArrayInputIterator<DefinitionStatement> const& lhs,
                         ArrayInputIterator<DefinitionStatement> const& rhs);

// ObjectDeclaration
template class ArrayInputIterator<ObjectDeclaration>;

template bool operator==(ArrayInputIterator<ObjectDeclaration> const& lhs,
                         ArrayInputIterator<ObjectDeclaration> const& rhs);

template bool operator!=(ArrayInputIterator<ObjectDeclaration> const& lhs,
                         ArrayInputIterator<ObjectDeclaration> const& rhs);

// ObjectDeclarationListValue
template class ArrayInputIterator<ObjectDeclarationListValue>;

template bool operator==(ArrayInputIterator<ObjectDeclarationListValue> const& lhs,
                         ArrayInputIterator<ObjectDeclarationListValue> const& rhs);

template bool operator!=(ArrayInputIterator<ObjectDeclarationListValue> const& lhs,
                         ArrayInputIterator<ObjectDeclarationListValue> const& rhs);

// Statement
template class ArrayInputIterator<Statement>;

template bool operator==(ArrayInputIterator<Statement> const& lhs, ArrayInputIterator<Statement> const& rhs);

template bool operator!=(ArrayInputIterator<Statement> const& lhs, ArrayInputIterator<Statement> const& rhs);

// Value
template class ArrayInputIterator<Value>;

template bool operator==(ArrayInputIterator<Value> const& lhs, ArrayInputIterator<Value> const& rhs);

template bool operator!=(ArrayInputIterator<Value> const& lhs, ArrayInputIterator<Value> const& rhs);

// VariantDefinition
template class ArrayInputIterator<VariantDefinition>;

template bool operator==(ArrayInputIterator<VariantDefinition> const& lhs,
                         ArrayInputIterator<VariantDefinition> const& rhs);

template bool operator!=(ArrayInputIterator<VariantDefinition> const& lhs,
                         ArrayInputIterator<VariantDefinition> const& rhs);

}  // namespace usdj_am
}  // namespace cavi

/**************************************************************************/
/* array_range.cpp                                                        */
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

#include <algorithm>

// local
#include "array_range.hpp"
#include "assignment.hpp"
#include "class_declaration.hpp"
#include "definition.hpp"
#include "definition_statement.hpp"
#include "object_declaration.hpp"
#include "object_declaration_list_value.hpp"
#include "statement.hpp"
#include "value.hpp"
#include "variant_definition.hpp"

namespace cavi {
namespace usdj_am {

template <typename T>
bool operator==(ArrayInputRange<T> const& lhs, ArrayInputRange<T> const& rhs) {
    return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T>
bool operator!=(ArrayInputRange<T> const& lhs, ArrayInputRange<T> const& rhs) {
    return !operator==(lhs, rhs);
}

template <typename T>
ArrayInputRange<T>::ArrayInputRange(AMdoc const* const document, AMitem const* const list_object)
    : m_document{document}, m_result{AMitemResult(list_object), AMresultFree} {}

template <typename T>
ArrayInputIterator<T> ArrayInputRange<T>::begin() const {
    return ArrayInputIterator<T>{m_document, AMresultItem(m_result.get())};
}

template <typename T>
ArrayInputIterator<T> ArrayInputRange<T>::end() const {
    return ArrayInputIterator<T>{};
}

template <typename T>
std::size_t ArrayInputRange<T>::size() const {
    return AMobjSize(m_document, get_object_id(), nullptr);
}

template <typename T>
inline AMdoc const* ArrayInputRange<T>::get_document() const {
    return m_document;
}

template <typename T>
AMobjId const* ArrayInputRange<T>::get_object_id() const {
    return AMitemObjId(AMresultItem(m_result.get()));
}

template class ArrayInputRange<Assignment>;

template class ArrayInputRange<ClassDeclaration>;

template class ArrayInputRange<Definition>;

template class ArrayInputRange<DefinitionStatement>;

template class ArrayInputRange<ObjectDeclaration>;

template class ArrayInputRange<ObjectDeclarationListValue>;

template class ArrayInputRange<Statement>;

template class ArrayInputRange<Value>;

template class ArrayInputRange<VariantDefinition>;

}  // namespace usdj_am
}  // namespace cavi

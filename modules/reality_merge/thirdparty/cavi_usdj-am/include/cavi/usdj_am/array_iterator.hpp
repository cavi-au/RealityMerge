/**************************************************************************/
/* array_iterator.hpp                                                     */
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

#ifndef CAVI_USDJ_AM_ARRAY_ITERATOR_HPP
#define CAVI_USDJ_AM_ARRAY_ITERATOR_HPP

#include <array>
#include <iterator>
#include <memory>
#include <optional>

// third-party
extern "C" {

#include <automerge-c/automerge.h>
}

namespace cavi {
namespace usdj_am {

template <typename T>
class ArrayInputIterator;

template <typename T>
bool operator==(ArrayInputIterator<T> const& lhs, ArrayInputIterator<T> const& rhs);

template <typename T>
bool operator!=(ArrayInputIterator<T> const& lhs, ArrayInputIterator<T> const& rhs);

/// \brief Represents an array value in a syntax tree that was parsed out of a
///        USDA document, encoded as JSON and stored within an Automerge
///        document.
///
/// \tparam T The type of an element in the array.
template <typename T>
class ArrayInputIterator {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = T;
    using pointer = T*;
    using reference = T&;

    using ResultPtr = std::shared_ptr<AMresult>;

    ArrayInputIterator();

    /// \param document[in] A pointer to a borrowed Automerge document.
    /// \param list_object[in] A pointer to a borrowed Automerge list object.
    /// \pre \p document `!= nullptr`
    /// \pre \p list_object `!= nullptr`
    /// \pre `AMitemValType(` \p list_object `) == AM_VAL_TYPE_OBJ_TYPE`
    /// \pre `AMobjObjType(` \p document `, AMitemObjId(` \p list_object `)) == AM_OBJ_TYPE_LIST`
    /// \throws std::invalid_argument
    ArrayInputIterator(AMdoc const* const document, AMitem const* const list_object);

    ArrayInputIterator(ArrayInputIterator const&) = default;

    ~ArrayInputIterator();

    ArrayInputIterator& operator=(ArrayInputIterator const&) = delete;

    ArrayInputIterator& operator++();

    ArrayInputIterator operator++(int);

    T operator*();

private:
    enum Result { BEGIN__, ITEMS = BEGIN__, OBJ_ID, END__, SIZE__ = END__ - BEGIN__ };

    AMdoc const* const m_document;
    std::optional<AMitems> m_items;
    std::array<ResultPtr, SIZE__> m_results;

    friend bool operator==<T>(ArrayInputIterator<T> const& lhs, ArrayInputIterator<T> const& rhs);
};

}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_ARRAY_ITERATOR_HPP

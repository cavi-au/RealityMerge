/**************************************************************************/
/* input_range.hpp                                                        */
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

#ifndef CAVI_USDJ_AM_INPUT_RANGE_HPP
#define CAVI_USDJ_AM_INPUT_RANGE_HPP

#include <algorithm>

// local
#include "input_iterator.hpp"

namespace cavi {
namespace usdj_am {

///
/// \tparam T A type of value to be iterated over.
template <typename T>
class ConstInputRange {
public:
    using value_type = typename ConstInputIterator<T>::value_type;

    ConstInputRange() = delete;

    /// \param document[in] A pointer to a borrowed Automerge document.
    /// \param list_object[in] A pointer to a borrowed Automerge list object.
    /// \pre \p document `!= nullptr`
    /// \pre \p list_object `!= nullptr`
    /// \pre `AMitemValType(` \p list_object `) == AM_VAL_TYPE_OBJ_TYPE`
    /// \pre `AMobjObjType(` \p document `, AMitemObjId(` \p list_object `)) == AM_OBJ_TYPE_LIST`
    ConstInputRange(AMdoc const* const document, AMitem const* const list_object);

    ConstInputRange(ConstInputRange const&) = default;

    ConstInputRange& operator=(ConstInputRange const&) = default;

    /// \throws std::invalid_argument
    ConstInputIterator<T> begin() const;

    ConstInputIterator<T> end() const;

    std::size_t size() const;

    AMdoc const* get_document() const;

    AMobjId const* get_object_id() const;

private:
    AMdoc const* const m_document;
    typename ConstInputIterator<T>::ResultPtr m_result;
};

template <typename T>
inline bool operator==(ConstInputRange<T> const& lhs, ConstInputRange<T> const& rhs) {
    return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T>
inline bool operator!=(ConstInputRange<T> const& lhs, ConstInputRange<T> const& rhs) {
    return !operator==(lhs, rhs);
}

template <typename T>
ConstInputRange<T>::ConstInputRange(AMdoc const* const document, AMitem const* const list_object)
    : m_document{document}, m_result{AMitemResult(list_object), AMresultFree} {}

template <typename T>
ConstInputIterator<T> ConstInputRange<T>::begin() const {
    return ConstInputIterator<T>(m_document, AMresultItem(m_result.get()));
}

template <typename T>
ConstInputIterator<T> ConstInputRange<T>::end() const {
    return ConstInputIterator<T>();
}

template <typename T>
std::size_t ConstInputRange<T>::size() const {
    return AMobjSize(m_document, get_object_id(), nullptr);
}

template <typename T>
inline AMdoc const* ConstInputRange<T>::get_document() const {
    return m_document;
}

template <typename T>
AMobjId const* ConstInputRange<T>::get_object_id() const {
    return AMitemObjId(AMresultItem(m_result.get()));
}

}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_INPUT_RANGE_HPP

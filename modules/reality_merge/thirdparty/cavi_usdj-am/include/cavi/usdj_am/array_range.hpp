/**************************************************************************/
/* array_range.hpp                                                        */
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

#ifndef CAVI_USDJ_AM_ARRAY_RANGE_HPP
#define CAVI_USDJ_AM_ARRAY_RANGE_HPP

// local
#include "array_iterator.hpp"

struct AMobjId;

namespace cavi {
namespace usdj_am {

/// \brief Represents an "Array<any>" node property in a syntax tree that was
///        parsed out of a USDA document, encoded as JSON and stored within an
///        Automerge document.
///
/// \tparam T The type of value to be iterated over.
template <typename T>
class ArrayInputRange {
public:
    using value_type = typename ArrayInputIterator<T>::value_type;

    ArrayInputRange() = delete;

    /// \param document[in] A pointer to a borrowed Automerge document.
    /// \param list_object[in] A pointer to a borrowed Automerge list object.
    /// \pre \p document `!= nullptr`
    /// \pre \p list_object `!= nullptr`
    /// \pre `AMitemValType(` \p list_object `) == AM_VAL_TYPE_OBJ_TYPE`
    /// \pre `AMobjObjType(` \p document `, AMitemObjId(` \p list_object `)) == AM_OBJ_TYPE_LIST`
    ArrayInputRange(AMdoc const* const document, AMitem const* const list_object);

    ArrayInputRange(ArrayInputRange const&) = default;

    ArrayInputRange& operator=(ArrayInputRange const&) = default;

    /// \throws std::invalid_argument
    ArrayInputIterator<T> begin() const;

    ArrayInputIterator<T> end() const;

    std::size_t size() const;

    AMdoc const* get_document() const;

    AMobjId const* get_object_id() const;

private:
    AMdoc const* const m_document;
    typename ArrayInputIterator<T>::ResultPtr m_result;
};

}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_ARRAY_RANGE_HPP

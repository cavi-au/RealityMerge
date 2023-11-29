/**************************************************************************/
/* input_iterator.hpp                                                     */
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

#ifndef CAVI_USDJ_AM_INPUT_ITERATOR_HPP
#define CAVI_USDJ_AM_INPUT_ITERATOR_HPP

#include <cassert>
#include <functional>
#include <iterator>
#include <memory>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <typeinfo>

// third-party
extern "C" {

#include <automerge-c/automerge.h>
#include <automerge-c/utils/enum_string.h>
}

namespace cavi {
namespace usdj_am {

template <typename T>
class ConstInputIterator;

template <typename T>
bool operator==(ConstInputIterator<T> const& lhs, ConstInputIterator<T> const& rhs);

/// \brief Represents a read-only array value in a syntax tree that was parsed
///        out of a USDA document, encoded as JSON and stored within an
///        Automerge document.
///
/// \tparam T The type of the elements in the array.
template <typename T>
class ConstInputIterator {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = T;
    using pointer = T const*;
    using reference = T const&;

    using ResultPtr = std::shared_ptr<AMresult>;

    ConstInputIterator();

    /// \param document[in] A pointer to a borrowed Automerge document.
    /// \param list_object[in] A pointer to a borrowed Automerge list object.
    /// \pre \p document `!= nullptr`
    /// \pre \p list_object `!= nullptr`
    /// \pre `AMitemValType(` \p list_object `) == AM_VAL_TYPE_OBJ_TYPE`
    /// \pre `AMobjObjType(` \p document `, AMitemObjId(` \p list_object `)) == AM_OBJ_TYPE_LIST`
    /// \throws std::invalid_argument
    ConstInputIterator(AMdoc const* const document, AMitem const* const list_object);

    ConstInputIterator(ConstInputIterator const&) = default;

    ~ConstInputIterator() = default;

    ConstInputIterator& operator=(ConstInputIterator const&) = delete;

    ConstInputIterator& operator++();

    ConstInputIterator operator++(int);

    reference operator*();

private:
    enum Result { BEGIN__, ITEMS = BEGIN__, OBJ_ID, END__, SIZE__ = END__ - BEGIN__ };

    AMdoc const* const m_document;
    std::optional<AMitems> m_items;
    std::array<ResultPtr, SIZE__> m_results;
    mutable std::optional<T> m_value;

    static reference dummy();

    friend bool operator==<T>(ConstInputIterator<T> const& lhs, ConstInputIterator<T> const& rhs);
};

template <typename T>
ConstInputIterator<T>::ConstInputIterator() : m_document{nullptr} {}

template <typename T>
ConstInputIterator<T>::ConstInputIterator(AMdoc const* const document, AMitem const* const list_object)
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
ConstInputIterator<T>& ConstInputIterator<T>::operator++() {
    if (m_items) {
        AMitemsAdvance(&*m_items, 1);
    }
    return *this;
}

template <typename T>
ConstInputIterator<T> ConstInputIterator<T>::operator++(int) {
    ConstInputIterator<T> current{*this};
    ++(*this);
    return current;
}

template <typename T>
typename ConstInputIterator<T>::reference ConstInputIterator<T>::operator*() {
    AMitem const* const item = (m_items) ? AMitemsNext(&*m_items, 0) : nullptr;
    if (item) {
        m_value.emplace(m_document, item);
        return *m_value;
    } else {
        return dummy();
    }
}

template <typename T>
typename ConstInputIterator<T>::reference ConstInputIterator<T>::dummy() {
    static char const value[sizeof(T)] = {0};
    return *reinterpret_cast<T const*>(value);
}

template <typename T>
bool operator==(ConstInputIterator<T> const& lhs, ConstInputIterator<T> const& rhs) {
    AMitem const* const lhs_item =
        (lhs.m_items) ? AMitemsNext(const_cast<AMitems*>(lhs.m_items.operator->()), 0) : nullptr;
    AMitem const* const rhs_item =
        (rhs.m_items) ? AMitemsNext(const_cast<AMitems*>(rhs.m_items.operator->()), 0) : nullptr;
    /// \note `AMitemEqual(nullptr, nullptr) == false`.
    return (lhs_item == rhs_item) || AMitemEqual(lhs_item, rhs_item);
}

template <typename T>
inline bool operator!=(ConstInputIterator<T> const& lhs, ConstInputIterator<T> const& rhs) {
    return !operator==(lhs, rhs);
}

}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_INPUT_ITERATOR_HPP

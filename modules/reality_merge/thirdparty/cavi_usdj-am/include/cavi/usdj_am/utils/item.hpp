/**************************************************************************/
/* item.hpp                                                               */
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

#ifndef CAVI_USDJ_AM_UTILS_ITEM_HPP
#define CAVI_USDJ_AM_UTILS_ITEM_HPP

#include <cstdint>
#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

struct AMdoc;
struct AMitem;
struct AMresult;

namespace cavi {
namespace usdj_am {
namespace utils {

/// \brief Represents an item within an Automerge document.
class Item {
public:
    Item() = delete;

    /// \brief Wraps the root map object of the given Automerge document.
    ///
    /// \param document[in] A pointer to a borrowed `AMdoc` struct.
    /// \pre \p document `!= nullptr`
    /// \throws std::invalid_argument
    Item(AMdoc const* const document);

    Item(Item const&) = default;

    Item(Item&&) = default;

    Item& operator=(Item const&) = delete;

    Item& operator=(Item&&) = delete;

    operator AMitem const*() const;

private:
    using ResultPtr = std::shared_ptr<AMresult>;
    using Results = std::vector<ResultPtr>;

    AMdoc const* const m_document;
    Results m_results;

    friend bool operator==(Item const& lhs, Item const& rhs);

    friend Item operator/(Item const& lhs, Item const& rhs);

    friend Item operator/(Item const&, std::string const&);

    friend Item operator/(Item const&, std::uint64_t const);

    friend std::ostream& operator<<(std::ostream& os, Item const& in);
};

bool operator==(Item const& lhs, Item const& rhs);

inline bool operator!=(Item const& lhs, Item const& rhs) {
    return !operator==(lhs, rhs);
}

/// \brief Appends another item to the given item.
///
/// \param lhs[in] An `Item` to which \p rhs will be appended.
/// \param rhs[in] An `Item` to append.
/// \returns A new `Item`.
/// \throws std::invalid_argument
Item operator/(Item const& lhs, Item const& rhs);

/// \brief Appends a string index to the given item.
///
/// \param lhs[in] An `Item` to which index \p key will be appended.
/// \param key[in] A key within a map object.
/// \returns A new `Item`.
/// \throws std::invalid_argument
Item operator/(Item const& lhs, std::string const& key);

/// \brief Appends an integer index to the given item.
///
/// \param lhs[in] An `Item` to which index \p pos will be appended.
/// \param pos[in] A position within a list object.
/// \returns A new `Item`.
/// \throws std::invalid_argument
Item operator/(Item const& lhs, std::uint64_t const pos);

std::ostream& operator<<(std::ostream& os, Item const& in);

}  // namespace utils
}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_UTILS_ITEM_HPP

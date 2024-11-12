/**************************************************************************/
/* document.hpp                                                           */
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

#ifndef CAVI_USDJ_AM_UTILS_DOCUMENT_HPP
#define CAVI_USDJ_AM_UTILS_DOCUMENT_HPP

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>

// local
#include "item.hpp"

struct AMdoc;
struct AMitem;
struct AMresult;

namespace cavi {
namespace usdj_am {
namespace utils {

/// \brief Represents an Automerge document.
class Document {
public:
    using ResultPtr = std::unique_ptr<AMresult, void (*)(AMresult*)>;

    /// \brief Load an Automerge document from a memory buffer.
    ///
    /// \param[in] src A pointer to an array of bytes.
    /// \param[in] count The number of bytes to read.
    /// \pre \p src `!= nullptr`
    /// \pre \p count `<= sizeof(*` \p src `)`
    /// \throws std::invalid_argument
    static Document load(std::uint8_t const* const src, std::size_t const count);

    /// \brief Load an Automerge document from a binary file.
    ///
    /// \param[in] filename A path to a binary file.
    /// \throws std::invalid_argument
    static Document load(std::filesystem::path const& filename);

    Document() = delete;

    /// \param[in] result A managed pointer to an `AMresult` struct.
    /// \pre `(` \p result `).get() != nullptr`
    /// \pre `AMitemValType(AMresultItem((` \p result `).get())) == AM_VAL_TYPE_DOC`
    /// \post `(` \p result `).get() == nullptr`
    /// \throws std::invalid_argument
    Document(ResultPtr&& result);

    Document(Document const&) = delete;
    Document& operator=(Document const&) = delete;

    Document(Document&&) = default;
    Document& operator=(Document&&) = delete;

    operator AMdoc*() const;

    /// \brief Gets the root map object of the document.
    ///
    /// \returns An `Item`.
    Item get_item() const;

    /// \brief Gets the item at an absolute POSIX path within the document.
    ///
    /// \param[in] posix_path An absolute POSIX path string.
    /// \returns An `Item`.
    /// \throws std::invalid_argument
    Item get_item(std::string const& posix_path) const;

    /// \brief Saves a compact representation of the Automerge document to a binary file.
    ///
    /// \param[in] filename A path to a binary file.
    /// \returns The number of bytes that were written.
    /// \throws std::invalid_argument
    /// \throws std::runtime_error
    std::size_t save(std::filesystem::path const& filename) const;

private:
    AMdoc* m_document;
    ResultPtr m_result;

    friend bool operator==(Document const& lhs, Document const& rhs);
};

inline Document::operator AMdoc*() const {
    return m_document;
}

inline Item Document::get_item() const {
    return Item(m_document);
}

bool operator==(Document const& lhs, Document const& rhs);

inline bool operator!=(Document const& lhs, Document const& rhs) {
    return !operator==(lhs, rhs);
}

}  // namespace utils
}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_UTILS_DOCUMENT_HPP

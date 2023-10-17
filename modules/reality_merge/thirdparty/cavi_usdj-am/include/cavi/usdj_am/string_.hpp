/**************************************************************************/
/* string_.hpp                                                             */
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

#ifndef CAVI_USDJ_AM_STRING_HPP
#define CAVI_USDJ_AM_STRING_HPP

#include <array>
#include <iosfwd>
#include <memory>
#include <string_view>

struct AMdoc;
struct AMitem;
struct AMobjId;
struct AMresult;

namespace cavi {
namespace usdj_am {

class String {
public:
    String();

    /// \param document[in] A pointer to a borrowed Automerge document.
    /// \param item[in] A pointer to a borrowed Automerge str or text object.
    /// \pre \p document `!= nullptr`
    /// \pre \p item `!= nullptr`
    /// \pre `AMitemValType(` \p item `) == AM_VAL_TYPE_OBJ_TYPE || AMitemValType(` \p item `) == AM_VAL_TYPE_STR`
    /// \throws std::invalid_argument
    String(AMdoc const* const document, AMitem const* const item);

    String(String const&) = default;

    String& operator=(String const&) = delete;

    String& operator=(String&&) = delete;

    String& operator=(String const&&) = delete;

    AMdoc const* get_document() const;

    AMobjId const* get_object_id() const;

    std::string_view get_view() const;

private:
    enum Result { BEGIN_, ITEM = BEGIN_, OBJ_ID, END_, SIZE_ = END_ - BEGIN_ };

    using ResultPtr = std::shared_ptr<AMresult>;

    AMdoc const* const m_document;
    std::array<ResultPtr, SIZE_> m_results;
};

inline String::String() : m_document{nullptr}, m_results{} {}

inline AMdoc const* String::get_document() const {
    return m_document;
}

std::ostream& operator<<(std::ostream& os, String const& in);

}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_STRING_HPP

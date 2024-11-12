/**************************************************************************/
/* number.hpp                                                             */
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

#ifndef CAVI_USDJ_AM_NUMBER_HPP
#define CAVI_USDJ_AM_NUMBER_HPP

#include <cstdint>
#include <iosfwd>
#include <variant>

struct AMdoc;
struct AMitem;

namespace cavi {
namespace usdj_am {

class Visitor;

/// \brief A struct representing a TypeScript `number` value which preserves the
///        distinct type of an Automerge `AM_VAL_TYPE_F64`, `AM_VAL_TYPE_INT` or
///        `AM_VAL_TYPE_UINT` value.
struct Number : public std::variant<double,        // AM_VAL_TYPE_F64
                                    std::int64_t,  // AM_VAL_TYPE_INT
                                    std::uint64_t  // AM_VAL_TYPE_UINT
                                    > {
    Number() = default;

    /// \param document[in] A pointer to a borrowed Automerge document.
    /// \param item[in] A pointer to a borrowed Automerge item.
    /// \pre \p document `!= nullptr`
    /// \pre \p item `!= nullptr`
    /// \pre `AMitemValType(` \p item `) ∈ {AM_VAL_TYPE_F64, AM_VAL_TYPE_INT, AM_VAL_TYPE_UINT}`
    /// \throws std::invalid_argument
    Number(AMdoc const* const document, AMitem const* const item);

    Number(Number const&) = delete;
    Number& operator=(Number const&) = delete;

    Number(Number&&) = default;
    Number& operator=(Number&&) = delete;

    /// \brief Accepts a visitor that can only read this node.
    ///
    /// \param[in] visitor A node visitor.
    void accept(Visitor& visitor) const&;

    /// \brief Accepts a visitor that can take ownership of this node.
    ///
    /// \param[in] visitor A node visitor.
    void accept(Visitor& visitor) &&;
};

std::ostream& operator<<(std::ostream& os, Number const& in);

}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_NUMBER_HPP

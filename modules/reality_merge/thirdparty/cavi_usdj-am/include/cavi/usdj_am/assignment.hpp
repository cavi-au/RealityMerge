/**************************************************************************/
/* assignment.hpp                                                         */
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

#ifndef CAVI_USDJ_AM_ASSIGNMENT_HPP
#define CAVI_USDJ_AM_ASSIGNMENT_HPP

#include <cstdint>
#include <iosfwd>
#include <optional>

// local
#include "assignment_keyword.hpp"
#include "node.hpp"
#include "value.hpp"

namespace cavi {
namespace usdj_am {

// export enum AssignmentType {
//     Assignment = 'assignment',
// }

/// \brief An enum representing an "AssignmentType" tag stored as a string
///        within an Automerge document.
enum class AssignmentType : std::uint8_t {
    BEGIN_ = 1,
    ASSIGNMENT = BEGIN_,
    END_,
    SIZE_ = END_ - BEGIN_,
};

// export interface USDA_Assignment<T extends USDA_ValueTypes> {
//     type: AssignmentType.Assignment;
//     keyword: null | USDA_AssignmentKeyword;
//     identifier: string;
//     value: T;
// }

class String;
class Visitor;

/// \brief Represents a "USDA_Assignment" node in a syntax tree that was parsed
///        out of a USDA document, encoded as JSON and stored within an
///        Automerge document.
class Assignment : public Node {
public:
    Assignment() = delete;

    /// \param document[in] A pointer to a borrowed Automerge document.
    /// \param map_object[in] A pointer to a borrowed Automerge map object.
    /// \pre \p document `!= nullptr`
    /// \pre \p map_object `!= nullptr`
    /// \pre `AMitemValType(` \p map_object `) == AM_VAL_TYPE_OBJ_TYPE`
    /// \pre `AMobjObjType(` \p document `, AMitemObjId(` \p map_object `)) == AM_OBJ_TYPE_MAP`
    /// \pre `AMobjSize(` \p document `, AMitemObjId(` \p map_object `)) == 4`
    /// \throws std::invalid_argument
    Assignment(AMdoc const* const document, AMitem const* const map_object);

    Assignment(Assignment const&) = delete;

    Assignment& operator=(Assignment const&) = delete;

    /// \brief Accepts a node visitor.
    ///
    /// \param[in] visitor A node visitor.
    void accept(Visitor& visitor) const;

    /// \brief Gets the `.identifier` property.
    ///
    /// \throws std::invalid_argument
    String get_identifier() const;

    /// \brief Gets the `.keyword` property.
    ///
    /// \throws std::invalid_argument
    std::optional<AssignmentKeyword> get_keyword() const;

    /// \brief Gets the `.type` property.
    constexpr AssignmentType get_type() const;

    /// \brief Gets the `.value` property.
    ///
    /// \throws std::invalid_argument
    Value get_value() const;
};

constexpr AssignmentType Assignment::get_type() const {
    return AssignmentType::ASSIGNMENT;
}

std::istream& operator>>(std::istream& is, AssignmentType& out);

std::ostream& operator<<(std::ostream& os, AssignmentType const& in);

}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_ASSIGNMENT_HPP

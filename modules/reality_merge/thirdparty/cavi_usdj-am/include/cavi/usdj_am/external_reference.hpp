/**************************************************************************/
/* external_reference.hpp                                                 */
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

#ifndef CAVI_USDJ_AM_EXTERNAL_REFERENCE_HPP
#define CAVI_USDJ_AM_EXTERNAL_REFERENCE_HPP

#include <optional>

// local
#include "external_reference_import.hpp"
#include "node.hpp"
#include "reference_file.hpp"
#include "value_type.hpp"

namespace cavi {
namespace usdj_am {

// export interface USDA_ExternalReference {
//     type: USDA_ValueType.ExternalReference;
//     referenceFile: USDA_ReferenceFile;
//     toImport: null | USDA_ExternalReferenceImport;
// }

class ReferenceFile;

/// \brief Represents a "USDA_ExternalReference" node in a syntax tree that was
///        parsed out of a USDA document, encoded as JSON and stored within an
///        Automerge document.
class ExternalReference : public Node {
public:
    ExternalReference() = delete;

    /// \param document[in] A pointer to a borrowed Automerge document.
    /// \param map_object[in] A pointer to a borrowed Automerge map object.
    /// \pre \p document `!= nullptr`
    /// \pre \p map_object `!= nullptr`
    /// \pre `AMobjObjType(` \p document `, AMitemObjId(` \p map_object `)) == AM_OBJ_TYPE_MAP`
    /// \pre `AMobjSize(` \p document `, AMitemObjId(` \p map_object `)) == 3`
    /// \throws std::invalid_argument
    ExternalReference(AMdoc const* const document, AMitem const* const map_object);

    ExternalReference(ExternalReference const&) = default;

    ExternalReference& operator=(ExternalReference const&) = delete;

    /// \brief Accepts a node visitor.
    ///
    /// \param[in] visitor A node visitor.
    void accept(Visitor& visitor) const;

    /// \brief Gets the `.referenceFile` property.
    ///
    /// \throws std::invalid_argument
    ReferenceFile get_reference_file() const;

    /// \brief Gets the `.toImport` property.
    ///
    /// \throws std::invalid_argument
    std::optional<ExternalReferenceImport> get_to_import() const;

    /// \brief Gets the `.type` property.
    constexpr ValueType get_type() const;
};

constexpr ValueType ExternalReference::get_type() const {
    return ValueType::EXTERNAL_REFERENCE;
}

}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_EXTERNAL_REFERENCE_HPP

/**************************************************************************/
/* external_reference_import.hpp                                          */
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

#ifndef CAVI_USDJ_AM_EXTERNAL_REFERENCE_IMPORT_HPP
#define CAVI_USDJ_AM_EXTERNAL_REFERENCE_IMPORT_HPP

#include <optional>

// local
#include "node.hpp"
#include "string_.hpp"
#include "value_type.hpp"

namespace cavi {
namespace usdj_am {

// export interface USDA_ExternalReferenceImport {
//     type: USDA_ValueType.ExternalReferenceImport;
//     importPath: string;
//     field: string | null;
// }

/// \brief Represents a "USDA_ExternalReferenceImport" node in a syntax tree
///        that was parsed out of a USDA document, encoded as JSON and stored
///        within an Automerge document.
class ExternalReferenceImport : public Node {
public:
    ExternalReferenceImport() = delete;

    /// \param document[in] A pointer to a borrowed Automerge document.
    /// \param map_object[in] A pointer to a borrowed Automerge map object.
    /// \pre \p document `!= nullptr`
    /// \pre \p map_object `!= nullptr`
    /// \pre `AMobjObjType(` \p document `, AMitemObjId(` \p map_object `)) == AM_OBJ_TYPE_MAP`
    /// \pre `AMobjSize(` \p document `, AMitemObjId(` \p map_object `)) == 3`
    /// \throws std::invalid_argument
    ExternalReferenceImport(AMdoc const* const document, AMitem const* const map_object);

    ExternalReferenceImport(ExternalReferenceImport const&) = delete;

    ExternalReferenceImport(ExternalReferenceImport&&) = default;

    ExternalReferenceImport& operator=(ExternalReferenceImport const&) = delete;

    /// \brief Accepts a node visitor.
    ///
    /// \param[in] visitor A node visitor.
    void accept(Visitor& visitor) const;

    /// \brief Gets the `.field` property.
    std::optional<String> get_field() const;

    /// \brief Gets the `.importPath` property.
    String get_import_path() const;

    /// \brief Gets the `.type` property.
    constexpr ValueType get_type() const;
};

constexpr ValueType ExternalReferenceImport::get_type() const {
    return ValueType::EXTERNAL_REFERENCE_IMPORT;
}

}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_EXTERNAL_REFERENCE_IMPORT_HPP

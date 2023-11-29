/**************************************************************************/
/* usd/token_type.hpp                                                     */
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

#ifndef CAVI_USDJ_AM_USD_TOKEN_TYPE_HPP
#define CAVI_USDJ_AM_USD_TOKEN_TYPE_HPP

#include <cstdint>
#include <iosfwd>
#include <optional>
#include <set>
#include <string_view>

// boost::python::class_<UsdTokensType, boost::noncopyable>
//     cls("Tokens", boost::python::no_init);
// _AddToken(cls, "apiSchemas", UsdTokens->apiSchemas);
// _AddToken(cls, "clips", UsdTokens->clips);
// _AddToken(cls, "clipSets", UsdTokens->clipSets);
// _AddToken(cls, "collection", UsdTokens->collection);
// _AddToken(cls, "collection_MultipleApplyTemplate_", UsdTokens->collection_MultipleApplyTemplate_);
// _AddToken(cls, "collection_MultipleApplyTemplate_Excludes", UsdTokens->collection_MultipleApplyTemplate_Excludes);
// _AddToken(cls, "collection_MultipleApplyTemplate_ExpansionRule",
// UsdTokens->collection_MultipleApplyTemplate_ExpansionRule); _AddToken(cls,
// "collection_MultipleApplyTemplate_IncludeRoot", UsdTokens->collection_MultipleApplyTemplate_IncludeRoot);
// _AddToken(cls, "collection_MultipleApplyTemplate_Includes", UsdTokens->collection_MultipleApplyTemplate_Includes);
// _AddToken(cls, "exclude", UsdTokens->exclude);
// _AddToken(cls, "expandPrims", UsdTokens->expandPrims);
// _AddToken(cls, "expandPrimsAndProperties", UsdTokens->expandPrimsAndProperties);
// _AddToken(cls, "explicitOnly", UsdTokens->explicitOnly);
// _AddToken(cls, "fallbackPrimTypes", UsdTokens->fallbackPrimTypes);
// _AddToken(cls, "APISchemaBase", UsdTokens->APISchemaBase);
// _AddToken(cls, "ClipsAPI", UsdTokens->ClipsAPI);
// _AddToken(cls, "CollectionAPI", UsdTokens->CollectionAPI);
// _AddToken(cls, "ModelAPI", UsdTokens->ModelAPI);
// _AddToken(cls, "Typed", UsdTokens->Typed);

namespace cavi {
namespace usdj_am {

struct Value;

namespace usd {

/// \brief An enum representing a `UsdTokensType` tag stored as a string within
///        a "USDA_Declaration" node in a syntax tree that was parsed out of a
///        USDA document, encoded as JSON and stored within an Automerge
///        document.
enum class TokenType : std::uint8_t {
    BEGIN__ = 1,
    API_SCHEMAS = BEGIN__,
    CLIPS,
    CLIP_SETS,
    COLLECTION,
    COLLECTION_MULTIPLE_APPLY_TEMPLATE_,
    COLLECTION_MULTIPLE_APPLY_TEMPLATE_EXCLUDES,
    COLLECTION_MULTIPLE_APPLY_TEMPLATE_EXPANSION_RULE,
    COLLECTION_MULTIPLE_APPLY_TEMPLATE_INCLUDE_ROOT,
    COLLECTION_MULTIPLE_APPLY_TEMPLATE_INCLUDES,
    EXCLUDE,
    EXPAND_PRIMS,
    EXPAND_PRIMS_AND_PROPERTIES,
    EXPLICIT_ONLY,
    FALLBACK_PRIM_TYPES,
    API_SCHEMA_BASE,
    CLIPS_API,
    COLLECTION_API,
    MODEL_API,
    TYPED,
    END__,
    SIZE__ = END__ - BEGIN__
};

/// \brief Extracts a `TokenType` enum tag from a string view.
///
/// \param[in] view A UTF-8 string view.
/// \returns The `TokenType` enum tag whose serialized form is \p view or
///          `std::nullopt`.
std::optional<TokenType> extract_TokenType(std::string_view const& view);

/// \brief Converts a string view into a `TokenType` enum tag.
///
/// \param[in] view A UTF-8 string view.
/// \returns The `TokenType` enum tag whose serialized form is \p view.
/// \throws std::invalid_argument
TokenType to_TokenType(std::string_view const& view);

using TokenTypeSet = std::set<TokenType>;

/// \brief Extracts a `TokenTypeSet` from a `Value`.
///
/// \param[in] value A `Value` in which to search for a `TokenTypeSet`.
/// \returns A `TokenTypeSet` that may be empty.
TokenTypeSet extract_TokenTypeSet(Value const& value);

std::istream& operator>>(std::istream& is, TokenType& out);

std::ostream& operator<<(std::ostream& os, TokenType const& in);

}  // namespace usd
}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_USD_TOKEN_TYPE_HPP

/**************************************************************************/
/* usd/token_type.cpp                                                     */
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

#include <map>
#include <ostream>
#include <sstream>
#include <stdexcept>

// local
#include "detail/enum_string.hpp"
#include "usd/token_type.hpp"
#include "value.hpp"

namespace {

using cavi::usdj_am::usd::TokenType;

static std::map<std::string_view, TokenType> const TAGS = {
    {"apiSchemas", TokenType::API_SCHEMAS},
    {"clips", TokenType::CLIPS},
    {"clipSets", TokenType::CLIP_SETS},
    {"collection", TokenType::COLLECTION},
    {"collection_MultipleApplyTemplate_", TokenType::COLLECTION_MULTIPLE_APPLY_TEMPLATE_},
    {"collection_MultipleApplyTemplate_Excludes", TokenType::COLLECTION_MULTIPLE_APPLY_TEMPLATE_EXCLUDES},
    {"collection_MultipleApplyTemplate_ExpansionRule", TokenType::COLLECTION_MULTIPLE_APPLY_TEMPLATE_EXPANSION_RULE},
    {"collection_MultipleApplyTemplate_IncludeRoot", TokenType::COLLECTION_MULTIPLE_APPLY_TEMPLATE_INCLUDE_ROOT},
    {"collection_MultipleApplyTemplate_Includes", TokenType::COLLECTION_MULTIPLE_APPLY_TEMPLATE_INCLUDES},
    {"exclude", TokenType::EXCLUDE},
    {"expandPrims", TokenType::EXPAND_PRIMS},
    {"expandPrimsAndProperties", TokenType::EXPAND_PRIMS_AND_PROPERTIES},
    {"explicitOnly", TokenType::EXPLICIT_ONLY},
    {"fallbackPrimTypes", TokenType::FALLBACK_PRIM_TYPES},
    {"APISchemaBase", TokenType::API_SCHEMA_BASE},
    {"ClipsAPI", TokenType::CLIPS_API},
    {"CollectionAPI", TokenType::COLLECTION_API},
    {"ModelAPI", TokenType::MODEL_API},
    {"Typed", TokenType::TYPED},
};

}  // namespace

namespace cavi {
namespace usdj_am {
namespace usd {

std::optional<TokenType> extract_TokenType(std::string_view const& view) {
    return detail::extract_enum_tag<TokenType>(TAGS, view);
}

TokenType to_TokenType(std::string_view const& view) {
    auto const result = detail::extract_enum_tag<TokenType>(TAGS, view);
    if (!result) {
        std::ostringstream what;
        what << __func__ << "(view == \"" << view << "\")";
        throw std::invalid_argument(what.str());
    }
    return *result;
}

TokenTypeSet extract_TokenTypeSet(Value const& value) {
    return detail::extract_enum_tag_set<TokenType>(TAGS, value);
}

/// \todo Also parse tokens containing substitutions for "__INSTANCE_NAME__".
std::istream& operator>>(std::istream& is, TokenType& out) {
    return detail::operator>><TokenType>(is, std::make_pair(std::cref(TAGS), std::ref(out)));
}

std::ostream& operator<<(std::ostream& os, TokenType const& in) {
    os << detail::extract_enum_string<TokenType>(TAGS, in).value_or("???");
    return os;
}

}  // namespace usd
}  // namespace usdj_am
}  // namespace cavi

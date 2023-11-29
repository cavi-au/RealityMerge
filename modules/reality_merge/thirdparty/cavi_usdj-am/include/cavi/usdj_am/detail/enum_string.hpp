/**************************************************************************/
/* detail/enum_string.hpp                                                 */
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

#ifndef CAVI_USDJ_AM_DETAIL_ENUM_STRING_HPP
#define CAVI_USDJ_AM_DETAIL_ENUM_STRING_HPP

#include <functional>
#include <istream>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// local
#include "value.hpp"

namespace cavi {
namespace usdj_am {
namespace detail {

/// \brief Extracts a sequence of enum tags corresponding to the UTF-8 string
///        views found within a `Value`.
///
/// \tparam EnumT The type of enum value to append.
/// \tparam SequenceT The type of sequence container to return.
/// \param[in] tags A map from UTF-8 string views to \p EnumT tags.
/// \param[in] value A `Value` in which to search for the serialized forms of
///                  \p EnumT tags.
/// \returns A \p SequenceT of \p EnumT values that may be empty.
template <class EnumT, class SequenceT = std::vector<EnumT>>
SequenceT extract_enum_tag_sequence(std::map<std::string_view, EnumT> const& tags, Value const& value) {
    SequenceT tag_sequence{};
    auto const values_ptr = std::get_if<std::unique_ptr<ConstValues>>(&value);
    if (values_ptr) {
        for (auto const& sub_value : **values_ptr) {
            auto const string_ptr = std::get_if<std::unique_ptr<String>>(&sub_value);
            if (string_ptr) {
                auto const tag = extract_enum_tag(tags, **string_ptr);
                if (tag)
                    tag_sequence.push_back(*tag);
            }
        }
    }
    return tag_sequence;
}

/// \brief Extracts a set of enum tags corresponding to the UTF-8 string views
///        found within a `Value`.
///
/// \tparam EnumT The type of enum value to insert.
/// \tparam SetT The type of set container to return.
/// \param[in] tags A map from UTF-8 string views to \p EnumT tags.
/// \param[in] value A `Value` in which to search for the serialized forms of
///                  \p EnumT tags.
/// \returns A \p SetT of \p EnumT values that may be empty.
template <class EnumT, class SetT = std::set<EnumT>>
SetT extract_enum_tag_set(std::map<std::string_view, EnumT> const& tags, Value const& value) {
    SetT tag_set;
    auto const values_ptr = std::get_if<std::unique_ptr<ConstValues>>(&value);
    if (values_ptr) {
        for (auto const& sub_value : **values_ptr) {
            auto const string_ptr = std::get_if<std::unique_ptr<String>>(&sub_value);
            if (string_ptr) {
                auto const tag = extract_enum_tag(tags, **string_ptr);
                if (tag)
                    tag_set.insert(*tag);
            }
        }
    }
    return tag_set;
}

/// \brief Find the UTF-8 string view corresponding to a given enum tag.
///
/// \tparam EnumT The type of \p tag.
/// \param[in] tags A map from UTF-8 string views to \p EnumT tags.
/// \param[in] tag A \p EnumT tag.
/// \returns The UTF-8 string view corresponding to \p tag or `std::nullopt`.
template <class EnumT>
std::optional<std::string_view> extract_enum_string(std::map<std::string_view, EnumT> const& tags, EnumT const tag) {
    for (auto item : tags) {
        if (item.second == tag) {
            return item.first;
        }
    }
    return std::nullopt;
}

/// \brief Find the enum tag corresponding to a given UTF-8 string view.
///
/// \tparam EnumT The type of enum value to return.
/// \param[in] tags A map from UTF-8 string views to \p EnumT tags.
/// \param[in] view A UTF-8 string view.
/// \returns The \p EnumT tag corresponding to \p view or `std::nullopt`.
template <class EnumT>
std::optional<EnumT> extract_enum_tag(std::map<std::string_view, EnumT> const& tags, std::string_view const& view) {
    auto const match = tags.find(view);
    if (match != tags.end()) {
        return match->second;
    }
    return std::nullopt;
}

template <class EnumT>
std::istream& operator>>(std::istream& is,
                         std::pair<std::reference_wrapper<std::map<std::string_view, EnumT> const>,
                                   std::reference_wrapper<EnumT>> const args) {
    std::string source;
    if (is >> source) {
        auto const result = extract_enum_tag<EnumT>(args.first, source);
        if (result) {
            args.second.get() = *result;
            return is;
        }
    }
    is.setstate(std::ios::failbit);
    return is;
}

}  // namespace detail
}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_DETAIL_ENUM_STRING_HPP

/**************************************************************************/
/* node.hpp                                                               */
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

#ifndef CAVI_USDJ_AM_NODE_HPP
#define CAVI_USDJ_AM_NODE_HPP

#include <cassert>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <strstream>

// third-party
extern "C" {

#include <automerge-c/automerge.h>
}

// local
#include "string_.hpp"
#include "utils/bytes.hpp"
#include "value.hpp"

namespace cavi {
namespace usdj_am {

class Visitor;

// export type USDA_Typename =
//     | string
using Typename = String;

/// \brief An abstract base class representing a node in a syntax tree that was
///        parsed out of a USDA document, encoded as JSON and stored within an
///        Automerge document.
class Node {
public:
    Node() = delete;

    virtual ~Node();

    Node(Node const&) = delete;

    Node& operator=(Node const&) = delete;

    /// \brief Accepts a node visitor.
    ///
    /// \param[in] visitor A node reader.
    virtual void accept(Visitor& visitor) const = 0;

    AMdoc const* get_document() const;

    virtual AMobjId const* get_object_id() const;

protected:
    using ResultPtr = std::shared_ptr<AMresult>;

    /// \param document[in] A pointer to a borrowed Automerge document.
    /// \pre \p document `!= nullptr`
    Node(AMdoc const* const document);

    /// \param document[in] A pointer to a borrowed Automerge document.
    /// \param map_object[in] A pointer to a borrowed Automerge map object.
    /// \param map_size[in] The expected count of items in \p map_object.
    /// \pre \p document `!= nullptr`
    /// \pre \p map_object `!= nullptr`
    /// \pre \p map_size `> 0`
    /// \pre `AMitemValType(` \p map_object `) == AM_VAL_TYPE_OBJ_TYPE`
    /// \pre `AMobjObjType(` \p document `, AMitemObjId(` \p map_object `)) == AM_OBJ_TYPE_MAP`
    /// \pre `AMobjSize(` \p document `, AMitemObjId(` \p map_object `), nullptr) ==` \p map_size
    Node(AMdoc const* const document, AMitem const* const map_object, int const map_size);

    Node(Node&&) = default;

    Node& operator=(Node&&) = default;

    /// \brief Checks that the enum property under a given key matches the given
    ///        tag value.
    ///
    /// \tparam EnumT A type of enum.
    /// \param key[in] A pointer to a null-terminated byte string.
    /// \throws std::invalid_argument
    template <typename EnumT>
    void check_enum_property(std::string const& key, EnumT const tag) const;

    /// \brief Checks that the string property under a given key matches the
    ///        given string value.
    ///
    /// \param key[in] A pointer to a null-terminated byte string.
    /// \param value[in] A pointer to a null-terminated byte string.
    /// \throws std::invalid_argument
    void check_string_property(std::string const& key, std::string const& value) const;

    /// \brief Gets the string property under a given key.
    ///
    /// \tparam InputRangeT A type satisfying the `std::ranges::input_range`
    ///         concept.
    /// \param key[in] A pointer to a null-terminated byte string.
    template <typename InputRangeT>
    InputRangeT get_array_property(std::string const& key) const;

    /// \brief Gets the enum property under a given key.
    ///
    /// \tparam EnumT A type of enum.
    /// \param key[in] A pointer to a null-terminated byte string.
    /// \throws std::invalid_argument
    template <typename EnumT>
    EnumT get_enum_property(std::string const& key) const;

    /// \brief Gets the nullable enum property under a given key.
    ///
    /// \tparam EnumT A type of enum.
    /// \param key[in] A pointer to a null-terminated byte string.
    /// \throws std::invalid_argument
    template <typename EnumT>
    std::optional<EnumT> get_nullable_enum_property(std::string const& key) const;

    /// \brief Gets the nullable object property under a given key.
    ///
    /// \tparam ObjectT A type of object.
    /// \param key[in] A pointer to a null-terminated byte string.
    /// \throws std::invalid_argument
    template <typename ObjectT>
    std::optional<ObjectT> get_nullable_object_property(std::string const& key) const;

    /// \brief Gets the object property under a given key.
    ///
    /// \tparam ObjectT A type of object.
    /// \param key[in] A pointer to a null-terminated byte string.
    /// \throws std::invalid_argument
    template <typename ObjectT>
    ObjectT get_object_property(std::string const& key) const;

    AMdoc const* const m_document;
    /// \note It is assumed that all key values are static strings passed from
    ///       within subclass member functions.
    mutable std::map<std::string, ResultPtr> m_results;
};

template <typename EnumT>
void Node::check_enum_property(std::string const& key, EnumT const tag) const {
    std::ostringstream arguments;
    try {
        if (get_enum_property<EnumT>(key) != tag) {
            arguments << "AMmapGet(m_document, ..., AMstr(\"" << key << "\"), nullptr) == \""
                      << get_object_property<String>(key) << "\", \"" << tag << "\"";
        }
    } catch (std::invalid_argument const& thrown) {
        arguments << thrown.what();
    }
    // Free the AMresult because we're finished with it.
    m_results.erase(std::string{key});
    if (!arguments.str().empty()) {
        std::ostringstream what;
        what << typeid(*this).name() << "::" << __func__ << "(" << arguments.str() << ")";
        throw std::invalid_argument(what.str());
    }
}

inline AMdoc const* Node::get_document() const {
    return m_document;
}

template <typename InputRangeT>
InputRangeT Node::get_array_property(std::string const& key) const {
    ResultPtr const result{AMmapGet(m_document, get_object_id(), utils::to_bytes(key), nullptr), AMresultFree};
    auto [iter, inserted] = m_results.insert_or_assign(key, result);
    return InputRangeT{m_document, AMresultItem(iter->second.get())};
}

template <typename EnumT>
EnumT Node::get_enum_property(std::string const& key) const {
    ResultPtr const result{AMmapGet(m_document, get_object_id(), utils::to_bytes(key), nullptr), AMresultFree};
    auto [iter, inserted] = m_results.insert_or_assign(key, result);
    std::ostringstream arguments;
    try {
        String string{m_document, AMresultItem(iter->second.get())};
        std::string buffer{string.get_view()};
        std::istrstream is(buffer.data(), buffer.size());
        EnumT tag;
        is >> tag;
        if (!is.fail()) {
            return tag;
        } else {
            arguments << "AMmapGet(m_document, ..., AMstr(\"" << key << "\"), nullptr) == \"" << string << "\"";
        }
    } catch (std::invalid_argument const& thrown) {
        arguments << thrown.what();
    }
    if (!arguments.str().empty()) {
        std::ostringstream what;
        what << typeid(*this).name() << "::" << __func__ << "(" << arguments.str() << ")";
        throw std::invalid_argument(what.str());
    }
    return EnumT{};
}

template <typename EnumT>
std::optional<EnumT> Node::get_nullable_enum_property(std::string const& key) const {
    std::optional<EnumT> nullable_enum;
    try {
        nullable_enum.emplace(get_enum_property<EnumT>(key));
    } catch (std::invalid_argument const&) {
        if (AMitemValType(AMresultItem(m_results.at(key).get())) != AM_VAL_TYPE_NULL) {
            throw;
        }
    }
    return nullable_enum;
}

template <typename ObjectT>
std::optional<ObjectT> Node::get_nullable_object_property(std::string const& key) const {
    std::optional<ObjectT> nullable_object;
    try {
        nullable_object.emplace(get_object_property<ObjectT>(key));
    } catch (std::invalid_argument const&) {
        if (AMitemValType(AMresultItem(m_results.at(key).get())) != AM_VAL_TYPE_NULL) {
            throw;
        }
    }
    return nullable_object;
}

template <typename ObjectT>
ObjectT Node::get_object_property(std::string const& key) const {
    ResultPtr const result{AMmapGet(m_document, get_object_id(), utils::to_bytes(key), nullptr), AMresultFree};
    auto [iter, inserted] = m_results.insert_or_assign(key, result);
    try {
        return ObjectT{m_document, AMresultItem(iter->second.get())};
    } catch (std::invalid_argument const& thrown) {
        std::ostringstream what;
        what << typeid(*this).name() << "::" << __func__ << "(" << thrown.what() << ")";
        throw std::invalid_argument(what.str());
    }
}

extern template std::optional<String> Node::get_nullable_object_property<String>(std::string const&) const;

extern template String Node::get_object_property<String>(std::string const&) const;

extern template Value Node::get_object_property<Value>(std::string const&) const;

}  // namespace usdj_am
}  // namespace cavi

// extern template std::optional<cavi::usdj_am::String>
// cavi::usdj_am::Node::get_nullable_object_property<cavi::usdj_am::String>(std::string const&) const;

// extern template cavi::usdj_am::String cavi::usdj_am::Node::get_object_property<cavi::usdj_am::String>(
//     std::string const&) const;

// extern template cavi::usdj_am::Value cavi::usdj_am::Node::get_object_property<cavi::usdj_am::Value>(
//     std::string const&) const;

#endif  // CAVI_USDJ_AM_NODE_HPP

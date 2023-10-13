/**************************************************************************/
/* node.cpp                                                               */
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

#include <cstddef>
#include <stdexcept>
#include <string_view>
#include <typeinfo>

// third-party
extern "C" {

#include <automerge-c/utils/enum_string.h>
}

// local
#include "node.hpp"
#include "string_.hpp"

namespace cavi {
namespace usdj_am {

Node::Node(AMdoc const* const document) : m_document{document} {
    std::ostringstream arguments;
    if (!document) {
        arguments << "document == nullptr";
    }
    if (!arguments.str().empty()) {
        std::ostringstream what;
        what << typeid(*this).name() << "::" << __func__ << "(" << arguments.str() << ")";
        throw std::invalid_argument(what.str());
    }
}

Node::Node(AMdoc const* const document, AMitem const* const map_object, int const map_size) : m_document{document} {
    std::ostringstream arguments;
    if (!document) {
        arguments << "document == nullptr, ..., ...";
    } else if (!map_object) {
        arguments << "..., map_object == nullptr, ...";
    } else if (!(map_size > 0)) {
        arguments << "..., ..., map_size == " << map_size;
    } else {
        AMvalType const val_type = AMitemValType(map_object);
        if (val_type != AM_VAL_TYPE_OBJ_TYPE) {
            arguments << "..., "
                      << "AMitemValType(map_object) == " << AMvalTypeToString(val_type) << ", ...";
        } else {
            AMobjId const* const obj_id = AMitemObjId(map_object);
            AMobjType const obj_type = AMobjObjType(document, obj_id);
            if (obj_type != AM_OBJ_TYPE_MAP) {
                arguments << "AMobjObjType(document, AMitemObjId(map_object)) == " << AMobjTypeToString(obj_type)
                          << ", ...";
            } else {
                std::size_t const obj_size = AMobjSize(document, obj_id, nullptr);
                if (obj_size != map_size) {
                    arguments << "AMobjSize(document, AMitemObjId(map_object), nullptr) == " << obj_size << ", "
                              << map_size;
                }
            }
        }
    }
    if (!arguments.str().empty()) {
        std::ostringstream what;
        what << typeid(*this).name() << "::" << __func__ << "(" << arguments.str() << ")";
        throw std::invalid_argument(what.str());
    }
    // Preserve the AMitem storing the node's object ID.
    ResultPtr result{AMitemResult(map_object), AMresultFree};
    m_results.insert_or_assign("", result);
}

Node::~Node() {}

void Node::check_string_property(std::string const& key, std::string const& value) const {
    std::ostringstream arguments;
    try {
        auto const property = get_object_property<String>(key);
        if (property.get_view() != std::string(value)) {
            arguments << "AMmapGet(m_document, get_object_id, AMstr(\"" << key << "\"), nullptr) == \"" << property
                      << "\", \"" << value << "\"";
        }
    } catch (std::invalid_argument const& thrown) {
        arguments << thrown.what();
    }
    // Free the AMresult because we're finished with it.
    m_results.erase(key);
    if (!arguments.str().empty()) {
        std::ostringstream what;
        what << typeid(*this).name() << "::" << __func__ << "(" << arguments.str() << ")";
        throw std::invalid_argument(what.str());
    }
}

AMobjId const* Node::get_object_id() const {
    return AMitemObjId(AMresultItem(m_results.at("").get()));
}

template std::optional<String> Node::get_nullable_object_property<String>(std::string const&) const;

template String Node::get_object_property<String>(std::string const&) const;

template Value Node::get_object_property<Value>(std::string const&) const;

}  // namespace usdj_am
}  // namespace cavi

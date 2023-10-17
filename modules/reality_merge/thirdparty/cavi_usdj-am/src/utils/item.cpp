/**************************************************************************/
/* item.cpp                                                               */
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

#include <optional>
#include <ostream>
#include <sstream>
#include <stdexcept>

#include <typeinfo>

// third-party
extern "C" {

#include <automerge-c/automerge.h>
#include <automerge-c/utils/enum_string.h>
}

// local
#include "utils/bytes.hpp"
#include "utils/item.hpp"

namespace cavi {
namespace usdj_am {
namespace utils {

Item::Item(AMdoc const* const document) : m_document{document} {
    if (!document) {
        std::ostringstream what;
        what << typeid(*this).name() << "::" << __func__ << "(document == nullptr)";
        throw std::invalid_argument(what.str());
    }
}

Item::operator AMitem const*() const {
    return (!m_results.empty()) ? AMresultItem(m_results.back().get()) : nullptr;
}

bool operator==(Item const& lhs, Item const& rhs) {
    if (lhs.m_document != rhs.m_document) {
        return false;
    }
    /// \note `AMitemEqual(nullptr, nullptr) == false`.
    AMitem const* const lhs_item = lhs;
    AMitem const* const rhs_item = rhs;
    return (lhs_item == rhs_item) || AMitemEqual(lhs_item, rhs_item);
}

Item& operator/(Item& item, std::string const& key) {
    std::ostringstream arguments;
    auto const* const obj_id = AMitemObjId(item);
    auto const val_type = (obj_id) ? AMitemValType(item) : AM_VAL_TYPE_OBJ_TYPE;
    if (val_type != AM_VAL_TYPE_OBJ_TYPE) {
        arguments << "AMitemValType(" << item << ") == " << AMvalTypeToString(val_type) << ", ...";
    } else {
        auto const obj_type = AMobjObjType(item.m_document, obj_id);
        if (obj_type != AM_OBJ_TYPE_MAP) {
            arguments << "AMobjObjType(..., AMitemObjId(" << item << ")) == " << AMobjTypeToString(obj_type) << ", ...";
        } else {
            if (arguments.str().empty()) {
                Item::ResultPtr result{AMmapGet(item.m_document, obj_id, to_bytes(key), nullptr), AMresultFree};
                if (AMresultStatus(result.get()) != AM_STATUS_OK) {
                    arguments << "AMresultError(AMmapGet(..., AMitemObjId(" << item << "), \"" << key
                              << "\", nullptr)) == \"" << from_bytes(AMresultError(result.get())) << "\"";
                } else {
                    item.m_results.emplace_back(result);
                }
            }
        }
    }
    if (!arguments.str().empty()) {
        std::ostringstream what;
        what << __func__ << "(" << arguments.str() << ")";
        throw std::invalid_argument(what.str());
    }
    return item;
}

Item& operator/(Item& item, std::uint64_t const pos) {
    std::ostringstream arguments;
    AMobjId const* const obj_id = AMitemObjId(item);
    if (!obj_id) {
        if (!item.m_results.empty()) {
            // It's a value so the pos is inapplicable.
            arguments << "AMitemValType(" << item << ") == " << AMvalTypeToString(AMitemValType(item)) << ", ...";
        } else {
            // The document's root object is a map so the pos is inapplicable.
            arguments << "AMobjObjType(..., AM_ROOT) == " << AMobjTypeToString(AM_OBJ_TYPE_MAP) << ", ...";
        }
    } else {
        AMobjType const obj_type = AMobjObjType(item.m_document, obj_id);
        if (obj_type != AM_OBJ_TYPE_LIST) {
            arguments << "AMobjObjType(..., AMitemObjId(" << item << ")) == " << AMobjTypeToString(obj_type) << ", ...";
        }
    }
    if (arguments.str().empty()) {
        Item::ResultPtr result{AMlistGet(item.m_document, obj_id, pos, nullptr), AMresultFree};
        if (AMresultStatus(item.m_results.back().get()) != AM_STATUS_OK) {
            arguments << "AMresultError(AMlistGet(..., AMitemObjId(" << item << "), " << pos << ", nullptr)) == \""
                      << from_bytes(AMresultError(result.get())) << "\"";
        } else {
            item.m_results.emplace_back(result);
        }
    }
    if (!arguments.str().empty()) {
        std::ostringstream what;
        what << __func__ << "(" << arguments.str() << ")";
        throw std::invalid_argument(what.str());
    }
    return item;
}

std::ostream& operator<<(std::ostream& os, Item const& in) {
    AMbyteSpan key;
    std::uint64_t pos;
    // The root of a document is a map object.
    AMvalType val_type = AM_VAL_TYPE_OBJ_TYPE;
    auto const fmtflags = os.flags();
    os << typeid(in).name() << "{" << std::showbase << std::hex << in.m_document << "}";
    os.setf(fmtflags);
    for (auto result : in.m_results) {
        os << "/";
        AMitem const* const c_item = AMresultItem(result.get());
        if (AMitemKey(c_item, &key)) {
            os << "\"" << from_bytes(key) << "\"";
        } else if (AMitemPos(c_item, &pos)) {
            os << pos;
        }
        val_type = AMitemValType(c_item);
    }
    if (val_type == AM_VAL_TYPE_OBJ_TYPE) {
        // Append a slash in order to indicate that it terminates on an object.
        os << "/";
    }
    return os;
}

}  // namespace utils
}  // namespace usdj_am
}  // namespace cavi

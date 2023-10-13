/**************************************************************************/
/* string_.cpp                                                             */
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

#include <cassert>
#include <sstream>
#include <stdexcept>
#include <typeinfo>

// third-party
extern "C" {

#include <automerge-c/automerge.h>
#include <automerge-c/utils/enum_string.h>
}

// local
#include "string_.hpp"

namespace std {

ostream& operator<<(ostream& os, cavi::usdj_am::String const& in) {
    os << in.get_view();
    return os;
}

}  // namespace std

namespace cavi {
namespace usdj_am {

String::String(AMdoc const* const document, AMitem const* const item) : m_document{document} {
    std::ostringstream arguments;
    if (!document) {
        arguments << "document == nullptr, ...";
    } else if (!item) {
        arguments << "..., item == nullptr";
    } else {
        AMvalType const val_type = AMitemValType(item);
        switch (val_type) {
            case AM_VAL_TYPE_OBJ_TYPE: {
                AMobjId const* const obj_id = AMitemObjId(item);
                AMobjType const obj_type = AMobjObjType(document, obj_id);
                if (obj_type == AM_OBJ_TYPE_TEXT) {
                    // Preserve the AMitem storing the text object's ID.
                    m_results[OBJ_ID] = ResultPtr{AMitemResult(item), AMresultFree};
                    // Preserve the AMitem storing the text object's contents.
                    m_results[ITEM] = ResultPtr{AMtext(m_document, obj_id, nullptr), AMresultFree};
                } else {
                    arguments << "AMobjObjType(document, AMitemObjId(item)) == " << AMobjTypeToString(obj_type);
                }
                break;
            }
            case AM_VAL_TYPE_STR: {
                // Preserve the AMitem storing the string.
                m_results[ITEM] = ResultPtr{AMitemResult(item), AMresultFree};
                break;
            }
            default: {
                arguments << "..., AMitemValType(item) == " << AMvalTypeToString(val_type);
                break;
            }
        }
    }
    if (!arguments.str().empty()) {
        std::ostringstream what;
        what << typeid(*this).name() << "::" << __func__ << "(" << arguments.str() << ")";
        throw std::invalid_argument(what.str());
    }
}

AMobjId const* String::get_object_id() const {
    return AMitemObjId(AMresultItem(m_results[OBJ_ID].get()));
}

std::string_view String::get_view() const {
    AMbyteSpan str = {0};
    assert(AMitemToStr(AMresultItem(m_results[ITEM].get()), &str) == true);
    return std::string_view{reinterpret_cast<std::string_view::const_pointer>(str.src), str.count};
}

}  // namespace usdj_am
}  // namespace cavi

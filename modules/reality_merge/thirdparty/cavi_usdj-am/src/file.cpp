/**************************************************************************/
/* file.cpp                                                               */
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

// third-party
extern "C" {

#include <automerge-c/automerge.h>
}

// local
#include "descriptor.hpp"
#include "file.hpp"
#include "statement.hpp"
#include "visitor.hpp"

namespace cavi {
namespace usdj_am {

File::File(AMdoc const* const document, AMitem const* const map_object) : Node(document) {
    static const std::size_t MAP_SIZE = 3;

    std::ostringstream args;
    if (!document) {
        args << "document == nullptr, ...";
    } else {
        AMobjId const* obj_id = AM_ROOT;
        if (map_object) {
            AMvalType const val_type = AMitemValType(map_object);
            if (val_type != AM_VAL_TYPE_OBJ_TYPE) {
                args << "..., "
                     << "AMitemValType(map_object) == " << AMvalTypeToString(val_type);
            } else {
                obj_id = AMitemObjId(map_object);
                AMobjType const obj_type = AMobjObjType(document, obj_id);
                if (obj_type != AM_OBJ_TYPE_MAP) {
                    args << "AMobjObjType(document, AMitemObjId(map_object)) == " << AMobjTypeToString(obj_type);
                }
            }
        }
        if (args.str().empty()) {
            std::size_t const obj_size = AMobjSize(document, obj_id, nullptr);
            if (obj_size != MAP_SIZE) {
                args << "AMobjSize(document, AMitemObjId(map_object), nullptr) == " << obj_size << ", " << MAP_SIZE;
            } else if (map_object) {
                // Preserve the AMitem storing the node's object ID.
                ResultPtr const result{AMitemResult(map_object), AMresultFree};
                m_results.insert_or_assign("", result);
            }
        }
    }
    if (!args.str().empty()) {
        std::ostringstream what;
        what << typeid(*this).name() << "::" << __func__ << "(" << args.str() << ")";
        throw std::invalid_argument(what.str());
    }
}

void File::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

AMobjId const* File::get_object_id() const {
    if (m_results.count("")) {
        return Node::get_object_id();
    } else {
        // The map object is the document itself.
        return AM_ROOT;
    }
}

Number File::get_version() const {
    return get_object_property<Number>("version");
}

std::optional<Descriptor> File::get_descriptor() const {
    return get_nullable_object_property<Descriptor>("descriptor");
}

File::Statements File::get_statements() const {
    return get_array_property<Statements>("statements");
}

}  // namespace usdj_am
}  // namespace cavi

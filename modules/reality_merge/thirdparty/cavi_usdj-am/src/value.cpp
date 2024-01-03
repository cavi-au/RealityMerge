/**************************************************************************/
/* value.cpp                                                              */
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

#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <typeinfo>

// third-party
extern "C" {

#include <automerge-c/automerge.h>
#include <automerge-c/utils/enum_string.h>
}

// local
#include "external_reference.hpp"
#include "external_reference_import.hpp"
#include "object_value.hpp"
#include "value.hpp"
#include "value_type.hpp"
#include "visitor.hpp"

namespace cavi {
namespace usdj_am {

Value::Value(AMdoc const* const document, AMitem const* const item) {
    std::ostringstream args;
    try {
        AMvalType const val_type = AMitemValType(item);
        switch (val_type) {
            case AM_VAL_TYPE_BOOL: {
                bool bool_;
                if (AMitemToBool(item, &bool_)) {
                    this->emplace<bool>(bool_);
                } else {
                    args << "..., AMitemToBool(item, ...) == " << std::boolalpha << false;
                }
                break;
            }
            case AM_VAL_TYPE_F64:
            case AM_VAL_TYPE_INT:
            case AM_VAL_TYPE_UINT: {
                this->emplace<Number>(document, item);
                break;
            }
            case AM_VAL_TYPE_NULL: {
                this->emplace<std::nullptr_t>(nullptr);
                break;
            }
            case AM_VAL_TYPE_OBJ_TYPE: {
                AMobjType const obj_type = AMobjObjType(document, AMitemObjId(item));
                switch (obj_type) {
                    case AM_OBJ_TYPE_LIST: {
                        this->emplace<ValueRange>(document, item);
                        break;
                    }
                    case AM_OBJ_TYPE_MAP: {
                        using Index = typename std::underlying_type<ValueType>::type;

                        for (Index index = static_cast<Index>(ValueType::BEGIN__);
                             index != static_cast<Index>(ValueType::END__); ++index) {
                            try {
                                switch (static_cast<ValueType>(index)) {
                                    case ValueType::EXTERNAL_REFERENCE: {
                                        this->emplace<ExternalReference>(document, item);
                                        break;
                                    }
                                    case ValueType::EXTERNAL_REFERENCE_IMPORT: {
                                        this->emplace<ExternalReferenceImport>(document, item);
                                        break;
                                    }
                                    case ValueType::OBJECT_VALUE: {
                                        this->emplace<ObjectValue>(document, item);
                                        break;
                                    }
                                    default:
                                        continue;
                                }
                                args.str("");
                                break;
                            } catch (std::invalid_argument const& thrown) {
                                if (!args.str().empty()) {
                                    args << " | ";
                                }
                                args << thrown.what();
                            }
                        }
                        break;
                    }
                    case AM_OBJ_TYPE_TEXT: {
                        this->emplace<String>(document, item);
                        break;
                    }
                }
                break;
            }
            case AM_VAL_TYPE_STR: {
                this->emplace<String>(document, item);
                break;
            }
            case AM_VAL_TYPE_VOID: {
                /// \note `std::monostate` is the default alternative.
                break;
            }
            default: {
                args << "..., AMitemValType(item) == " << AMvalTypeToString(val_type);
                break;
            }
        }
    } catch (std::invalid_argument const& thrown) {
        args << thrown.what();
    }
    if (!args.str().empty()) {
        std::ostringstream what;
        what << typeid(*this).name() << "::" << __func__ << "(" << args.str() << ")";
        throw std::invalid_argument(what.str());
    }
}

Value::~Value() {}

void Value::accept(Visitor& visitor) const& {
    visitor.visit(*this);
}

void Value::accept(Visitor& visitor) && {
    visitor.visit(std::forward<Value>(*this));
}

}  // namespace usdj_am
}  // namespace cavi

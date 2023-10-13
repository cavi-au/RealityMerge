/**************************************************************************/
/* number.cpp                                                             */
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
#include "number.hpp"

namespace cavi {
namespace usdj_am {

Number::Number(AMdoc const* const document, AMitem const* const item) {
    std::ostringstream arguments;
    AMvalType const val_type = AMitemValType(item);
    switch (val_type) {
        case AM_VAL_TYPE_F64: {
            double f64;
            if (AMitemToF64(item, &f64)) {
                this->emplace<double>(f64);
            } else {
                arguments << "..., AMitemToF64(item, ...) == " << std::boolalpha << false;
            }
            break;
        }
        case AM_VAL_TYPE_INT: {
            std::int64_t int_;
            if (AMitemToInt(item, &int_)) {
                this->emplace<std::int64_t>(int_);
            } else {
                arguments << "..., AMitemToInt(item, ...) == " << std::boolalpha << false;
            }
            break;
        }
        case AM_VAL_TYPE_UINT: {
            std::uint64_t uint;
            if (AMitemToUint(item, &uint)) {
                this->emplace<std::uint64_t>(uint);
            } else {
                arguments << "..., AMitemToUint(item, ...) == " << std::boolalpha << false;
            }
            break;
        }
        default: {
            arguments << "..., AMitemValType(item) == " << AMvalTypeToString(val_type);
            break;
        }
    }
    if (!arguments.str().empty()) {
        std::ostringstream what;
        what << typeid(*this).name() << "::" << __func__ << "(" << arguments.str() << ")";
        throw std::invalid_argument(what.str());
    }
}

std::ostream& operator<<(std::ostream& os, Number const& in) {
    std::visit([&os](auto const& alt) { os << alt; }, in);
    return os;
}

}  // namespace usdj_am
}  // namespace cavi

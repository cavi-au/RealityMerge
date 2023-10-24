/**************************************************************************/
/* exception_callback.cpp                                                 */
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

#include <memory>
#include <sstream>
#include <stdexcept>
#include <typeinfo>

// third-party
extern "C" {

#include <automerge-c/automerge.h>
#include <automerge-c/utils/enum_string.h>
#include <automerge-c/utils/stack.h>
#include <automerge-c/utils/stack_callback_data.h>
#include <automerge-c/utils/string.h>
}

// local
#include "exception_callback.h"

using CharPtr = std::unique_ptr<char, void (*)(void*)>;
using DataPtr = std::unique_ptr<AMstackCallbackData, void (*)(void*)>;

bool exc_cb(AMstack** p_stack, void* p_data) {
    DataPtr const data_ptr{static_cast<AMstackCallbackData*>(p_data), std::free};
    std::ostringstream msg;
    if (!p_stack) {
        msg << '`' << typeid(p_stack).name();
    } else if (!*p_stack) {
        msg << '`' << typeid(*p_stack).name();
    } else if (!(*p_stack)->result) {
        msg << '`' << typeid((*p_stack)->result).name();
    }
    if (!msg.str().empty()) {
        msg << " == nullptr`.";
        throw std::invalid_argument(msg.str());
        return false;
    }
    AMstatus const status = AMresultStatus((*p_stack)->result);
    switch (status) {
        case AM_STATUS_ERROR: {
            msg << "Error";
            break;
        }
        case AM_STATUS_INVALID_RESULT: {
            msg << "Invalid result";
            break;
        }
        case AM_STATUS_OK:
            break;
        default: {
            msg << "Unknown `" << typeid(status).name() << "` tag " << status;
        }
    }
    if (!msg.str().empty()) {
        CharPtr const c_msg{AMstrdup(AMresultError((*p_stack)->result), nullptr), std::free};
        msg << "; " << c_msg.get() << ".";
        throw std::runtime_error(msg.str());
        return false;
    }
    if (p_data) {
        auto const tag = AMitemValType(AMresultItem((*p_stack)->result));
        if (!(tag & data_ptr->bitmask)) {
            msg << "Unexpected value type `" << AMvalTypeToString(tag) << "` (" << tag << ") at " << data_ptr->file
                << ":" << data_ptr->line << ".";
            throw std::runtime_error(msg.str());
            return false;
        }
    }
    return true;
}

/**************************************************************************/
/* uuid.cpp                                                               */
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

// platform
#ifdef _WIN32

#include <Rpc.h>

#else

#include <unistd.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#endif

// local
#include "uuid.h"

String generate_uuidv4() {
    String result{};
#ifdef _WIN32
    UUID uuid = {0};
    UuidCreate(&uuid);
    RPC_CSTR c_str = nullptr;
    UuidToString(&uuid, &c_str);
    if (c_str != nullptr) {
        result = String{reinterpret_cast<char*>(c_str)};
        RpcStringFree(&c_str);
    }
#else
    // Call the uuidgen command to avoid LGPL entanglements.
    static auto const TEMPLATE = "uuidgenXXXXXX";

    std::vector<char> filename{};
    for (auto next = TEMPLATE; *next; ++next) {
        filename.push_back(*next);
    }
    filename.push_back('\0');
    auto const fd = ::mkstemp(filename.data());
    if (fd != -1) {
        ::fflush(stdout);
        auto const prev_stdout = ::dup(1);
        ::dup2(fd, 1);
        ::close(fd);
        ::system("uuidgen");
        ::fflush(stdout);
        ::dup2(prev_stdout, 1);
        ::close(prev_stdout);
        std::ifstream ifs(filename.data());
        if (ifs) {
            std::vector<char> buffer;
            std::copy(std::istreambuf_iterator<std::ifstream::char_type>(ifs),
                      std::istreambuf_iterator<std::ifstream::char_type>(), std::back_inserter(buffer));
            if (!buffer.empty()) {
                // Ignore the linefeed.
                result = String{buffer.data(), static_cast<int>(buffer.size() - 1)};
            }
            ifs.close();
        }
        ::remove(filename.data());
    }

#endif
    return result;
}

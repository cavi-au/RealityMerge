/**************************************************************************/
/* usdj_basis.cpp                                                         */
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

#include <iterator>
#include <sstream>
#include <stdexcept>
#include <typeinfo>

// third-party
#include <cavi/usdj_am/value.hpp>

// regional
#include <core/math/vector3.h>

// local
#include "usdj_basis.h"
#include "usdj_vector.h"

Basis to_Basis(cavi::usdj_am::Value const& value) {
    using cavi::usdj_am::ConstValues;
    using cavi::usdj_am::Number;

    std::ostringstream args;
    Basis result{};
    try {
        /// \note A Godot Basis and a USD Matrix3d are both row-major.
        auto const& rows_ptr = std::get<std::unique_ptr<ConstValues>>(value);
        if (rows_ptr->size() != 3) {
            args << "std::get<" << typeid(decltype(rows_ptr)).name() << ">(value)->size() == " << rows_ptr->size();
        } else {
            auto result_row = std::begin(result.rows);
            std::size_t row_index = 0;
            for (auto const& row : *rows_ptr) {
                try {
                    *result_row = to_Vector<Vector3, real_t>(row);
                } catch (std::invalid_argument const& thrown) {
                    args << "(*std::get<" << typeid(decltype(rows_ptr)).name() << ">(value))[" << row_index
                         << "]: " << thrown.what();
                    break;
                }
                ++result_row;
                ++row_index;
            }
        }
    } catch (std::bad_variant_access const& thrown) {
        args << "std::get<std::unique_ptr<ConstValues>>(value): " << thrown.what();
    }
    if (!args.str().empty()) {
        std::ostringstream what;
        what << __func__ << "(" << args.str() << ")";
        std::invalid_argument(what.str());
    }
    return result;
}

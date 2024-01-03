/**************************************************************************/
/* usdj_projection.cpp                                                    */
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
#include <core/math/vector4.h>

// local
#include "usdj_projection.h"

Projection to_Projection(cavi::usdj_am::Value const& value) {
    using cavi::usdj_am::Number;
    using cavi::usdj_am::ValueRange;

    std::ostringstream args;
    Projection result{};
    try {
        /// \note A Godot Projection is column-major but a USD Matrix4d is row-major.
        auto const& rows = std::get<ValueRange>(value);
        if (rows.size() != Vector4::AXIS_COUNT) {
            args << "std::get<" << typeid(decltype(rows)).name() << ">(value).size() == " << rows.size();
        } else {
            std::size_t row_index = 0;
            std::size_t column_index = 0;
            std::size_t component_index = 0;
            for (auto const& row : rows) {
                try {
                    auto const& columns = std::get<ValueRange>(row);
                    if (columns.size() != 4) {
                        args << "(*std::get<" << typeid(decltype(rows)).name() << ">(value))[" << row_index
                             << "].size() == " << columns.size();
                        break;
                    } else {
                        for (auto const& column : columns) {
                            auto const& number = std::get<Number>(column);
                            std::visit(
                                [&](auto const& alt) {
                                    result.columns[column_index].components[component_index] = static_cast<real_t>(alt);
                                },
                                number);
                            if (++component_index == Vector4::AXIS_COUNT) {
                                ++column_index;
                                component_index = 0;
                            }
                        }
                    }
                    ++row_index;
                } catch (std::bad_variant_access const& thrown) {
                    args << "(*std::get<" << typeid(decltype(rows)).name() << ">(value))[" << row_index
                         << "]: " << thrown.what();
                    break;
                }
            }
        }
    } catch (std::bad_variant_access const& thrown) {
        args << "std::get<ValueRange>(value): " << thrown.what();
    }
    if (!args.str().empty()) {
        std::ostringstream what;
        what << __func__ << "(" << args.str() << ")";
        std::invalid_argument(what.str());
    }
    return result;
}

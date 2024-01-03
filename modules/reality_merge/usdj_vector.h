/**************************************************************************/
/* usdj_vector.h                                                         */
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

#ifndef REALITY_MERGE_USDJ_VECTOR_H
#define REALITY_MERGE_USDJ_VECTOR_H

#include <iterator>
#include <sstream>
#include <stdexcept>
#include <typeinfo>
#include <variant>

// third-party
#include <cavi/usdj_am/value.hpp>

/// \brief Converts a USDJ value into a Godot 3D vector.
///
/// \tparam VectorT The class of vector value to return.
/// \tparam AxisT The type of constituent axis values within \p VectorT.
/// \param[in] value A USDA-to-JSON `Value`.
/// \return A Godot \p VectorT instance.
/// \throws std::invalid_argument
template <class VectorT, typename AxisT>
VectorT to_Vector(cavi::usdj_am::Value const& value) {
    using cavi::usdj_am::Number;
    using cavi::usdj_am::ValueRange;

    std::ostringstream args;
    VectorT result{};
    try {
        auto const& values = std::get<ValueRange>(value);
        if (values.size() != VectorT::AXIS_COUNT) {
            args << "std::get<" << typeid(decltype(values)).name() << ">(value).size() == " << values.size();
        } else {
            auto axis = std::begin(result.coord);
            for (auto const& value : values) {
                auto const& number = std::get<Number>(value);
                std::visit([&](auto const& alt) { *axis = static_cast<AxisT>(alt); }, number);
                ++axis;
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

#endif  // REALITY_MERGE_USDJ_VECTOR_H

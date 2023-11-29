/**************************************************************************/
/* usd/geom/xform_op_type.cpp                                             */
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

#include <map>
#include <ostream>
#include <sstream>
#include <stdexcept>

// local
#include "detail/enum_string.hpp"
#include "usd/geom/xform_op_type.hpp"

namespace {

using cavi::usdj_am::usd::geom::XformOpType;

static std::map<std::string_view, XformOpType> const TAGS = {
    {"xformOp:translate", XformOpType::TRANSLATE},  {"xformOp:scale", XformOpType::SCALE},
    {"xformOp:rotateX", XformOpType::ROTATE_X},     {"xformOp:rotateY", XformOpType::ROTATE_Y},
    {"xformOp:rotateZ", XformOpType::ROTATE_Z},     {"xformOp:rotateXYZ", XformOpType::ROTATE_XYZ},
    {"xformOp:rotateXZY", XformOpType::ROTATE_XZY}, {"xformOp:rotateYXZ", XformOpType::ROTATE_YXZ},
    {"xformOp:rotateYZX", XformOpType::ROTATE_YZX}, {"xformOp:rotateZXY", XformOpType::ROTATE_ZXY},
    {"xformOp:rotateZYX", XformOpType::ROTATE_ZYX}, {"xformOp:orient", XformOpType::ORIENT},
    {"xformOp:transform", XformOpType::TRANSFORM},  {"!resetXformStack!", XformOpType::RESET_XFORM_STACK}};

}  // namespace

namespace cavi {
namespace usdj_am {
namespace usd {
namespace geom {

std::optional<XformOpType> extract_XformOpType(std::string_view const& view) {
    return detail::extract_enum_tag<XformOpType>(TAGS, view);
}

XformOpTypeOrder extract_XformOpTypeOrder(Value const& value) {
    return detail::extract_enum_tag_sequence<XformOpType>(TAGS, value);
}

XformOpType to_XformOpType(std::string_view const& view) {
    auto const result = detail::extract_enum_tag<XformOpType>(TAGS, view);
    if (!result) {
        std::ostringstream what;
        what << __func__ << "(view == \"" << view << "\")";
        throw std::invalid_argument(what.str());
    }
    return *result;
}

std::istream& operator>>(std::istream& is, XformOpType& out) {
    return detail::operator>><XformOpType>(is, std::make_pair(std::cref(TAGS), std::ref(out)));
}

std::ostream& operator<<(std::ostream& os, XformOpType const& in) {
    os << detail::extract_enum_string<XformOpType>(TAGS, in).value_or("???");
    return os;
}

}  // namespace geom
}  // namespace usd
}  // namespace usdj_am
}  // namespace cavi

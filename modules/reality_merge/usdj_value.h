/**************************************************************************/
/* usdj_value.h                                                           */
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

#ifndef REALITY_MERGE_USDJ_VALUE_H
#define REALITY_MERGE_USDJ_VALUE_H

#include <optional>
#include <variant>

// regional
#include <core/math/basis.h>
#include <core/math/color.h>
#include <core/math/math_defs.h>
#include <core/math/projection.h>
#include <core/math/quaternion.h>
#include <core/math/vector3.h>
#include <core/math/vector3i.h>
#include <core/math/vector4.h>
#include <core/math/vector4i.h>

// local
#include "usdj_reals.h"

namespace cavi {
namespace usdj_am {

class Declaration;

}  // namespace usdj_am
}  // namespace cavi

using UsdjValue =
    std::variant<real_t, Reals, Basis, Color, Projection, Quaternion, Vector3, Vector3i, Vector4, Vector4i>;

/// \brief Extracts the Godot counterpart of a USD value from within the given
///        USDJ declaration, if any.
///
/// \param[in] declaration A "USDA_Declaration" node.
/// \returns A `UsdjValue` instance or `std::nullopt`.
/// \throws `std::logic_error` if the type of the embedded USD value isn't supported.
std::optional<UsdjValue> extract_UsdjValue(cavi::usdj_am::Declaration const& declaration);

/// \brief Converts a USDJ declaration into the Godot counterpart of the USD
///        value embedded within it.
///
/// \param[in] declaration A "USDA_Declaration" node.
/// \return A `UsdjValue` instance.
/// \throws `std::invalid_argument` if \p declaration doesn't contain a USD value.
/// \throws `std::logic_error` if the type of the embedded USD value isn't supported.
UsdjValue to_UsdjValue(cavi::usdj_am::Declaration const& declaration);

#endif  // REALITY_MERGE_USDJ_VALUE_H

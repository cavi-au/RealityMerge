/**************************************************************************/
/* definition.cpp                                                         */
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

// local
#include "definition.hpp"
#include "descriptor.hpp"
#include "visitor.hpp"

namespace cavi {
namespace usdj_am {

Definition::Definition(AMdoc const* const document, AMitem const* const map_object) : Node(document, map_object, 6) {
    check_enum_property("type", StatementType::DEFINITION);
}

void Definition::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

std::optional<Typename> Definition::get_def_type() const {
    return get_nullable_object_property<String>("defType");
}

std::optional<Descriptor> Definition::get_descriptor() const {
    return get_nullable_object_property<Descriptor>("descriptor");
}

String Definition::get_name() const {
    return get_object_property<String>("name");
}

Definition::Statements Definition::get_statements() const {
    return get_array_property<Statements>("statements");
}

DefinitionType Definition::get_sub_type() const {
    return get_enum_property<DefinitionType>("subType");
}

}  // namespace usdj_am
}  // namespace cavi

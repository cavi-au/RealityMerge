/**************************************************************************/
/* class_definition.cpp                                                   */
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
#include "class_definition.hpp"
#include "class_declaration.hpp"
#include "descriptor.hpp"
#include "visitor.hpp"

namespace cavi {
namespace usdj_am {

ClassDefinition::ClassDefinition(AMdoc const* const document, AMitem const* const map_object)
    : Node(document, map_object, 5) {
    check_enum_property("type", StatementType::CLASS_DEFINITION);
}

void ClassDefinition::accept(Visitor& visitor) const& {
    visitor.visit(*this);
}

void ClassDefinition::accept(Visitor& visitor) && {
    visitor.visit(std::forward<ClassDefinition>(*this));
}

ClassDefinition::ClassDeclarations ClassDefinition::get_class_declarations() const {
    return get_array_property<ClassDeclarations>("classDeclarations");
}

std::optional<Descriptor> ClassDefinition::get_descriptor() const {
    return get_nullable_object_property<Descriptor>("descriptor");
}

std::optional<String> ClassDefinition::get_id() const {
    return get_nullable_object_property<String>("id");
}

String ClassDefinition::get_name() const {
    return get_object_property<String>("name");
}

}  // namespace usdj_am
}  // namespace cavi

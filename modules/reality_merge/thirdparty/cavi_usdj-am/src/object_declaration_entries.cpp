/**************************************************************************/
/* object_declaration_entries.cpp                                         */
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
#include "object_declaration_entries.hpp"
#include "visitor.hpp"

namespace cavi {
namespace usdj_am {

ObjectDeclarationEntries::ObjectDeclarationEntries(AMdoc const* const document, AMitem const* const map_object)
    : Node(document, map_object, 2) {
    check_string_property("type", "objectDeclarationEntries");
}

void ObjectDeclarationEntries::accept(Visitor& visitor) const& {
    visitor.visit(*this);
}

void ObjectDeclarationEntries::accept(Visitor& visitor) && {
    visitor.visit(std::forward<ObjectDeclarationEntries>(*this));
}

String ObjectDeclarationEntries::get_type() const {
    return get_object_property<String>("type");
}

ObjectDeclarationEntries::Values ObjectDeclarationEntries::get_values() const {
    return get_array_property<Values>("values");
}

}  // namespace usdj_am
}  // namespace cavi

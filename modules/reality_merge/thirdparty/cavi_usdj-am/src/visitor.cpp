/**************************************************************************/
/* visitor.cpp                                                            */
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

#include <sstream>
#include <stdexcept>
#include <typeinfo>

// local
#include "assignment.hpp"
#include "class_declaration.hpp"
#include "class_definition.hpp"
#include "declaration.hpp"
#include "definition.hpp"
#include "definition_statement.hpp"
#include "descriptor.hpp"
#include "external_reference.hpp"
#include "external_reference_import.hpp"
#include "file.hpp"
#include "object_declaration.hpp"
#include "object_declaration_entries.hpp"
#include "object_declaration_list.hpp"
#include "object_declaration_list_value.hpp"
#include "object_value.hpp"
#include "reference_file.hpp"
#include "variant_definition.hpp"
#include "variant_set.hpp"
#include "visitor.hpp"

namespace cavi {
namespace usdj_am {

Visitor::~Visitor() {}

void Visitor::visit(Assignment&& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(Assignment const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(ClassDeclaration&& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(ClassDeclaration const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(ClassDefinition&& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(ClassDefinition const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(Declaration&& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(Declaration const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(Definition&& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(Definition const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(DefinitionStatement&& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(DefinitionStatement const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(Descriptor&& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(Descriptor const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(ExternalReference&& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(ExternalReference const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(ExternalReferenceImport&& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(ExternalReferenceImport const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(File&& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(File const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(ObjectDeclaration&& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(ObjectDeclaration const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(ObjectDeclarationEntries&& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(ObjectDeclarationEntries const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(ObjectDeclarationList&& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(ObjectDeclarationList const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(ObjectDeclarationListValue&& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(ObjectDeclarationListValue const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(ObjectDeclarations&& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(ObjectDeclarations const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(ObjectValue&& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}
void Visitor::visit(ObjectValue const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(ReferenceFile&& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(ReferenceFile const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(Statement&& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(Statement const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(Value&& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(Value const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(VariantDefinition&& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(VariantDefinition const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(VariantSet&& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void Visitor::visit(VariantSet const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

}  // namespace usdj_am
}  // namespace cavi

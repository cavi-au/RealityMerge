/**************************************************************************/
/* object_declarations.cpp                                                */
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

#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <typeinfo>

// third-party
extern "C" {

#include <automerge-c/automerge.h>
}

// local
#include "object_declaration_entries.hpp"
#include "object_declaration_list.hpp"
#include "object_declarations.hpp"
#include "visitor.hpp"

namespace cavi {
namespace usdj_am {

ObjectDeclarations::ObjectDeclarations(AMdoc const* const document, AMitem const* const map_object) {
    enum { BEGIN_, LIST = BEGIN_, ENTRIES, END_, SIZE_ = END_ - BEGIN_ };

    std::ostringstream arguments;
    for (std::size_t index = BEGIN_; index != END_; ++index) {
        try {
            switch (index) {
                case LIST: {
                    this->emplace<std::unique_ptr<ObjectDeclarationList> >(
                        std::make_unique<ObjectDeclarationList>(document, map_object));
                    break;
                }
                case ENTRIES: {
                    this->emplace<std::unique_ptr<ObjectDeclarationEntries> >(
                        std::make_unique<ObjectDeclarationEntries>(document, map_object));
                    break;
                }
            }
            arguments.str("");
            break;
        } catch (std::invalid_argument const& thrown) {
            if (!arguments.str().empty()) {
                arguments << " | ";
            }
            arguments << thrown.what();
        }
    }
    if (!arguments.str().empty()) {
        std::ostringstream what;
        what << typeid(*this).name() << "::" << __func__ << "(" << arguments.str() << ")";
        throw std::invalid_argument(what.str());
    }
}

ObjectDeclarations::~ObjectDeclarations() {}

void ObjectDeclarations::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

}  // namespace usdj_am
}  // namespace cavi

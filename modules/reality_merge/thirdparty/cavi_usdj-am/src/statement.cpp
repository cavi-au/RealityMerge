/**************************************************************************/
/* statement.cpp                                                          */
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
#include <type_traits>
#include <typeinfo>

// third-party
extern "C" {

#include <automerge-c/automerge.h>
}

// local
#include "class_definition.hpp"
#include "definition.hpp"
#include "statement.hpp"
#include "statement_type.hpp"
#include "variant_set.hpp"
#include "visitor.hpp"

namespace cavi {
namespace usdj_am {

Statement::Statement(AMdoc const* const document, AMitem const* const map_object) {
    using Index = typename std::underlying_type<StatementType>::type;

    std::ostringstream args;
    for (Index index = static_cast<Index>(StatementType::BEGIN__); index != static_cast<Index>(StatementType::END__);
         ++index) {
        try {
            switch (static_cast<StatementType>(index)) {
                case StatementType::CLASS_DEFINITION: {
                    this->emplace<std::unique_ptr<ClassDefinition> >(
                        std::make_unique<ClassDefinition>(document, map_object));
                    break;
                }
                case StatementType::DEFINITION: {
                    this->emplace<std::unique_ptr<Definition> >(std::make_unique<Definition>(document, map_object));
                    break;
                }
                case StatementType::VARIANT_SET: {
                    this->emplace<std::unique_ptr<VariantSet> >(std::make_unique<VariantSet>(document, map_object));
                    break;
                }
                default:
                    continue;
            }
            args.str("");
            break;
        } catch (std::invalid_argument const& thrown) {
            if (!args.str().empty()) {
                args << " | ";
            }
            args << thrown.what();
        }
    }
    if (!args.str().empty()) {
        std::ostringstream what;
        what << typeid(*this).name() << "::" << __func__ << "(" << args.str() << ")";
        throw std::invalid_argument(what.str());
    }
}

Statement::~Statement() {}

void Statement::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

}  // namespace usdj_am
}  // namespace cavi

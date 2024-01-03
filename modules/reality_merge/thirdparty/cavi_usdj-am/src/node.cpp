/**************************************************************************/
/* node.cpp                                                               */
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
#include <stdexcept>
#include <string_view>
#include <typeinfo>

// third-party
extern "C" {

#include <automerge-c/utils/enum_string.h>
}

// local
#include "assignment.hpp"
#include "assignment_keyword.hpp"
#include "class_definition.hpp"
#include "declaration_keyword.hpp"
#include "definition.hpp"
#include "definition_type.hpp"
#include "descriptor.hpp"
#include "external_reference_import.hpp"
#include "file.hpp"
#include "node.hpp"
#include "number.hpp"
#include "object_declaration_entries.hpp"
#include "object_declaration_list.hpp"
#include "object_declarations.hpp"
#include "reference_file.hpp"
#include "statement_type.hpp"
#include "value.hpp"
#include "value_type.hpp"
#include "variant_definition.hpp"
#include "variant_set.hpp"

namespace cavi {
namespace usdj_am {

Node::Node(AMdoc const* const document) : m_document{document} {
    std::ostringstream args;
    if (!document) {
        args << "document == nullptr";
    }
    if (!args.str().empty()) {
        std::ostringstream what;
        what << typeid(*this).name() << "::" << __func__ << "(" << args.str() << ")";
        throw std::invalid_argument(what.str());
    }
}

Node::Node(AMdoc const* const document, AMitem const* const map_object, int const map_size) : m_document{document} {
    std::ostringstream args;
    if (!document) {
        args << "document == nullptr, ..., ...";
    } else if (!map_object) {
        args << "..., map_object == nullptr, ...";
    } else if (!(map_size > 0)) {
        args << "..., ..., map_size == " << map_size;
    } else {
        AMvalType const val_type = AMitemValType(map_object);
        if (val_type != AM_VAL_TYPE_OBJ_TYPE) {
            args << "..., "
                 << "AMitemValType(map_object) == " << AMvalTypeToString(val_type) << ", ...";
        } else {
            AMobjId const* const obj_id = AMitemObjId(map_object);
            AMobjType const obj_type = AMobjObjType(document, obj_id);
            if (obj_type != AM_OBJ_TYPE_MAP) {
                args << "AMobjObjType(document, AMitemObjId(map_object)) == " << AMobjTypeToString(obj_type) << ", ...";
            } else {
                std::size_t const obj_size = AMobjSize(document, obj_id, nullptr);
                if (obj_size != map_size) {
                    args << "AMobjSize(document, AMitemObjId(map_object), nullptr) == " << obj_size << ", " << map_size;
                }
            }
        }
    }
    if (!args.str().empty()) {
        std::ostringstream what;
        what << typeid(*this).name() << "::" << __func__ << "(" << args.str() << ")";
        throw std::invalid_argument(what.str());
    }
    // Preserve the AMitem storing the node's object ID.
    ResultPtr result{AMitemResult(map_object), AMresultFree};
    m_results.insert_or_assign("", result);
}

Node::~Node() {}

template <typename EnumT>
void Node::check_enum_property(std::string const& key, EnumT const tag) const {
    std::ostringstream args;
    try {
        if (get_enum_property<EnumT>(key) != tag) {
            args << "AMmapGet(m_document, ..., AMstr(\"" << key << "\"), nullptr) == \""
                 << get_object_property<String>(key) << "\", \"" << tag << "\"";
        }
    } catch (std::invalid_argument const& thrown) {
        args << thrown.what();
    }
    // Free the AMresult because we're finished with it.
    m_results.erase(key);
    if (!args.str().empty()) {
        std::ostringstream what;
        what << typeid(*this).name() << "::" << __func__ << "(" << args.str() << ")";
        throw std::invalid_argument(what.str());
    }
}

void Node::check_string_property(std::string const& key, std::string const& value) const {
    std::ostringstream args;
    try {
        auto const property = get_object_property<String>(key);
        if (property != value) {
            args << "AMmapGet(m_document, get_object_id, AMstr(\"" << key << "\"), nullptr) == \"" << property
                 << "\", \"" << value << "\"";
        }
    } catch (std::invalid_argument const& thrown) {
        args << thrown.what();
    }
    // Free the AMresult because we're finished with it.
    m_results.erase(key);
    if (!args.str().empty()) {
        std::ostringstream what;
        what << typeid(*this).name() << "::" << __func__ << "(" << args.str() << ")";
        throw std::invalid_argument(what.str());
    }
}

template <typename InputRangeT>
InputRangeT Node::get_array_property(std::string const& key) const {
    ResultPtr const result{AMmapGet(m_document, get_object_id(), utils::to_bytes(key), nullptr), AMresultFree};
    auto [iter, inserted] = m_results.insert_or_assign(key, result);
    return InputRangeT{m_document, AMresultItem(iter->second.get())};
}

template <typename EnumT>
EnumT Node::get_enum_property(std::string const& key) const {
    ResultPtr const result{AMmapGet(m_document, get_object_id(), utils::to_bytes(key), nullptr), AMresultFree};
    std::ostringstream args;
    if (!result) {
        args << "AMmapGet(m_document, ..., AMstr(\"" << key << "\"), nullptr) == nullptr";
    } else {
        auto [iter, inserted] = m_results.insert_or_assign(key, result);
        try {
            String string{m_document, AMresultItem(iter->second.get())};
            EnumT tag;
            auto const buffer = std::string{string};
            std::istringstream iss{buffer};
            if (iss >> tag) {
                return tag;
            } else {
                args << "AMmapGet(m_document, ..., AMstr(\"" << key << "\"), nullptr) == \"" << string << "\"";
            }
        } catch (std::invalid_argument const& thrown) {
            args << thrown.what();
        }
    }
    if (!args.str().empty()) {
        std::ostringstream what;
        what << typeid(*this).name() << "::" << __func__ << "(" << args.str() << ")";
        throw std::invalid_argument(what.str());
    }
    return EnumT{};
}

template <typename EnumT>
std::optional<EnumT> Node::get_nullable_enum_property(std::string const& key) const {
    std::optional<EnumT> nullable_enum;
    try {
        nullable_enum.emplace(get_enum_property<EnumT>(key));
    } catch (std::invalid_argument const&) {
        if (AMitemValType(AMresultItem(m_results.at(key).get())) != AM_VAL_TYPE_NULL) {
            throw;
        }
    }
    return nullable_enum;
}

template <typename ObjectT>
std::optional<ObjectT> Node::get_nullable_object_property(std::string const& key) const {
    std::optional<ObjectT> nullable_object;
    try {
        nullable_object.emplace(get_object_property<ObjectT>(key));
    } catch (std::invalid_argument const&) {
        if (AMitemValType(AMresultItem(m_results.at(key).get())) != AM_VAL_TYPE_NULL) {
            throw;
        }
    }
    return nullable_object;
}

AMobjId const* Node::get_object_id() const {
    return AMitemObjId(AMresultItem(m_results.at("").get()));
}

template <typename ObjectT>
ObjectT Node::get_object_property(std::string const& key) const {
    ResultPtr const result{AMmapGet(m_document, get_object_id(), utils::to_bytes(key), nullptr), AMresultFree};
    auto [iter, inserted] = m_results.insert_or_assign(key, result);
    try {
        return ObjectT{m_document, AMresultItem(iter->second.get())};
    } catch (std::invalid_argument const& thrown) {
        std::ostringstream what;
        what << typeid(*this).name() << "::" << __func__ << "(" << thrown.what() << ")";
        throw std::invalid_argument(what.str());
    }
}

// Node::check_enum_property()
template void Node::check_enum_property<AssignmentType>(std::string const&, AssignmentType const) const;

template void Node::check_enum_property<StatementType>(std::string const&, StatementType const) const;

template void Node::check_enum_property<ValueType>(std::string const&, ValueType const) const;

// Node::get_array_property()
template ClassDefinition::ClassDeclarations Node::get_array_property<ClassDefinition::ClassDeclarations>(
    std::string const&) const;

template Definition::Statements Node::get_array_property<Definition::Statements>(std::string const&) const;

template Descriptor::Assignments Node::get_array_property<Descriptor::Assignments>(std::string const&) const;

template File::Statements Node::get_array_property<File::Statements>(std::string const&) const;

template ObjectDeclarationEntries::Values Node::get_array_property<ObjectDeclarationEntries::Values>(
    std::string const&) const;

template ObjectDeclarationList::Values Node::get_array_property<ObjectDeclarationList::Values>(
    std::string const&) const;

template VariantDefinition::Definitions Node::get_array_property<VariantDefinition::Definitions>(
    std::string const&) const;

template VariantSet::VariantDefinitions Node::get_array_property<VariantSet::VariantDefinitions>(
    std::string const&) const;

// Node::get_enum_property()
template AssignmentKeyword Node::get_enum_property<AssignmentKeyword>(std::string const&) const;

template DeclarationKeyword Node::get_enum_property<DeclarationKeyword>(std::string const&) const;

template DefinitionType Node::get_enum_property<DefinitionType>(std::string const&) const;

// Node::get_nullable_enum_property()
template std::optional<AssignmentKeyword> Node::get_nullable_enum_property<AssignmentKeyword>(std::string const&) const;

template std::optional<DeclarationKeyword> Node::get_nullable_enum_property<DeclarationKeyword>(
    std::string const&) const;

// Node::get_nullable_object_property()
template std::optional<Descriptor> Node::get_nullable_object_property<Descriptor>(std::string const&) const;

template std::optional<ExternalReferenceImport> Node::get_nullable_object_property<ExternalReferenceImport>(
    std::string const&) const;

template std::optional<String> Node::get_nullable_object_property<String>(std::string const&) const;

// Node::get_object_property()
template Number Node::get_object_property<Number>(std::string const&) const;

template ObjectDeclarations Node::get_object_property<ObjectDeclarations>(std::string const&) const;

template String Node::get_object_property<String>(std::string const&) const;

template ReferenceFile Node::get_object_property<ReferenceFile>(std::string const&) const;

template Value Node::get_object_property<Value>(std::string const&) const;

}  // namespace usdj_am
}  // namespace cavi

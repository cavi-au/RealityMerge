/**************************************************************************/
/* json_writer.cpp                                                        */
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

#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>

// local
#include "utils/json_writer.hpp"

namespace cavi {
namespace usdj_am {
namespace utils {

JsonWriter::JsonWriter(JsonWriter::Indenter&& indenter, std::size_t const precision)
    : m_indenter{indenter}, m_precision{precision} {}

JsonWriter::operator std::string() const {
    return m_os.str();
}

void JsonWriter::visit(Assignment const& assignment) {
    m_os << "{\n";
    ++m_indenter;
    m_os << m_indenter << "\"type\": \"" << assignment.get_type() << "\",\n";
    m_os << m_indenter << "\"keyword\": ";
    auto keyword = assignment.get_keyword();
    if (keyword) {
        m_os << "\"" << *keyword << "\"";
    } else {
        m_os << "null";
    }
    m_os << ",\n";
    m_os << m_indenter << "\"identifier\": \"" << assignment.get_identifier() << "\",\n";
    m_os << m_indenter << "\"value\": ";
    assignment.get_value().accept(*this);
    m_os << "\n";
    --m_indenter;
    m_os << m_indenter << "}";
}

void JsonWriter::visit(ClassDeclaration const& class_declaration) {
    std::visit([this](auto const& alt) { alt->accept(*this); }, class_declaration);
}

void JsonWriter::visit(ClassDefinition const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void JsonWriter::visit(Declaration const& declaration) {
    m_os << "{\n";
    ++m_indenter;
    m_os << m_indenter << "\"type\": \"" << declaration.get_type() << "\",\n";
    m_os << m_indenter << "\"keyword\": ";
    auto keyword = declaration.get_keyword();
    if (keyword) {
        m_os << "\"" << *keyword << "\"";
    } else {
        m_os << "null";
    }
    m_os << ",\n";
    m_os << m_indenter << "\"defineType\": \"" << declaration.get_define_type() << "\",\n";
    m_os << m_indenter << "\"reference\": \"" << declaration.get_reference() << "\",\n";
    m_os << m_indenter << "\"value\": ";
    declaration.get_value().accept(*this);
    m_os << ",\n";
    m_os << m_indenter << "\"descriptor\": ";
    auto descriptor = declaration.get_descriptor();
    if (descriptor) {
        descriptor->accept(*this);
    } else {
        m_os << "null";
    }
    m_os << "\n";
    --m_indenter;
    m_os << m_indenter << "}";
}

void JsonWriter::visit(Definition const& definition) {
    m_os << "{\n";
    ++m_indenter;
    m_os << m_indenter << "\"type\": \"" << definition.get_type() << "\",\n";
    m_os << m_indenter << "\"subType\": \"" << definition.get_sub_type() << "\",\n";
    m_os << m_indenter << "\"defType\": ";
    auto def_type = definition.get_def_type();
    if (def_type) {
        m_os << "\"" << *def_type << "\"";
    } else {
        m_os << "null";
    }
    m_os << ",\n";
    m_os << m_indenter << "\"name\": \"" << definition.get_name() << "\",\n";
    m_os << m_indenter << "\"descriptor\": ";
    auto descriptor = definition.get_descriptor();
    if (descriptor) {
        descriptor->accept(*this);
    } else {
        m_os << "null";
    }
    m_os << ",\n";
    m_os << m_indenter << "\"statements\": ";
    write_array(definition.get_statements());
    m_os << "\n";
    --m_indenter;
    m_os << m_indenter << "}";
}

void JsonWriter::visit(DefinitionStatement const& definition_statement) {
    std::visit([this](auto const& alt) { alt->accept(*this); }, definition_statement);
}

void JsonWriter::visit(Descriptor const& descriptor) {
    m_os << "{\n";
    ++m_indenter;
    m_os << m_indenter << "\"description\": ";
    auto description = descriptor.get_description();
    if (description) {
        m_os << *description;
    } else {
        m_os << "null";
    }
    m_os << ",\n";
    m_os << m_indenter << "\"assignments\": ";
    write_array(descriptor.get_assignments());
    m_os << "\n";
    --m_indenter;
    m_os << m_indenter << "}";
}

void JsonWriter::visit(ExternalReference const& external_reference) {
    m_os << "{\n";
    ++m_indenter;
    m_os << m_indenter << "\"type\": \"" << external_reference.get_type() << "\",\n";
    m_os << m_indenter << "\"referenceFile\": ";
    external_reference.get_reference_file().accept(*this);
    m_os << ",\n";
    m_os << m_indenter << "\"toImport\": ";
    auto to_import = external_reference.get_to_import();
    if (to_import) {
        to_import->accept(*this);
    } else {
        m_os << "null";
    }
    m_os << "\n";
    --m_indenter;
    m_os << m_indenter << "}";
}

void JsonWriter::visit(ExternalReferenceImport const& external_reference_import) {
    m_os << "{\n";
    ++m_indenter;
    m_os << m_indenter << "\"type\": \"" << external_reference_import.get_type() << "\",\n";
    m_os << m_indenter << "\"importPath\": \"" << external_reference_import.get_import_path() << "\",\n";
    m_os << m_indenter << "\"field\": ";
    auto field = external_reference_import.get_field();
    if (field) {
        m_os << "\"" << *field << "\"";
    } else {
        m_os << "null";
    }
    m_os << "\n";
    --m_indenter;
    m_os << m_indenter << "}";
}

void JsonWriter::visit(File const& file) {
    try {
        m_os << "{\n";
        m_os << m_indenter << "\"version\": " << file.get_version() << ",\n";
        m_os << m_indenter << "\"descriptor\": ";
        auto descriptor = file.get_descriptor();
        if (descriptor) {
            descriptor->accept(*this);
        } else {
            m_os << "null";
        }
        m_os << ",\n";
        m_os << m_indenter << "\"statements\": ";
        write_array(file.get_statements());
        m_os << "\n";
        m_os << "}";
    } catch (std::invalid_argument const& thrown) {
        std::cout << m_os.str() << std::endl;
        throw;
    }
}

void JsonWriter::visit(ObjectDeclaration const& object_declaration) {
    m_os << "{\n";
    ++m_indenter;
    m_os << m_indenter << "\"keyword\": ";
    auto keyword = object_declaration.get_keyword();
    if (keyword) {
        m_os << "\"" << *keyword << "\"";
    } else {
        m_os << "null";
    }
    m_os << ",\n";
    m_os << m_indenter << "\"defineType\": \"" << object_declaration.get_define_type() << "\",\n";
    m_os << m_indenter << "\"reference\": \"" << object_declaration.get_reference() << "\",\n";
    m_os << m_indenter << "\"value\": ";
    object_declaration.get_value().accept(*this);
    m_os << "\n";
    --m_indenter;
    m_os << m_indenter << "}";
}

void JsonWriter::visit(ObjectDeclarationEntries const& object_declaration_entries) {
    m_os << "{\n";
    ++m_indenter;
    m_os << m_indenter << "\"type\": \"" << object_declaration_entries.get_type() << "\",\n";
    m_os << m_indenter << "\"values\": ";
    write_array(object_declaration_entries.get_values());
    m_os << "\n";
    --m_indenter;
    m_os << m_indenter << "}";
}

void JsonWriter::visit(ObjectDeclarationList const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void JsonWriter::visit(ObjectDeclarationListValue const& node) {
    std::ostringstream what;
    what << typeid(*this).name() << "::" << __func__ << "(" << typeid(node).name() << ") isn't implemented.";
    throw std::logic_error(what.str());
}

void JsonWriter::visit(ObjectDeclarations const& object_declarations) {
    std::visit([this](auto const& alt) { alt->accept(*this); }, object_declarations);
}

void JsonWriter::visit(ObjectValue const& object_value) {
    m_os << "{\n";
    ++m_indenter;
    m_os << m_indenter << "\"type\": \"" << object_value.get_type() << "\",\n";
    m_os << m_indenter << "\"declarations\": ";
    object_value.get_declarations().accept(*this);
    m_os << "\n";
    --m_indenter;
    m_os << m_indenter << "}";
}

void JsonWriter::visit(ReferenceFile const& reference_file) {
    m_os << "{\n";
    ++m_indenter;
    m_os << m_indenter << "\"type\": \"" << reference_file.get_type() << "\",\n";
    m_os << m_indenter << "\"src\": \"" << reference_file.get_src() << "\",\n";
    m_os << m_indenter << "\"descriptor\": ";
    auto descriptor = reference_file.get_descriptor();
    if (descriptor) {
        descriptor->accept(*this);
    } else {
        m_os << "null";
    }
    m_os << "\n";
    --m_indenter;
    m_os << m_indenter << "}";
}

void JsonWriter::visit(Statement const& statement) {
    std::visit([this](auto const& alt) { alt->accept(*this); }, statement);
}

void JsonWriter::visit(Value const& value) {
    std::visit(
        [&](auto const& alt) {
            using T = std::decay_t<decltype(alt)>;
            if constexpr (std::is_same_v<T, std::monostate>)
                m_os << "undefined";
            else if constexpr (std::is_same_v<T, std::unique_ptr<String> >)
                m_os << "\"" << *alt << "\"";
            else if constexpr (std::is_same_v<T, bool>) {
                auto const fmtflags = m_os.flags();
                m_os << std::boolalpha << alt;
                m_os.setf(fmtflags);
            } else if constexpr (std::is_same_v<T, Number>) {
                auto const precision = m_os.precision();
                m_os << std::setprecision(m_precision) << alt << std::setprecision(precision);
            } else if constexpr (std::is_same_v<T, std::unique_ptr<ConstInputRange<Value> > >)
                write_array(*alt);
            else if constexpr (std::is_same_v<T, std::unique_ptr<ExternalReferenceImport> > ||
                               std::is_same_v<T, std::unique_ptr<ExternalReference> > ||
                               std::is_same_v<T, std::unique_ptr<ObjectValue> >)
                alt->accept(*this);
            else if constexpr (std::is_same_v<T, std::nullptr_t>)
                m_os << "null";
        },
        value);
}

void JsonWriter::visit(VariantDefinition const& variant_definition) {
    m_os << "{\n";
    ++m_indenter;
    m_os << m_indenter << "\"type\": \"" << variant_definition.get_type() << "\",\n";
    m_os << m_indenter << "\"name\": \"" << variant_definition.get_name() << "\",\n";
    m_os << m_indenter << "\"descriptor\": ";
    auto descriptor = variant_definition.get_descriptor();
    if (descriptor) {
        descriptor->accept(*this);
    } else {
        m_os << "null";
    }
    m_os << ",\n";
    m_os << m_indenter << "\"definitions\": ";
    write_array(variant_definition.get_definitions());
    m_os << "\n";
    --m_indenter;
    m_os << m_indenter << "}";
}

void JsonWriter::visit(VariantSet const& variant_set) {
    m_os << "{\n";
    ++m_indenter;
    m_os << m_indenter << "\"type\": \"" << variant_set.get_type() << "\",\n";
    m_os << m_indenter << "\"name\": \"" << variant_set.get_name() << "\",\n";
    m_os << m_indenter << "\"definitions\": ";
    write_array(variant_set.get_definitions());
    m_os << "\n";
    --m_indenter;
    m_os << m_indenter << "}";
}

template <typename InputRangeT>
void JsonWriter::write_array(InputRangeT const& input_range) {
    m_os << "[";
    if (input_range.size()) {
        m_os << "\n";
        ++m_indenter;
        std::size_t count = 0;
        for (auto const& next : input_range) {
            if (count++) {
                m_os << ",\n";
            }
            m_os << m_indenter;
            next.accept(*this);
        }
        m_os << "\n";
        --m_indenter;
        m_os << m_indenter << "]";
    } else {
        m_os << "]";
    }
}

JsonWriter::Indenter::Indenter(char const fill, std::size_t const span, std::size_t const count)
    : m_fill{fill}, m_span{span}, m_count{count} {}

JsonWriter::Indenter& JsonWriter::Indenter::operator++() {
    ++m_count;
    return *this;
}

JsonWriter::Indenter JsonWriter::Indenter::operator++(int) {
    Indenter current{*this};
    ++(*this);
    return current;
}

JsonWriter::Indenter& JsonWriter::Indenter::operator--() {
    if (m_count)
        --m_count;
    return *this;
}

JsonWriter::Indenter JsonWriter::Indenter::operator--(int) {
    Indenter current{*this};
    --(*this);
    return current;
}

std::ostream& operator<<(std::ostream& os, JsonWriter::Indenter const& in) {
    os << static_cast<std::string>(in);
    return os;
}

}  // namespace utils
}  // namespace usdj_am
}  // namespace cavi

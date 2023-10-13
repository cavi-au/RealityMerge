/**************************************************************************/
/* json_writer.hpp                                                        */
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

#ifndef CAVI_USDJ_AM_UTILS_JSON_WRITER_HPP
#define CAVI_USDJ_AM_UTILS_JSON_WRITER_HPP

#include <cstddef>
#include <iosfwd>
#include <string>

// local
#include "assignment.hpp"
#include "class_definition.hpp"
#include "declaration.hpp"
#include "definition.hpp"
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

namespace utils {

class JsonWriter : public Visitor {
public:
    class Indenter {
    public:
        Indenter() = delete;

        Indenter(char const fill, std::size_t const span, std::size_t const count = 1);

        Indenter(Indenter const&) = default;

        Indenter(Indenter&&) = default;

        Indenter& operator=(Indenter const&) = default;

        Indenter& operator=(Indenter&&) = delete;

        Indenter& operator++();

        Indenter operator++(int);

        Indenter& operator--();

        Indenter operator--(int);

        operator std::string() const;

    private:
        char const m_fill;
        std::size_t const m_span;
        std::size_t m_count;
    };

    JsonWriter() = delete;

    JsonWriter(Indenter&& indenter, std::size_t const precision = 7);

    JsonWriter(JsonWriter const&) = delete;

    JsonWriter(JsonWriter&&) = default;

    ~JsonWriter() = default;

    JsonWriter& operator=(JsonWriter const&) = delete;

    JsonWriter& operator=(JsonWriter&&) = default;

    operator std::string() const;

    void visit(Assignment const&);

    void visit(ClassDeclaration const&);

    void visit(ClassDefinition const&);

    void visit(Declaration const&);

    void visit(Definition const&);

    void visit(DefinitionStatement const&);

    void visit(Descriptor const&);

    void visit(ExternalReference const&);

    void visit(ExternalReferenceImport const&);

    void visit(File const&);

    void visit(ObjectDeclaration const&);

    void visit(ObjectDeclarationEntries const&);

    void visit(ObjectDeclarationList const&);

    void visit(ObjectDeclarationListValue const&);

    void visit(ObjectDeclarations const&);

    void visit(ObjectValue const&);

    void visit(ReferenceFile const&);

    void visit(Statement const&);

    void visit(Value const&);

    void visit(VariantDefinition const&);

    void visit(VariantSet const&);

private:
    template <typename InputRangeT>
    void write_array(InputRangeT const& input_range);

    Indenter m_indenter;
    std::ostringstream m_os;
    std::size_t const m_precision;
};

inline JsonWriter::Indenter::operator std::string() const {
    return std::string(m_count * m_span, m_fill);
}

std::ostream& operator<<(std::ostream& os, JsonWriter::Indenter const& in);

}  // namespace utils
}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_UTILS_JSON_WRITER_HPP

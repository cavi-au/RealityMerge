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
#include <sstream>
#include <string>

// local
#include <cavi/usdj_am/visitor.hpp>

namespace cavi {
namespace usdj_am {
namespace utils {

/// \brief Writes the contents of a "USDA_File" node into a string.
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

    static constexpr const std::size_t DEFAULT_PRECISION = 7;

    JsonWriter() = delete;

    /// \brief Configures the formatting of the JSON output and child node
    ///        descent.
    /// \param[in] indenter An indent string generator.
    /// \param[in] precision The precision of floating point value output.
    JsonWriter(Indenter&& indenter, std::size_t const precision = DEFAULT_PRECISION);

    JsonWriter(JsonWriter const&) = delete;

    JsonWriter(JsonWriter&&) = default;

    ~JsonWriter() = default;

    JsonWriter& operator=(JsonWriter const&) = delete;

    JsonWriter& operator=(JsonWriter&&) = default;

    operator std::string() const;

    void visit(Assignment const&) override;

    void visit(ClassDeclaration const&) override;

    void visit(Declaration const&) override;

    void visit(Definition const&) override;

    void visit(DefinitionStatement const&) override;

    void visit(Descriptor const&) override;

    void visit(ExternalReference const&) override;

    void visit(ExternalReferenceImport const&) override;

    void visit(File const&) override;

    void visit(ObjectDeclaration const&) override;

    void visit(ObjectDeclarationEntries const&) override;

    void visit(ObjectDeclarations const&) override;

    void visit(ObjectValue const&) override;

    void visit(ReferenceFile const&) override;

    void visit(Statement const&) override;

    void visit(Value const&) override;

    void visit(VariantDefinition const&) override;

    void visit(VariantSet const&) override;

private:
    template <typename InputRangeT>
    void write_array(InputRangeT const& array_range);

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

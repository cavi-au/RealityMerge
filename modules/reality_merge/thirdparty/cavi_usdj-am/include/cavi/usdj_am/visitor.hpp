/**************************************************************************/
/* visitor.hpp                                                            */
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

#ifndef CAVI_USDJ_AM_VISITOR_HPP
#define CAVI_USDJ_AM_VISITOR_HPP

namespace cavi {
namespace usdj_am {

class Assignment;
struct ClassDeclaration;
class ClassDefinition;
class Declaration;
class Definition;
struct DefinitionStatement;
class Descriptor;
class ExternalReference;
class ExternalReferenceImport;
class File;
class ObjectDeclaration;
class ObjectDeclarationEntries;
class ObjectDeclarationList;
class ObjectDeclarationListValue;
struct ObjectDeclarations;
class ObjectValue;
class ReferenceFile;
struct Statement;
struct Value;
class VariantDefinition;
class VariantSet;

/// \brief An abstract base class for USDJ-AM node visitors.
class Visitor {
public:
    Visitor(Visitor const&) = delete;

    Visitor(Visitor&&) = default;

    Visitor& operator=(Visitor const&) = delete;

    Visitor& operator=(Visitor&&) = delete;

    virtual ~Visitor() = 0;

    virtual void visit(Assignment const&);

    virtual void visit(Assignment&&);

    virtual void visit(ClassDeclaration const&);

    virtual void visit(ClassDeclaration&&);

    virtual void visit(ClassDefinition const&);

    virtual void visit(ClassDefinition&&);

    virtual void visit(Declaration const&);

    virtual void visit(Declaration&&);

    virtual void visit(Definition const&);

    virtual void visit(Definition&&);

    virtual void visit(DefinitionStatement const&);

    virtual void visit(DefinitionStatement&&);

    virtual void visit(Descriptor const&);

    virtual void visit(Descriptor&&);

    virtual void visit(ExternalReference const&);

    virtual void visit(ExternalReference&&);

    virtual void visit(ExternalReferenceImport const&);

    virtual void visit(ExternalReferenceImport&&);

    virtual void visit(File const&);

    virtual void visit(File&&);

    virtual void visit(ObjectDeclaration const&);

    virtual void visit(ObjectDeclaration&&);

    virtual void visit(ObjectDeclarationEntries const&);

    virtual void visit(ObjectDeclarationEntries&&);

    virtual void visit(ObjectDeclarationList const&);

    virtual void visit(ObjectDeclarationList&&);

    virtual void visit(ObjectDeclarationListValue const&);

    virtual void visit(ObjectDeclarationListValue&&);

    virtual void visit(ObjectDeclarations const&);

    virtual void visit(ObjectDeclarations&&);

    virtual void visit(ObjectValue const&);

    virtual void visit(ObjectValue&&);

    virtual void visit(ReferenceFile const&);

    virtual void visit(ReferenceFile&&);

    virtual void visit(Statement const&);

    virtual void visit(Statement&&);

    virtual void visit(Value const&);

    virtual void visit(Value&&);

    virtual void visit(VariantDefinition const&);

    virtual void visit(VariantDefinition&&);

    virtual void visit(VariantSet const&);

    virtual void visit(VariantSet&&);

protected:
    Visitor() = default;
};

}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_VISITOR_HPP

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

    Visitor& operator=(Visitor&&) = default;

    virtual ~Visitor();

    virtual void visit(Assignment const&) = 0;

    virtual void visit(ClassDeclaration const&) = 0;

    virtual void visit(ClassDefinition const&) = 0;

    virtual void visit(Declaration const&) = 0;

    virtual void visit(Definition const&) = 0;

    virtual void visit(DefinitionStatement const&) = 0;

    virtual void visit(Descriptor const&) = 0;

    virtual void visit(ExternalReference const&) = 0;

    virtual void visit(ExternalReferenceImport const&) = 0;

    virtual void visit(File const&) = 0;

    virtual void visit(ObjectDeclaration const&) = 0;

    virtual void visit(ObjectDeclarationEntries const&) = 0;

    virtual void visit(ObjectDeclarationList const&) = 0;

    virtual void visit(ObjectDeclarationListValue const&) = 0;

    virtual void visit(ObjectDeclarations const&) = 0;

    virtual void visit(ObjectValue const&) = 0;

    virtual void visit(ReferenceFile const&) = 0;

    virtual void visit(Statement const&) = 0;

    virtual void visit(Value const&) = 0;

    virtual void visit(VariantDefinition const&) = 0;

    virtual void visit(VariantSet const&) = 0;

protected:
    Visitor() = default;
};

}  // namespace usdj_am
}  // namespace cavi

#endif  // CAVI_USDJ_AM_VISITOR_HPP

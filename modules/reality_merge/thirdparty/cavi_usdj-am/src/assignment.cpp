/**************************************************************************/
/* assignment.cpp                                                         */
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

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <string_view>
#include <typeinfo>

// third-party
extern "C" {

#include <automerge-c/automerge.h>
}

// local
#include "assignment.hpp"
#include "assignment_keyword.hpp"
#include "string_.hpp"
#include "visitor.hpp"

// export enum USDA_ValueType {
//     ExternalReference = 'externalReference',
//     ExternalReferenceImport = 'externalReferenceImport',
//     ExternalReferenceSrc = 'externalReferenceSrc',
//     ObjectValue = 'objectValue',
// }

// export enum AssignmentType {
//     Assignment = 'assignment',
// }
//
// // noinspection JSUnusedGlobalSymbols
// export enum USDA_AssignmentKeyword {
//     /**
//      * https://graphics.pixar.com/usd/release/tut_xforms.html
//      *
//      *
//      *
//      * Why prepend?
//      *
//      * You may have noticed the prepend operation in the reference statement above. Prepend means that, when this
//      layer
//      * is composed with others to populate the stage, the reference will be inserted before any references that might
//      * exist in weaker sublayers. This ensures that the contents of the reference will contribute stronger opinions
//      * than any reference arcs that might exist in other, weaker layers.
//      *
//      * In other words, prepend gives the intuitive result you’d expect when you apply one layer on top of another.
//      * This is what the UsdReferences API https://graphics.pixar.com/usd/release/api/class_usd_references.html will
//      create by default. You can specify other options with the position
//      * parameter, but this should rarely be necessary.
//      *
//      *
//      */
//     Prepend = 'prepend',
//     Add = 'add',
//     Append = 'append',
//     Delete = 'delete',
// }
//
// export type USDA_Array<T extends USDA_ValueTypes>
//     = T[]
//
// export interface USDA_ReferenceFile {
//     type: USDA_ValueType.ExternalReferenceSrc;
//     src: string;
//     descriptor: USDA_Descriptor | null;
// }
//
// export interface USDA_ExternalReferenceImport {
//     type: USDA_ValueType.ExternalReferenceImport;
//     importPath: string;
//     field: string | null;
// }
//
// export interface USDA_ExternalReference {
//     type: USDA_ValueType.ExternalReference;
//     referenceFile: USDA_ReferenceFile;
//     toImport: null | USDA_ExternalReferenceImport;
// }
//
// // "varying" / "uniform" / "custom" / "prepend" / "append" / "delete" / "add"
// // noinspection JSUnusedGlobalSymbols
// export enum USDA_DeclarationKeyword {
//     Varying = 'varying',
//     Uniform = 'uniform',
//     Custom = 'custom',
//     Prepend = 'prepend',
//     Append = 'append',
//     Delete = 'delete',
//     Add = 'add',
// }
//
// export type USDA_TypeReference = string;
//
// export type USDA_Reference = string;
//
// export type USDA_ObjectDeclaration<T extends USDA_ValueTypes> = {
//     keyword: USDA_DeclarationKeyword | null;
//     defineType: USDA_TypeReference;
//     reference: USDA_Reference;
//     value: T;
// };
//
// export type USDA_ObjectDeclarationEntries<T extends USDA_ValueTypes> = {
//     type: 'objectDeclarationEntries',
//     values: USDA_ObjectDeclaration<T>[],
///    values: {
///        keyword: USDA_DeclarationKeyword | null;
///        defineType: USDA_TypeReference;
///        reference: USDA_Reference;
///        value: T;
///    }[],
// };
//
// export type USDA_ObjectDeclarationList<T extends USDA_ValueTypes> = {
//     type: 'objectDeclarationList',
//     values: {
//         index: number,
//         value: T
//     }[],
// };
//
// export type USDA_ObjectDeclarations<T extends USDA_ValueTypes> =
//     T extends (infer TArrayItem)[] ?
//         USDA_ObjectDeclarationList<TArrayItem> :
//         USDA_ObjectDeclarationList<T>
//       | USDA_ObjectDeclarationEntries<T>;
//
// export type USDA_ObjectValue<T extends USDA_ValueTypes> = {
//     type: USDA_ValueType.ObjectValue;
//     declarations: USDA_ObjectDeclarations<T>;
// };
//
// export type USDA_ValueTypes =
//     | string
//     | boolean
//     | number
//     | USDA_Array<any>
//     | Readonly<USDA_Array<any>>
//     | USDA_ExternalReferenceImport
//     | USDA_ExternalReference
//     | USDA_ObjectValue<any>
//     | null
//     | undefined
//

namespace {

using cavi::usdj_am::AssignmentType;

static std::map<std::string_view, AssignmentType> const TAGS = {{"assignment", AssignmentType::ASSIGNMENT}};

}  // namespace

namespace cavi {
namespace usdj_am {

Assignment::Assignment(AMdoc const* const document, AMitem const* const map_object) : Node(document, map_object, 4) {
    check_enum_property("type", AssignmentType::ASSIGNMENT);
}

void Assignment::accept(Visitor& visitor) const {
    visitor.visit(*this);
}

String Assignment::get_identifier() const {
    return get_object_property<String>("identifier");
}

std::optional<AssignmentKeyword> Assignment::get_keyword() const {
    return get_nullable_enum_property<AssignmentKeyword>("keyword");
}

Value Assignment::get_value() const {
    return get_object_property<Value>("value");
}

std::istream& operator>>(std::istream& is, AssignmentType& out) {
    std::string token;
    if (is >> token) {
        auto const match = TAGS.find(token);
        if (match != TAGS.end()) {
            out = match->second;
            return is;
        }
    }
    is.setstate(std::ios::failbit);
    return is;
}

std::ostream& operator<<(std::ostream& os, AssignmentType const& in) {
    for (auto item : TAGS) {
        if (item.second == in) {
            os << item.first;
            return os;
        }
    }
    os << "???";
    return os;
}

}  // namespace usdj_am
}  // namespace cavi

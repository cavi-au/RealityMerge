/**************************************************************************/
/* usdj_material.cpp                                                      */
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

// third-party
#include <cavi/usdj_am/definition.hpp>

// local
#include "usdj_color_extractor.h"
#include "usdj_material.h"

void UsdjMaterial::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_albedo", "albedo"), &UsdjMaterial::set_albedo);
    ClassDB::bind_method(D_METHOD("get_albedo"), &UsdjMaterial::get_albedo);
    ClassDB::bind_method(D_METHOD("set_emission", "emission"), &UsdjMaterial::set_emission);
    ClassDB::bind_method(D_METHOD("get_emission"), &UsdjMaterial::get_emission);

    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "albedo", PROPERTY_HINT_COLOR_NO_ALPHA), "set_albedo", "get_albedo");
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "emission", PROPERTY_HINT_COLOR_NO_ALPHA), "set_emission",
                 "get_emission");
}

UsdjMaterial::UsdjMaterial(std::shared_ptr<cavi::usdj_am::Definition> const& p_definition)
    : m_definition{p_definition} {
    std::ostringstream args;
    if (!p_definition) {
        args << "p_definition == nullptr";
    }
    if (!args.str().empty()) {
        std::ostringstream msg;
        msg << typeid(*this).name() << "::" << __func__ << "(" << args.str() << ")";
        ERR_FAIL_MSG(msg.str().c_str());
    }
    set_emission(Color(0, 0, 0, 1));
}

UsdjMaterial::~UsdjMaterial() {
    RS::get_singleton()->material_set_shader(_get_material(), RID());
}

void UsdjMaterial::set_albedo(Color p_albedo) {
    std::ostringstream msg;
    msg << typeid(*this).name() << "::" << __func__ << "(p_albedo) is not implemented.";
    ERR_FAIL_MSG(msg.str().c_str());
}

Color UsdjMaterial::get_albedo() const {
    Color albedo{1, 1, 1, 1};
    auto const definition = m_definition.lock();
    if (definition)
        albedo = UsdjColorExtractor{definition}().value_or(albedo);
    return albedo;
}

void UsdjMaterial::set_emission(Color p_emission) {
    emission = p_emission;
    RS::get_singleton()->material_set_param(_get_material(), "emission", emission);
}

Color UsdjMaterial::get_emission() const {
    return emission;
}

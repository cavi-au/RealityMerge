/**************************************************************************/
/* usdj_material.h                                                        */
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

#ifndef REALITY_MERGE_USDJ_MATERIAL_H
#define REALITY_MERGE_USDJ_MATERIAL_H

#include <cstdint>
#include <memory>
#include <optional>

// regional
#include "scene/resources/material.h"

namespace cavi {
namespace usdj_am {

class Definition;

}  // namespace usdj_am
}  // namespace cavi

class UsdjMaterial : public Material {
    GDCLASS(UsdjMaterial, Material);

private:
    std::weak_ptr<cavi::usdj_am::Definition> m_definition;
    Color emission = Color(0, 0, 0, 0);

protected:
    static void _bind_methods();

public:
    UsdjMaterial(std::shared_ptr<cavi::usdj_am::Definition> const& p_definition);

    virtual ~UsdjMaterial();

    void set_albedo(Color p_color);
    Color get_albedo() const;

    void set_emission(Color p_color);
    Color get_emission() const;
};

#endif  // REALITY_MERGE_USDJ_MATERIAL_H

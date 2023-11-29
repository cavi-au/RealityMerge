/**************************************************************************/
/* usdj_box_shape_3d.h                                                    */
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

#ifndef REALITY_MERGE_USDJ_BOX_SHAPE_3D_H
#define REALITY_MERGE_USDJ_BOX_SHAPE_3D_H

#include <memory>

// regional
#include <scene/resources/shape_3d.h>

namespace cavi {
namespace usdj_am {

class Definition;

}  // namespace usdj_am
}  // namespace cavi

/// \brief A shape defined by a USD Cube prim.
class UsdjBoxShape3D : public Shape3D {
    GDCLASS(UsdjBoxShape3D, Shape3D);
    // Vector3 size;
    std::weak_ptr<cavi::usdj_am::Definition> m_definition;

protected:
    static void _bind_methods();
#ifndef DISABLE_DEPRECATED
    bool _set(const StringName& p_name, const Variant& p_value);
    bool _get(const StringName& p_name, Variant& r_property) const;
#endif  // DISABLE_DEPRECATED

    virtual void _update_shape() override;

public:
    void set_size(const Vector3& p_size);
    Vector3 get_size() const;

    virtual Vector<Vector3> get_debug_mesh_lines() const override;
    virtual real_t get_enclosing_radius() const override;

    UsdjBoxShape3D(std::shared_ptr<cavi::usdj_am::Definition> const& p_definition);
};

#endif  // REALITY_MERGE_USDJ_BOX_SHAPE_3D_H

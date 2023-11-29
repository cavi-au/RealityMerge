/**************************************************************************/
/* usdj_box_mesh.h                                                        */
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

#ifndef REALITY_MERGE_USDJ_BOX_MESH_H
#define REALITY_MERGE_USDJ_BOX_MESH_H

#include <memory>

// regional
#include <scene/resources/primitive_meshes.h>

namespace cavi {
namespace usdj_am {

class Definition;

}  // namespace usdj_am
}  // namespace cavi

/// \brief A mesh defined by a USD Cube prim.
class UsdjBoxMesh : public PrimitiveMesh {
    GDCLASS(UsdjBoxMesh, PrimitiveMesh);

private:
    /// Vector3 size = Vector3(1, 1, 1);
    std::weak_ptr<cavi::usdj_am::Definition> m_definition;
    int subdivide_w = 0;
    int subdivide_h = 0;
    int subdivide_d = 0;

protected:
    static void _bind_methods();
    virtual void _create_mesh_array(Array& p_arr) const override;

    virtual void _update_lightmap_size() override;

public:
    static void create_mesh_array(Array& p_arr,
                                  Vector3 size,
                                  int subdivide_w = 0,
                                  int subdivide_h = 0,
                                  int subdivide_d = 0,
                                  bool p_add_uv2 = false,
                                  const float p_uv2_padding = 1.0);

    void set_size(const Vector3& p_size);
    Vector3 get_size() const;

    void set_subdivide_width(const int p_divisions);
    int get_subdivide_width() const;

    void set_subdivide_height(const int p_divisions);
    int get_subdivide_height() const;

    void set_subdivide_depth(const int p_divisions);
    int get_subdivide_depth() const;

    /// \throws std::invalid_argument
    UsdjBoxMesh(std::shared_ptr<cavi::usdj_am::Definition> const& p_definition);
};

#endif  // REALITY_MERGE_USDJ_BOX_MESH_H

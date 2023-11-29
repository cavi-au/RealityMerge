/**************************************************************************/
/* usdj_box_mesh.cpp                                                      */
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

// regional
#include <core/object/ref_counted.h>
#include <core/os/memory.h>

// local
#include "usdj_box_mesh.h"
#include "usdj_box_size_extractor.h"
#include "usdj_material.h"

void UsdjBoxMesh::_update_lightmap_size() {
    if (get_add_uv2()) {
        // size must have changed, update lightmap size hint
        Size2i _lightmap_size_hint;
        float texel_size = get_lightmap_texel_size();
        float padding = get_uv2_padding();

        Vector3 const size = get_size();
        float width = (size.x + size.z) / texel_size;
        float length = (size.y + size.y + MAX(size.x, size.z)) / texel_size;

        _lightmap_size_hint.x = MAX(1.0, width) + 2.0 * padding;
        _lightmap_size_hint.y = MAX(1.0, length) + 3.0 * padding;

        set_lightmap_size_hint(_lightmap_size_hint);
    }
}

void UsdjBoxMesh::_create_mesh_array(Array& p_arr) const {
    // Note about padding, with our box each face of the box faces a different direction so we want a seam
    // around every face. We thus add our padding to the right and bottom of each face.
    // With 3 faces along the width and 2 along the height of the texture we need to adjust our scale
    // accordingly.
    bool _add_uv2 = get_add_uv2();
    float texel_size = get_lightmap_texel_size();
    float _uv2_padding = get_uv2_padding() * texel_size;

    UsdjBoxMesh::create_mesh_array(p_arr, get_size(), subdivide_w, subdivide_h, subdivide_d, _add_uv2, _uv2_padding);
}

void UsdjBoxMesh::create_mesh_array(Array& p_arr,
                                    Vector3 size,
                                    int subdivide_w,
                                    int subdivide_h,
                                    int subdivide_d,
                                    bool p_add_uv2,
                                    const float p_uv2_padding) {
    int i, j, prevrow, thisrow, point;
    float x, y, z;
    float onethird = 1.0 / 3.0;
    float twothirds = 2.0 / 3.0;

    // Only used if we calculate UV2
    // TODO this could be improved by changing the order depending on which side is the longest (basically the below
    // works best if size.y is the longest)
    float total_h = (size.x + size.z + (2.0 * p_uv2_padding));
    float padding_h = p_uv2_padding / total_h;
    float width_h = size.x / total_h;
    float depth_h = size.z / total_h;
    float total_v = (size.y + size.y + MAX(size.x, size.z) + (3.0 * p_uv2_padding));
    float padding_v = p_uv2_padding / total_v;
    float width_v = size.x / total_v;
    float height_v = size.y / total_v;
    float depth_v = size.z / total_v;

    Vector3 start_pos = size * -0.5;

    // set our bounding box

    Vector<Vector3> points;
    Vector<Vector3> normals;
    Vector<float> tangents;
    Vector<Vector2> uvs;
    Vector<Vector2> uv2s;
    Vector<int> indices;
    point = 0;

#define ADD_TANGENT(m_x, m_y, m_z, m_d) \
    tangents.push_back(m_x);            \
    tangents.push_back(m_y);            \
    tangents.push_back(m_z);            \
    tangents.push_back(m_d);

    // front + back
    y = start_pos.y;
    thisrow = point;
    prevrow = 0;
    for (j = 0; j <= subdivide_h + 1; j++) {
        float v = j;
        float v2 = v / (subdivide_w + 1.0);
        v /= (2.0 * (subdivide_h + 1.0));

        x = start_pos.x;
        for (i = 0; i <= subdivide_w + 1; i++) {
            float u = i;
            float u2 = u / (subdivide_w + 1.0);
            u /= (3.0 * (subdivide_w + 1.0));

            // front
            points.push_back(Vector3(x, -y, -start_pos.z));  // double negative on the Z!
            normals.push_back(Vector3(0.0, 0.0, 1.0));
            ADD_TANGENT(1.0, 0.0, 0.0, 1.0);
            uvs.push_back(Vector2(u, v));
            if (p_add_uv2) {
                uv2s.push_back(Vector2(u2 * width_h, v2 * height_v));
            }
            point++;

            // back
            points.push_back(Vector3(-x, -y, start_pos.z));
            normals.push_back(Vector3(0.0, 0.0, -1.0));
            ADD_TANGENT(-1.0, 0.0, 0.0, 1.0);
            uvs.push_back(Vector2(twothirds + u, v));
            if (p_add_uv2) {
                uv2s.push_back(Vector2(u2 * width_h, height_v + padding_v + (v2 * height_v)));
            }
            point++;

            if (i > 0 && j > 0) {
                int i2 = i * 2;

                // front
                indices.push_back(prevrow + i2 - 2);
                indices.push_back(prevrow + i2);
                indices.push_back(thisrow + i2 - 2);
                indices.push_back(prevrow + i2);
                indices.push_back(thisrow + i2);
                indices.push_back(thisrow + i2 - 2);

                // back
                indices.push_back(prevrow + i2 - 1);
                indices.push_back(prevrow + i2 + 1);
                indices.push_back(thisrow + i2 - 1);
                indices.push_back(prevrow + i2 + 1);
                indices.push_back(thisrow + i2 + 1);
                indices.push_back(thisrow + i2 - 1);
            }

            x += size.x / (subdivide_w + 1.0);
        }

        y += size.y / (subdivide_h + 1.0);
        prevrow = thisrow;
        thisrow = point;
    }

    // left + right
    y = start_pos.y;
    thisrow = point;
    prevrow = 0;
    for (j = 0; j <= (subdivide_h + 1); j++) {
        float v = j;
        float v2 = v / (subdivide_h + 1.0);
        v /= (2.0 * (subdivide_h + 1.0));

        z = start_pos.z;
        for (i = 0; i <= (subdivide_d + 1); i++) {
            float u = i;
            float u2 = u / (subdivide_d + 1.0);
            u /= (3.0 * (subdivide_d + 1.0));

            // right
            points.push_back(Vector3(-start_pos.x, -y, -z));
            normals.push_back(Vector3(1.0, 0.0, 0.0));
            ADD_TANGENT(0.0, 0.0, -1.0, 1.0);
            uvs.push_back(Vector2(onethird + u, v));
            if (p_add_uv2) {
                uv2s.push_back(Vector2(width_h + padding_h + (u2 * depth_h), v2 * height_v));
            }
            point++;

            // left
            points.push_back(Vector3(start_pos.x, -y, z));
            normals.push_back(Vector3(-1.0, 0.0, 0.0));
            ADD_TANGENT(0.0, 0.0, 1.0, 1.0);
            uvs.push_back(Vector2(u, 0.5 + v));
            if (p_add_uv2) {
                uv2s.push_back(Vector2(width_h + padding_h + (u2 * depth_h), height_v + padding_v + (v2 * height_v)));
            }
            point++;

            if (i > 0 && j > 0) {
                int i2 = i * 2;

                // right
                indices.push_back(prevrow + i2 - 2);
                indices.push_back(prevrow + i2);
                indices.push_back(thisrow + i2 - 2);
                indices.push_back(prevrow + i2);
                indices.push_back(thisrow + i2);
                indices.push_back(thisrow + i2 - 2);

                // left
                indices.push_back(prevrow + i2 - 1);
                indices.push_back(prevrow + i2 + 1);
                indices.push_back(thisrow + i2 - 1);
                indices.push_back(prevrow + i2 + 1);
                indices.push_back(thisrow + i2 + 1);
                indices.push_back(thisrow + i2 - 1);
            }

            z += size.z / (subdivide_d + 1.0);
        }

        y += size.y / (subdivide_h + 1.0);
        prevrow = thisrow;
        thisrow = point;
    }

    // top + bottom
    z = start_pos.z;
    thisrow = point;
    prevrow = 0;
    for (j = 0; j <= (subdivide_d + 1); j++) {
        float v = j;
        float v2 = v / (subdivide_d + 1.0);
        v /= (2.0 * (subdivide_d + 1.0));

        x = start_pos.x;
        for (i = 0; i <= (subdivide_w + 1); i++) {
            float u = i;
            float u2 = u / (subdivide_w + 1.0);
            u /= (3.0 * (subdivide_w + 1.0));

            // top
            points.push_back(Vector3(-x, -start_pos.y, -z));
            normals.push_back(Vector3(0.0, 1.0, 0.0));
            ADD_TANGENT(-1.0, 0.0, 0.0, 1.0);
            uvs.push_back(Vector2(onethird + u, 0.5 + v));
            if (p_add_uv2) {
                uv2s.push_back(Vector2(u2 * width_h, ((height_v + padding_v) * 2.0) + (v2 * depth_v)));
            }
            point++;

            // bottom
            points.push_back(Vector3(x, start_pos.y, -z));
            normals.push_back(Vector3(0.0, -1.0, 0.0));
            ADD_TANGENT(1.0, 0.0, 0.0, 1.0);
            uvs.push_back(Vector2(twothirds + u, 0.5 + v));
            if (p_add_uv2) {
                uv2s.push_back(
                    Vector2(width_h + padding_h + (u2 * depth_h), ((height_v + padding_v) * 2.0) + (v2 * width_v)));
            }
            point++;

            if (i > 0 && j > 0) {
                int i2 = i * 2;

                // top
                indices.push_back(prevrow + i2 - 2);
                indices.push_back(prevrow + i2);
                indices.push_back(thisrow + i2 - 2);
                indices.push_back(prevrow + i2);
                indices.push_back(thisrow + i2);
                indices.push_back(thisrow + i2 - 2);

                // bottom
                indices.push_back(prevrow + i2 - 1);
                indices.push_back(prevrow + i2 + 1);
                indices.push_back(thisrow + i2 - 1);
                indices.push_back(prevrow + i2 + 1);
                indices.push_back(thisrow + i2 + 1);
                indices.push_back(thisrow + i2 - 1);
            }

            x += size.x / (subdivide_w + 1.0);
        }

        z += size.z / (subdivide_d + 1.0);
        prevrow = thisrow;
        thisrow = point;
    }

    p_arr[RS::ARRAY_VERTEX] = points;
    p_arr[RS::ARRAY_NORMAL] = normals;
    p_arr[RS::ARRAY_TANGENT] = tangents;
    p_arr[RS::ARRAY_TEX_UV] = uvs;
    if (p_add_uv2) {
        p_arr[RS::ARRAY_TEX_UV2] = uv2s;
    }
    p_arr[RS::ARRAY_INDEX] = indices;
}

void UsdjBoxMesh::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_size", "size"), &UsdjBoxMesh::set_size);
    ClassDB::bind_method(D_METHOD("get_size"), &UsdjBoxMesh::get_size);

    ClassDB::bind_method(D_METHOD("set_subdivide_width", "subdivide"), &UsdjBoxMesh::set_subdivide_width);
    ClassDB::bind_method(D_METHOD("get_subdivide_width"), &UsdjBoxMesh::get_subdivide_width);
    ClassDB::bind_method(D_METHOD("set_subdivide_height", "divisions"), &UsdjBoxMesh::set_subdivide_height);
    ClassDB::bind_method(D_METHOD("get_subdivide_height"), &UsdjBoxMesh::get_subdivide_height);
    ClassDB::bind_method(D_METHOD("set_subdivide_depth", "divisions"), &UsdjBoxMesh::set_subdivide_depth);
    ClassDB::bind_method(D_METHOD("get_subdivide_depth"), &UsdjBoxMesh::get_subdivide_depth);

    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "size", PROPERTY_HINT_NONE, "suffix:m"), "set_size", "get_size");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "subdivide_width", PROPERTY_HINT_RANGE, "0,100,1,or_greater"),
                 "set_subdivide_width", "get_subdivide_width");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "subdivide_height", PROPERTY_HINT_RANGE, "0,100,1,or_greater"),
                 "set_subdivide_height", "get_subdivide_height");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "subdivide_depth", PROPERTY_HINT_RANGE, "0,100,1,or_greater"),
                 "set_subdivide_depth", "get_subdivide_depth");
}

void UsdjBoxMesh::set_size(const Vector3& p_size) {
    // size = p_size;
    // _update_lightmap_size();
    // _request_update();
    std::ostringstream msg;
    msg << typeid(*this).name() << "::" << __func__ << "(p_size) is not implemented.";
    ERR_FAIL_MSG(msg.str().c_str());
}

Vector3 UsdjBoxMesh::get_size() const {
    Vector3 size{1, 1, 1};
    auto const definition = m_definition.lock();
    if (definition)
        size = UsdjBoxSizeExtractor{definition}().value_or(size);
    return size;
}

void UsdjBoxMesh::set_subdivide_width(const int p_divisions) {
    subdivide_w = p_divisions > 0 ? p_divisions : 0;
    _request_update();
}

int UsdjBoxMesh::get_subdivide_width() const {
    return subdivide_w;
}

void UsdjBoxMesh::set_subdivide_height(const int p_divisions) {
    subdivide_h = p_divisions > 0 ? p_divisions : 0;
    _request_update();
}

int UsdjBoxMesh::get_subdivide_height() const {
    return subdivide_h;
}

void UsdjBoxMesh::set_subdivide_depth(const int p_divisions) {
    subdivide_d = p_divisions > 0 ? p_divisions : 0;
    _request_update();
}

int UsdjBoxMesh::get_subdivide_depth() const {
    return subdivide_d;
}

UsdjBoxMesh::UsdjBoxMesh(std::shared_ptr<cavi::usdj_am::Definition> const& p_definition) : m_definition(p_definition) {
    std::ostringstream args;
    if (!p_definition) {
        args << "p_definition == nullptr";
    } else {
        set_material(Ref<Material>(memnew(UsdjMaterial{p_definition})));
    }
    if (!args.str().empty()) {
        std::ostringstream msg;
        msg << typeid(*this).name() << "::" << __func__ << "(" << args.str() << ")";
        ERR_FAIL_MSG(msg.str().c_str());
    }
}

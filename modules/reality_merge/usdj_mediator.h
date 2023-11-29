/**************************************************************************/
/* usdj_mediator.h                                                        */
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

#ifndef REALITY_MERGE_USDJ_MEDIATOR_H
#define REALITY_MERGE_USDJ_MEDIATOR_H

// regional
#include <core/object/ref_counted.h>
#include <core/string/ustring.h>
#include <core/variant/variant.h>
#include <scene/3d/node_3d.h>

// local
#include "automerge_resource.h"

class UsdjMediator : public Node3D {
    GDCLASS(UsdjMediator, Node3D);

public:
    UsdjMediator() = default;

    ~UsdjMediator();

    PackedStringArray get_configuration_warnings() const override;

    /// \returns An AutomergeResource document resource containing a scene description
    ///          in USDJ format or null.
    Ref<AutomergeResource> get_automerge_resource() const;

    /// \returns The path to a scene description within an AutomergeResource document
    ///          resource where "/" is the root and "" is invalid.
    String get_usdj_path() const;

    /// \param[in] p_usdj_am An AutomergeResource document resource containing a scene
    ///                      description in USDJ format.
    void set_automerge_resource(Ref<AutomergeResource> const& p_automerge_resource);

    /// \param[in] p_scene_path The path to a scene description within an
    ///                         AutomergeResource document resource.
    void set_usdj_path(String const& p_usdj_path);

protected:
    static void _bind_methods();

    void _update_bodies();

private:
    Ref<AutomergeResource> m_automerge_resource;
    String m_usdj_path;
};

#endif  // REALITY_MERGE_USDJ_MEDIATOR_H

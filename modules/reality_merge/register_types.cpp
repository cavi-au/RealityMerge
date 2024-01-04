/**************************************************************************/
/* register_types.cpp                                                     */
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

// regional
#include <core/object/class_db.h>
#include <core/object/ref_counted.h>

// local
#include "automerge_resource.h"
#include "register_types.h"
#include "usdj_mediator.h"
#include "usdj_static_body_3d.h"

static Ref<ResourceFormatLoaderAutomerge> resource_loader_automerge;
static Ref<ResourceFormatSaverAutomerge> resource_saver_automerge;

void initialize_reality_merge_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
    GDREGISTER_CLASS(AutomergeResource);
    GDREGISTER_CLASS(UsdjMediator);
    GDREGISTER_CLASS(UsdjStaticBody3D);

    resource_loader_automerge.instantiate();
    ResourceLoader::add_resource_format_loader(resource_loader_automerge, true);

    resource_saver_automerge.instantiate();
    ResourceSaver::add_resource_format_saver(resource_saver_automerge);
}

void uninitialize_reality_merge_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
    ResourceLoader::remove_resource_format_loader(resource_loader_automerge);
    resource_loader_automerge.unref();

    ResourceSaver::remove_resource_format_saver(resource_saver_automerge);
    resource_saver_automerge.unref();
}

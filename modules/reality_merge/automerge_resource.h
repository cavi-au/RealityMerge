/**************************************************************************/
/* automerge_resource.h                                                   */
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

#ifndef REALITY_MERGE_AUTOMERGE_RESOURCE_H
#define REALITY_MERGE_AUTOMERGE_RESOURCE_H

#include <cstdint>
#include <functional>
#include <optional>

// third-party
#include <cavi/usdj_am/utils/document.hpp>

// regional
#include <core/error/error_list.h>
#include <core/io/resource.h>
#include <core/io/resource_loader.h>
#include <core/io/resource_saver.h>
#include <core/string/ustring.h>
#include <core/templates/list.h>
#include <core/templates/vector.h>

class AutomergeResource : public Resource {
    GDCLASS(AutomergeResource, Resource);

public:
    AutomergeResource() = default;

    ~AutomergeResource();

    std::optional<std::reference_wrapper<cavi::usdj_am::utils::Document const>> get_document() const;

    Error load(Vector<std::uint8_t> const& p_data, String& p_err_msg);

protected:
    static void _bind_methods();

private:
    std::optional<cavi::usdj_am::utils::Document> m_document;
};

class ResourceFormatLoaderAutomerge : public ResourceFormatLoader {
public:
    virtual Ref<Resource> load(String const& p_path,
                               String const& p_original_path = "",
                               Error* r_error = nullptr,
                               bool p_use_sub_threads = false,
                               float* r_progress = nullptr,
                               CacheMode p_cache_mode = CACHE_MODE_REUSE);
    virtual bool handles_type(String const& p_type) const;
    virtual void get_recognized_extensions(List<String>* p_extensions) const;
    virtual String get_resource_type(String const& p_path) const;
};

class ResourceFormatSaverAutomerge : public ResourceFormatSaver {
public:
    virtual void get_recognized_extensions(Ref<Resource> const& p_resource, List<String>* p_extensions) const;
    virtual bool recognize(Ref<Resource> const& p_resource) const;
    virtual Error save(Ref<Resource> const& p_resource, String const& p_path, uint32_t p_flags = 0);
};

#endif  // REALITY_MERGE_AUTOMERGE_RESOURCE_H

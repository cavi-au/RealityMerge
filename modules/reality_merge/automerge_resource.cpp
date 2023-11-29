/**************************************************************************/
/* automerge_resource.cpp                                                 */
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

#include <filesystem>
#include <stdexcept>

// regional
#include <core/config/project_settings.h>
#include <core/io/file_access.h>

// local
#include "automerge_resource.h"

namespace {

char const* CLASS_NAME = "AutomergeResource";
char const* FILE_EXT = "automerge";
}  // namespace

// AutomergeResource
void AutomergeResource::_bind_methods() {}

AutomergeResource::~AutomergeResource() {}

std::optional<std::reference_wrapper<cavi::usdj_am::utils::Document const>> AutomergeResource::get_document() const {
    if (m_document)
        return std::cref(*m_document);
    return std::nullopt;
}

Error AutomergeResource::load(Vector<std::uint8_t> const& p_data, String& p_err_msg) {
    using cavi::usdj_am::utils::Document;

    auto outcome = Error::OK;
    p_err_msg.clear();
    try {
        m_document.emplace(Document::load(p_data.ptr(), p_data.size()));
    } catch (std::invalid_argument const& thrown) {
        outcome = Error::ERR_INVALID_PARAMETER;
        p_err_msg = thrown.what();
    } catch (std::runtime_error const& thrown) {
        outcome = Error::FAILED;
        p_err_msg = thrown.what();
    }
    emit_changed();
    return outcome;
}

// ResourceFormatLoaderAutomerge
void ResourceFormatLoaderAutomerge::get_recognized_extensions(List<String>* p_extensions) const {
    p_extensions->push_back(FILE_EXT);
}

String ResourceFormatLoaderAutomerge::get_resource_type(String const& p_path) const {
    return (p_path.get_extension().to_lower() == FILE_EXT) ? CLASS_NAME : "";
}

bool ResourceFormatLoaderAutomerge::handles_type(String const& p_type) const {
    return (p_type == CLASS_NAME);
}

Ref<Resource> ResourceFormatLoaderAutomerge::load(String const& p_path,
                                                  String const& p_original_path,
                                                  Error* r_error,
                                                  bool p_use_sub_threads,
                                                  float* r_progress,
                                                  CacheMode p_cache_mode) {
    if (r_error) {
        *r_error = Error::ERR_FILE_CANT_OPEN;
    }
    if (!FileAccess::exists(p_path)) {
        *r_error = ERR_FILE_NOT_FOUND;
        return Ref<Resource>();
    }

    Ref<AutomergeResource> automerge_resource;
    automerge_resource.instantiate();

    Error file_error;
    Vector<std::uint8_t> const bytes = FileAccess::get_file_as_bytes(p_path, &file_error);
    if (file_error != Error::OK) {
        String error_msg = "Error reading file at \"" + p_path + "\".";
        if (r_error) {
            *r_error = file_error;
        }
        ERR_PRINT(error_msg);
        return Ref<Resource>();
    }
    String load_error_msg;
    auto load_error = automerge_resource->load(bytes, load_error_msg);
    if (load_error != Error::OK) {
        String error_msg = "Error loading file at \"" + p_path + "\": " + load_error_msg;
        if (r_error) {
            *r_error = load_error;
        }
        ERR_PRINT(error_msg);
        return Ref<Resource>();
    }
    if (r_error) {
        *r_error = OK;
    }
    return automerge_resource;
}

// ResourceFormatSaverAutomerge
void ResourceFormatSaverAutomerge::get_recognized_extensions(Ref<Resource> const& p_resource,
                                                             List<String>* p_extensions) const {
    Ref<AutomergeResource> automerge_resource = p_resource;
    if (automerge_resource.is_valid() && automerge_resource->get_document()) {
        // The resource and the document it contains are both valid.
        p_extensions->push_back(FILE_EXT);
    }
}

bool ResourceFormatSaverAutomerge::recognize(Ref<Resource> const& p_resource) const {
    return p_resource->get_class_name() == CLASS_NAME;
}

Error ResourceFormatSaverAutomerge::save(Ref<Resource> const& p_resource, String const& p_path, uint32_t p_flags) {
    using std::filesystem::path;

    Ref<AutomergeResource> automerge_resource = p_resource;
    ERR_FAIL_COND_V(!(automerge_resource.is_valid() && automerge_resource->get_document()),
                    Error::ERR_INVALID_PARAMETER);
    // Convert the project-relative path into an absolute one.
    auto const global_path = ProjectSettings::get_singleton()->globalize_path(p_path);
    auto const buffer = global_path.to_utf8_buffer();
    path const filename{std::string{reinterpret_cast<std::string::const_pointer>(buffer.ptr()),
                                    static_cast<std::string::size_type>(buffer.size())}};
    try {
        automerge_resource->get_document()->get().save(filename);
    } catch (std::invalid_argument const&) {
        auto const err = Error::ERR_INVALID_PARAMETER;
        ERR_FAIL_COND_V_MSG(err, err, "Cannot save file \"" + p_path + "\".");
    }
    return Error::OK;
}

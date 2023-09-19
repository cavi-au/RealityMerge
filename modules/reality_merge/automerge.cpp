/**************************************************************************/
/* automerge.cpp                                                          */
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

#include <algorithm>
#include <memory>
#include <stdexcept>

// third-party
extern "C" {

#include <automerge-c/automerge.h>
#include <automerge-c/utils/stack.h>
#include <automerge-c/utils/stack_callback_data.h>
#include <automerge-c/utils/string.h>
}

// regional
#include <core/io/file_access.h>

// local
#include "automerge.h"
#include "exception_callback.h"

namespace {
char const* CLASS_NAME = "Automerge";
char const* FILE_EXT = "automerge";
}  // namespace

// Automerge
void Automerge::_bind_methods() {}

Automerge::Automerge() : m_document{nullptr}, m_stack{nullptr} {}

Automerge::~Automerge() {
    AMstackFree(&m_stack);
}

Error Automerge::load(Vector<std::uint8_t> const& p_data, String& p_err_msg) {
    auto outcome = Error::OK;
    p_err_msg.clear();
    try {
        AMitem* const item =
            AMstackItem(&m_stack, AMload(p_data.ptr(), p_data.size()), exc_cb, AMexpect(AM_VAL_TYPE_DOC));
        AMitemToDoc(item, &m_document);
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

bool operator<(Automerge const& lhs, Automerge const& rhs) {
    return lhs.m_document < rhs.m_document;
}

bool operator==(Automerge const& lhs, Automerge const& rhs) {
    return (lhs.m_document == rhs.m_document) || AMequal(lhs.m_document, rhs.m_document);
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

    Ref<Automerge> automerge;
    automerge.instantiate();

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
    auto load_error = automerge->load(bytes, load_error_msg);
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
    return automerge;
}

// ResourceFormatSaverAutomerge
void ResourceFormatSaverAutomerge::get_recognized_extensions(Ref<Resource> const& p_resource,
                                                             List<String>* p_extensions) const {
    Ref<Automerge> automerge = p_resource;
    if (automerge.is_valid() && automerge->operator AMdoc*()) {
        // The resource and the document it contains are both valid.
        p_extensions->push_back(FILE_EXT);
    }
}

bool ResourceFormatSaverAutomerge::recognize(Ref<Resource> const& p_resource) const {
    return p_resource->get_class_name() == CLASS_NAME;
}

Error ResourceFormatSaverAutomerge::save(Ref<Resource> const& p_resource, String const& p_path, uint32_t p_flags) {
    Ref<Automerge> automerge = p_resource;
    AMdoc* const doc = automerge->operator AMdoc*();
    ERR_FAIL_COND_V(!(p_resource.is_valid() && doc), Error::ERR_INVALID_PARAMETER);
    std::unique_ptr<AMresult, void (*)(AMresult*)> const result{AMsave(doc), AMresultFree};
    if (AMresultStatus(result.get()) == AM_STATUS_OK) {
        AMitem const* const item = AMresultItem(result.get());
        AMbyteSpan bytes;
        if (AMitemToBytes(item, &bytes)) {
            Vector<std::uint8_t> buffer;
            buffer.resize(bytes.count);
            std::copy(bytes.src, bytes.src + bytes.count, buffer.begin());
            Error err;
            Ref<FileAccess> file = FileAccess::open(p_path, FileAccess::WRITE, &err);
            ERR_FAIL_COND_V_MSG(err, err, "Cannot save file '" + p_path + "'.");
            file->store_buffer(buffer);
            if (file->get_error() != OK && file->get_error() != Error::ERR_FILE_EOF) {
                return Error::ERR_CANT_CREATE;
            }
            return Error::OK;
        }
    }
    return Error::FAILED;
}

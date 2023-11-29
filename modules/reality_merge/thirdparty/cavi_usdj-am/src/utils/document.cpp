/**************************************************************************/
/* document.cpp                                                           */
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

#include <fstream>
#include <limits>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <typeinfo>
#include <vector>

// third-party
extern "C" {

#include <automerge-c/automerge.h>
#include <automerge-c/utils/enum_string.h>
}

// local
#include "utils/bytes.hpp"
#include "utils/document.hpp"

namespace {

using ::cavi::usdj_am::utils::Document;

void throw_on_error(std::string const& func_name, std::string const& args_msg) {
    if (!args_msg.empty()) {
        std::ostringstream what;
        what << typeid(Document).name() << "::" << func_name << "(" << args_msg << ")";
        throw std::invalid_argument(what.str());
    }
}

}  // namespace

namespace cavi {
namespace usdj_am {
namespace utils {

Document Document::load(std::uint8_t const* const src, std::size_t const count) {
    std::ostringstream args;
    ResultPtr result{AMload(src, count), AMresultFree};
    if (AMresultStatus(result.get()) != AM_STATUS_OK) {
        args << "AMresultError(AMload(src, count)) == \"" << from_bytes(AMresultError(result.get())) << "\"";
    }
    throw_on_error(__func__, args.str());
    return Document(std::move(result));
}

Document Document::load(std::filesystem::path const& filename) {
    ResultPtr result{nullptr, AMresultFree};
    std::ostringstream args;
    if (filename.empty()) {
        args << "filename == " << typeid(filename).name() << "{}";
    } else {
        std::ifstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);
        if (!file) {
            args << typeid(decltype(file)).name() << "(" << filename << ").fail()";
        } else {
            std::vector<std::uint8_t> buffer;
            std::streampos const size = file.tellg();
            buffer.reserve(size);
            file.seekg(0, std::ios::beg);
            auto const first = buffer.insert(buffer.end(), std::istreambuf_iterator<std::ifstream::char_type>(file),
                                             std::istreambuf_iterator<std::ifstream::char_type>());
            if (first == buffer.end()) {
                args << typeid(decltype(file)).name() << "(" << filename << ").read(..., ...) == " << std::boolalpha
                     << false << std::noboolalpha;
            } else {
                return load(buffer.data(), buffer.size());
            }
        }
    }
    throw_on_error(__func__, args.str());
    return Document(std::move(result));
}

Document::Document(ResultPtr&& result) : m_document{nullptr}, m_result{std::move(result)} {
    std::ostringstream args;
    if (!m_result) {
        args << "result.get() == nullptr";
    } else {
        AMvalType const val_type = AMitemValType(AMresultItem(m_result.get()));
        if (val_type != AM_VAL_TYPE_DOC) {
            args << "AMitemValType(AMresultItem(result.get())) == " << AMvalTypeToString(val_type);
        } else if (!AMitemToDoc(AMresultItem(m_result.get()), &m_document)) {
            args << "AMitemToDoc(AMresultItem(result.get()), ...) == " << std::boolalpha << false << std::noboolalpha;
        }
    }
    throw_on_error(__func__, args.str());
}

Item Document::get_item(std::string const& posix_path) const {
    namespace fs = std::filesystem;

    std::ostringstream args;
    std::optional<Item> item;
    std::istringstream iss;
    try {
        auto path = fs::path{posix_path, fs::path::format::generic_format};
        std::size_t count = 0;
        for (auto const& element : path) {
            if (!count++) {
                if (!element.has_root_directory()) {
                    args << typeid(path).name() << "{posix_path}.has_root_directory() == " << std::boolalpha << false
                         << std::noboolalpha;
                    break;
                } else {
                    item.emplace(std::move(get_item()));
                }
            } else {
                std::istringstream iss{element.string()};
                auto pos = std::numeric_limits<std::uint64_t>::max();
                if (iss >> pos) {
                    item.emplace(*item / pos);
                } else {
                    item.emplace(*item / iss.str());
                }
            }
        }
        if (!item) {
            args << "posix_path == \"\"";
        }
    } catch (std::exception const& thrown) {
        args << thrown.what();
    }
    throw_on_error(__func__, args.str());
    return *item;
}

std::size_t Document::save(std::filesystem::path const& filename) const {
    std::ostringstream args;
    std::size_t count = 0;
    ResultPtr const result{AMsave(m_document), AMresultFree};
    if (AMresultStatus(result.get()) != AM_STATUS_OK) {
        args << "AMresultError(AMsave(...)) == \"" << from_bytes(AMresultError(result.get())) << "\"";
    } else {
        AMitem const* const item = AMresultItem(result.get());
        AMbyteSpan bytes;
        if (!AMitemToBytes(item, &bytes)) {
            args << "AMitemToBytes(..., ...) == " << std::boolalpha << false << std::noboolalpha;
        } else {
            std::ofstream ofs{filename, std::ios::binary | std::ios::out};
            if (!ofs.is_open()) {
                args << typeid(decltype(ofs)).name() << "{" << filename << ", ...}.is_open() == " << std::boolalpha
                     << false << std::noboolalpha;
            } else {
                ofs.write(reinterpret_cast<decltype(ofs)::char_type const*>(bytes.src), bytes.count);
                if (!ofs.good()) {
                    auto const fmtflags = args.flags();
                    args << typeid(decltype(ofs)).name() << "{" << filename << ", ...}.write(" << std::showbase
                         << std::hex << bytes.src << ", " << std::dec << bytes.count << ").good() == " << std::boolalpha
                         << false;
                    args.setf(fmtflags);
                }
                ofs.close();
                count = bytes.count;
            }
        }
    }
    throw_on_error(__func__, args.str());
    return count;
}

bool operator==(Document const& lhs, Document const& rhs) {
    /// \note `AMequal(nullptr, nullptr) == false`
    return (lhs.m_document == rhs.m_document) || AMequal(lhs.m_document, rhs.m_document);
}

}  // namespace utils
}  // namespace usdj_am
}  // namespace cavi

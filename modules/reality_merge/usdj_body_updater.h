/**************************************************************************/
/* usdj_body_updater.h                                                    */
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

#ifndef REALITY_MERGE_USDJ_BODY_UPDATER_H
#define REALITY_MERGE_USDJ_BODY_UPDATER_H

#include <cstdint>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <string>

// third-party
#include <cavi/usdj_am/definition.hpp>
#include <cavi/usdj_am/visitor.hpp>

// regional
#include <core/typedefs.h>
#include <core/variant/typed_array.h>

// local
#include "usdj_static_body_3d.h"

namespace cavi {
namespace usdj_am {
namespace utils {

class Document;

}  // namespace utils
}  // namespace usdj_am
}  // namespace cavi

class UsdjBodyUpdater : public cavi::usdj_am::Visitor {
public:
    using Body = UsdjStaticBody3D;

    enum class Action : std::uint8_t { BEGIN__ = 1, ADD = BEGIN__, KEEP, REMOVE, END__, SIZE__ = END__ - BEGIN__ };

    using Updates = std::multimap<Action, Body*>;

    UsdjBodyUpdater() = delete;

    /// \brief Borrows nodes that represent physics bodies within a scene.
    ///
    /// \param[in] nodes An array of child nodes in a scene node.
    UsdjBodyUpdater(TypedArray<Node> const& nodes);

    UsdjBodyUpdater(UsdjBodyUpdater const&) = delete;

    UsdjBodyUpdater(UsdjBodyUpdater&&) = default;

    ~UsdjBodyUpdater();

    UsdjBodyUpdater& operator=(UsdjBodyUpdater const&) = delete;

    UsdjBodyUpdater& operator=(UsdjBodyUpdater&&) = default;

    /// \brief Creates new physics bodies and sorts pre-existing ones into
    ///        categories of forgotten, kept and removed.
    ///
    /// \param[in] document An Automerge document.
    /// \param[in] path A POSIX path to a "USDA_File" node within \p document.
    /// \returns A multimap of categories to physics bodies.
    Updates operator()(cavi::usdj_am::utils::Document const& document, std::string const& path);

    void visit(cavi::usdj_am::Assignment const& assignment) override;

    void visit(cavi::usdj_am::Definition const& definition) override;

    void visit(cavi::usdj_am::DefinitionStatement&& definition_statement) override;

    void visit(cavi::usdj_am::Descriptor const& descriptor) override;

    void visit(cavi::usdj_am::File const& file) override;

    void visit(cavi::usdj_am::Statement&& statement) override;

private:
    using Bodies = std::list<Body*>;

    Bodies m_bodies;
    std::optional<std::string> m_default_prim;
    std::optional<cavi::usdj_am::Definition> m_definition;
    Updates m_updates;
    bool m_visited_default_prim;
};

#endif  // REALITY_MERGE_USDJ_BODY_UPDATER_H

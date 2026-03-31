/**
* @file MenuItemDefinition.h
 * @brief Static descriptor for a single context menu entry.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "model/context_menu/ContextMenu.h"

#include <QString>
#include <functional>

namespace dentry::model {

    /**
     * @struct MenuItemDefinition
     * @brief Immutable template for one menu row.
     *
     * All logic is encoded as callables so the definition table is the
     * single source of truth — adding a new item requires only one new
     * entry in the table, nothing else changes.
     *
     * labelFn takes priority over label when set, enabling dynamic labels
     * such as "Paste (Move)" vs "Paste (Copy)".
     *
     * Example:
     * @code
     * MenuItemDefinition {
     *     "Copy",
     *     {},            // no dynamic label
     *     false,
     *     [](const MenuContext &c) { return !c.selectedPaths.isEmpty(); },
     *     [ctrl](const MenuContext &c) { ctrl->onCopyRequested(c.selectedPaths); }
     * };
     * @endcode
     */
    struct MenuItemDefinition {
        QString                                    label;
        std::function<QString(const ContextMenu &)> labelFn;       ///< Optional — overrides label.
        bool                                       isSeparator = false;
        std::function<bool(const ContextMenu &)>   isEnabled;      ///< nullptr → always disabled.
        std::function<void(const ContextMenu &)>   execute;        ///< nullptr for separators.

        /** @brief Convenience factory for separator rows. */
        [[nodiscard]] static MenuItemDefinition separator() {
            MenuItemDefinition d;
            d.isSeparator = true;
            return d;
        }
    };

} // namespace dentry::model

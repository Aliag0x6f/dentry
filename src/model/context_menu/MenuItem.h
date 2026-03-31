/**
* @file MenuItem.h
 * @brief Materialized context menu row, ready for display.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QString>
#include <functional>

namespace dentry::model {

    /**
     * @struct MenuItem
     * @brief Result of evaluating a MenuItemDefinition against a MenuContext.
     *
     * execute() is already bound to the context that was active at update() time
     * — FileListContextMenu calls it directly on QAction::triggered.
     */
    struct MenuItem {
        QString               label;
        bool                  isSeparator = false;
        bool                  enabled     = false;
        std::function<void()> execute;

        [[nodiscard]] static MenuItem separator() {
            MenuItem m;
            m.isSeparator = true;
            return m;
        }
    };

} // namespace dentry::model

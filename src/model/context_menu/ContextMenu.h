/**
* @file MenuContext.h
 * @brief Snapshot of the UI state used to evaluate context menu items.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QString>
#include <QStringList>

namespace dentry::model {

    /**
     * @struct ContextMenu
     * @brief Plain snapshot passed to every MenuItemDefinition predicate.
     *
     * Carries no logic — it is only read by isEnabled and execute functions.
     */
    struct ContextMenu {
        QStringList selectedPaths;
        QString     currentDirectory;
        bool        clipboardEmpty = true;
        bool        clipboardIsCut = false;
    };

} // namespace dentry::model

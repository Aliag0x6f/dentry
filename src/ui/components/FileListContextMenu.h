/**
* @file FileListContextMenu.h
 * @brief Context menu driven entirely by a ContextMenuModel.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "app/clipboard/Clipboard.h"
#include "model/context_menu/ContextMenuModel.h"

#include <QMenu>
#include <QPointer>
#include <QString>
#include <QStringList>

namespace dentry::ui {

    /**
     * @class FileListContextMenu
     * @brief Pure-UI context menu — no logic, no signals, no switch cases.
     *
     * On popup() it updates the model with the current context and iterates
     * model->items() to build QActions. Each action's triggered signal is
     * wired directly to the pre-bound execute() from the MenuItem.
     *
     * Adding a new menu entry requires only a new MenuItemDefinition in
     * the table passed to ContextMenuModel — this class never changes.
     */
    class FileListContextMenu : public QMenu {
        Q_OBJECT

    public:
        explicit FileListContextMenu(model::ContextMenuModel *model,
                                     app::Clipboard          *clipboard,
                                     QWidget                 *parent = nullptr);

        ~FileListContextMenu() override = default;

        FileListContextMenu(const FileListContextMenu &)            = delete;
        FileListContextMenu &operator=(const FileListContextMenu &) = delete;
        FileListContextMenu(FileListContextMenu &&)                 = delete;
        FileListContextMenu &operator=(FileListContextMenu &&)      = delete;

        /**
         * @brief Updates the model with the current context, rebuilds, and shows.
         */
        void popup(const QPoint      &pos,
                   const QStringList &selectedPaths,
                   const QString     &currentDirectory);

    private:
        /** @brief Clears and recreates QActions from the current model state. Pure loop. */
        void rebuild();

        QPointer<model::ContextMenuModel> m_model;
        QPointer<app::Clipboard>          m_clipboard;
    };

} // namespace dentry::ui

/**
* @file FileListContextMenu.cpp
 * @brief Implementation of FileListContextMenu.
 *
 * @author Hugo Fabresse
 */

#include "ui/components/FileListContextMenu.h"
#include "log/Logger.h"

namespace dentry::ui {

    FileListContextMenu::FileListContextMenu(model::ContextMenuModel *model,
                                             app::Clipboard          *clipboard,
                                             QWidget                 *parent)
        : QMenu(parent)
        , m_model(model)
        , m_clipboard(clipboard) {}

    void FileListContextMenu::popup(const QPoint      &pos,
                                     const QStringList &selectedPaths,
                                     const QString     &currentDirectory) {
        if (!m_model || !m_clipboard)
            return;

        m_model->update({
            selectedPaths,
            currentDirectory,
            m_clipboard->isEmpty(),
            m_clipboard->isCut()
        });

        rebuild();
        QMenu::popup(pos);
    }

    void FileListContextMenu::rebuild() {
        clear();

        if (!m_model)
            return;

        for (const model::MenuItem &item : m_model->items()) {
            if (item.isSeparator) {
                addSeparator();
                continue;
            }

            QAction *action = addAction(item.label);
            action->setEnabled(item.enabled);

            if (item.execute)
                connect(action, &QAction::triggered, this, [exec = item.execute] { exec(); });
        }

        log::debug("Ui") << "FileListContextMenu rebuilt:" << m_model->items().size() << "item(s)";
    }

} // namespace dentry::ui

/**
 * @file FileListEvents.h
 * @brief Event state and logging logic for FileListView interactions.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "../controllers/KeyboardController.h"
#include "../../model/FileItem.h"

#include <QList>
#include <QObject>
#include <QPointer>
#include <QStringList>
#include <functional>

class QContextMenuEvent;
class QEvent;
class QKeyEvent;
class QMouseEvent;
class QTreeView;

namespace dentry::app::events {

struct FileListEventCallbacks {
    std::function<void(FileListCommand)>         onCommand;
    std::function<void(const QString &)>         onDirectoryRequested;
    std::function<void(const model::FileItem &)> onFileActivated;
    std::function<void(const QStringList &)>     onCopyRequested;
    std::function<void(const QStringList &)>     onCutRequested;
    std::function<void(const QString &)>         onPasteRequested;
    std::function<void(const QStringList &)>     onDeleteRequested;
    std::function<void(const QString &)>         onRenameRequested;
    std::function<void(const QString &)>         onCreateFileRequested;
    std::function<void(const QString &)>         onCreateFolderRequested;
};

/**
 * @brief Event filter that centralizes file-list input handling and logging.
 */
class FileListEvents : public QObject {
public:
    explicit FileListEvents(QObject *parent = nullptr);

    /** @brief Sets callbacks used to dispatch semantic UI actions. */
    void setCallbacks(FileListEventCallbacks callbacks);

    /** @brief Installs this filter on the given file-list view. */
    void installOn(QTreeView *view);

    /** @brief Logs the command that has just been executed by the view. */
    void onCommandExecuted(FileListCommand command) const;

    /** @brief Logs current selection and inferred input source. */
    void onSelectionChanged(const QList<model::FileItem> &selected);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    enum class SelectionInputSource {
        Unknown,
        Keyboard,
        Mouse
    };

    [[nodiscard]] static const char *selectionInputSourceToString(SelectionInputSource source);

    bool handleKeyPress(QKeyEvent *event);
    bool handleMouseDoubleClick(QMouseEvent *event) const;
    bool handleContextMenu(QContextMenuEvent *event) const;

    [[nodiscard]] bool hasEventView() const;

    QPointer<QTreeView>    m_view;
    FileListEventCallbacks m_callbacks;
    KeyboardController     m_keyboardController;
    SelectionInputSource   m_lastSelectionInputSource = SelectionInputSource::Unknown;
};

} // namespace dentry::app::events

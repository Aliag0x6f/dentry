/**
 * @file FileListView.h
 * @brief File list view widget for the Dentry file manager.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "../model/AFileSystemModel.h"
#include "../model/FileItem.h"
#include <QList>
#include <QTreeView>

namespace Dentry::Ui {

/**
 * @class FileListView
 * @brief Displays the contents of the current directory as a flat list.
 *
 * Wraps QTreeView configured as a flat list (no expand arrows).
 * Connects to a FileSystemModel and emits signals when the user
 * navigates or selects files.
 *
 * Example:
 * @code
 * auto *view = new FileListView(this);
 * view->setModel(model);
 * connect(view, &FileListView::directoryRequested, model, &FileSystemModel::setDirectory);
 * connect(view, &FileListView::selectionChanged,   statusBar, &StatusBar::setSelection);
 * @endcode
 */
class FileListView : public QTreeView {
    Q_OBJECT

public:
    /**
     * @brief Constructs the FileListView.
     * @param parent Optional Qt parent widget.
     */
    explicit FileListView(QWidget *parent = nullptr);

    ~FileListView() override = default;

    FileListView(const FileListView &)            = delete;
    FileListView &operator=(const FileListView &) = delete;
    FileListView(FileListView &&)                 = delete;
    FileListView &operator=(FileListView &&)      = delete;

signals:
    /**
     * @brief Emitted when the user double-clicks a directory.
     * @param path Absolute path of the directory to navigate into.
     */
    void directoryRequested(const QString &path);

    /**
     * @brief Emitted when the selection changes.
     * @param selected List of currently selected FileItems.
     */
    void selectionChanged(const QList<Model::FileItem> &selected);

    /**
     * @brief Emitted when the user double-clicks a file.
     * @param item The FileItem that was activated.
     */
    void fileActivated(const Model::FileItem &item);

protected:
    /**
     * @brief Handles double-click to navigate into directories or open files.
     * @param index The index that was double-clicked.
     */
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    /**
     * @brief Handles context menu requests.
     * @param event The context menu event.
     */
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    /**
     * @brief Handles selection changes in the view.
     */
    void onSelectionChanged();

private:
    /** @brief Configures the view properties. */
    void configure();
};

} // namespace Dentry::Ui

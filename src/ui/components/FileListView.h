/**
 * @file FileListView.h
 * @brief File list view widget for the Dentry file manager.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "../UIComponent.h"
#include "../../app/controllers/KeyboardController.h"
#include "../../model/AFileSystemModel.h"
#include "../../model/FileItem.h"

#include <QList>
#include <QTreeView>

class QKeyEvent;

namespace dentry::ui {

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
 * auto view = std::make_unique<FileListView>(this);
 * view->setModel(model);
 * connect(view.get(), &FileListView::directoryRequested, this, &MainWindow::navigateTo);
 * connect(view.get(), &FileListView::selectionChanged,   statusBar, &StatusBar::setSelection);
 * view.release(); // Qt parent now owns the view.
 * @endcode
 */
class FileListView : public UIComponent<QTreeView, void> {
    Q_OBJECT

public:
    explicit FileListView(QWidget *parent = nullptr);

    ~FileListView() override = default;

    FileListView(const FileListView &)            = delete;
    FileListView &operator=(const FileListView &) = delete;
    FileListView(FileListView &&)                 = delete;
    FileListView &operator=(FileListView &&)      = delete;

    /**
     * @brief Sets the model and reconnects the selection model signal.
     * @param model The model to set.
     */
    void setModel(QAbstractItemModel *model) override;

    /**
     * @brief Sets the leader key used by custom keyboard sequences.
     *
     * Default bindings do not require a leader, but this enables future
     * user-configurable Vim-like combos.
     */
    void setKeyboardLeaderKey(Qt::Key key);


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
    void selectionChanged(const QList<model::FileItem> &selected);

    /**
     * @brief Emitted when the user double-clicks a file.
     * @param item The FileItem that was activated.
     */
    void fileActivated(const model::FileItem &item);

    /**
     * @brief Emitted when the user requests back navigation from the keyboard.
     */
    void backRequested();

    // ── Operations ────────────────────────────────────────────────────────

    /**
     * @brief Emitted when the user requests a copy of the selected items.
     * @param paths Absolute paths of the items to copy.
     */
    void copyRequested(const QStringList &paths);

    /**
     * @brief Emitted when the user requests a cut of the selected items.
     * @param paths Absolute paths of the items to cut.
     */
    void cutRequested(const QStringList &paths);

    /**
     * @brief Emitted when the user requests a paste into the current directory.
     * @param destination Absolute path of the destination directory.
     */
    void pasteRequested(const QString &destination);

    /**
     * @brief Emitted when the user requests deletion of the selected items.
     * @param paths Absolute paths of the items to delete.
     */
    void deleteRequested(const QStringList &paths);

    /**
     * @brief Emitted when the user requests a rename of a single item.
     * @param path Absolute path of the item to rename.
     */
    void renameRequested(const QString &path);

    /**
     * @brief Emitted when the user requests a new file in the current directory.
     * @param directory Absolute path of the directory.
     */
    void createFileRequested(const QString &directory);

    /**
     * @brief Emitted when the user requests a new folder in the current directory.
     * @param directory Absolute path of the directory.
     */
    void createFolderRequested(const QString &directory);

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

    /**
     * @brief Handles keyboard-driven navigation/actions in the list view.
     */
    void keyPressEvent(QKeyEvent *event) override;

    /** @brief Applies sorting and view sizing behavior. */
    void setupSize()  override;

    /** @brief Applies list view style and interaction mode. */
    void setupStyle() override;

private slots:
    /**
     * @brief Handles selection changes in the view.
     */
    void onSelectionChanged();

private:
    /** @brief Selects a specific row if it is valid. */
    void selectRow(int row);

    /** @brief Moves selection by a row delta (positive: down, negative: up). */
    void selectRelativeRow(int delta);

    /** @brief Selects the first row in the current model. */
    void selectFirstRow();

    /** @brief Selects the last row in the current model. */
    void selectLastRow();

    /** @brief Activates the current selection (navigate into folder / activate file). */
    void activateCurrentItem();

    /** @brief Dedicated keyboard command parser/binding manager. */
    app::KeyboardController m_keyboardController;
};

} // namespace dentry::ui

/**
 * @file FileListView.cpp
 * @brief Implementation of FileListView.
 *
 * @author Hugo Fabresse
 */

#include "FileListView.h"

#include <utility>

namespace dentry::ui {

    FileListView::FileListView(QWidget *parent)
        : UIComponent(parent) {
        build();

        app::events::FileListEventCallbacks callbacks;
        callbacks.onCommand = [this](const app::FileListCommand command) {
            executeKeyboardCommand(command);
        };
        callbacks.onDirectoryRequested = [this](const QString &path) {
            emit directoryRequested(path);
        };
        callbacks.onFileActivated = [this](const model::FileItem &item) {
            emit fileActivated(item);
        };
        callbacks.onCopyRequested = [this](const QStringList &paths) {
            emit copyRequested(paths);
        };
        callbacks.onCutRequested = [this](const QStringList &paths) {
            emit cutRequested(paths);
        };
        callbacks.onPasteRequested = [this](const QString &destination) {
            emit pasteRequested(destination);
        };
        callbacks.onDeleteRequested = [this](const QStringList &paths) {
            emit deleteRequested(paths);
        };
        callbacks.onRenameRequested = [this](const QString &path) {
            emit renameRequested(path);
        };
        callbacks.onCreateFileRequested = [this](const QString &directory) {
            emit createFileRequested(directory);
        };
        callbacks.onCreateFolderRequested = [this](const QString &directory) {
            emit createFolderRequested(directory);
        };

        m_events.setCallbacks(std::move(callbacks));
        m_events.installOn(this);
    }


    void FileListView::setupSize() {
        setSortingEnabled(true);
        sortByColumn(0, Qt::AscendingOrder);
    }

    void FileListView::setupStyle() {
        setRootIsDecorated(false);
        setAlternatingRowColors(true);
        setUniformRowHeights(true);
        setFocusPolicy(Qt::StrongFocus);
        setSelectionMode(QAbstractItemView::ExtendedSelection);
        setEditTriggers(QAbstractItemView::NoEditTriggers);
        setContextMenuPolicy(Qt::DefaultContextMenu);
        setObjectName("fileList");
    }

    // ── Model ─────────────────────────────────────────────────────────────────

    void FileListView::setModel(QAbstractItemModel *model) {
        if (QItemSelectionModel *old = selectionModel()) {
            disconnect(old, &QItemSelectionModel::selectionChanged,
                       this, &FileListView::onSelectionChanged);
        }

        QTreeView::setModel(model);

        if (QItemSelectionModel *current = selectionModel()) {
            connect(current, &QItemSelectionModel::selectionChanged,
                    this, &FileListView::onSelectionChanged);
        }
    }


    // ── Selection ─────────────────────────────────────────────────────────────

    void FileListView::onSelectionChanged() {
        const auto *fsModel = qobject_cast<const model::AFileSystemModel *>(model());
        if (!fsModel)
            return;

        QList<model::FileItem> selected;
        const QModelIndexList indexes = selectionModel()->selectedRows();
        selected.reserve(indexes.count());

        for (const QModelIndex &index : indexes)
            selected.append(fsModel->entries().at(index.row()));

        m_events.onSelectionChanged(selected);

        emit selectionChanged(selected);
    }

    void FileListView::executeKeyboardCommand(const app::FileListCommand command) {
        switch (command) {
            case app::FileListCommand::MoveDown:
                selectRelativeRow(+1);
                break;
            case app::FileListCommand::MoveUp:
                selectRelativeRow(-1);
                break;
            case app::FileListCommand::FirstEntry:
                selectFirstRow();
                break;
            case app::FileListCommand::LastEntry:
                selectLastRow();
                break;
            case app::FileListCommand::Activate:
                activateCurrentItem();
                break;
            case app::FileListCommand::NavigateBack:
                emit backRequested();
                break;
            case app::FileListCommand::FocusSidebar:
                emit focusSidebarRequested();
                break;
        }
    }

    void FileListView::selectRow(int row) {
        if (!model() || !selectionModel())
            return;

        if (row < 0 || row >= model()->rowCount(rootIndex()))
            return;

        const QModelIndex index = model()->index(row, 0, rootIndex());
        if (!index.isValid())
            return;

        setCurrentIndex(index);
        selectionModel()->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        scrollTo(index, QAbstractItemView::PositionAtCenter);
    }

    void FileListView::selectRelativeRow(int delta) {
        if (!model())
            return;

        const int rowCount = model()->rowCount(rootIndex());
        if (rowCount <= 0)
            return;

        const QModelIndex current = currentIndex();
        const int currentRow = current.isValid() ? current.row() : (delta > 0 ? -1 : rowCount);
        const int targetRow = qBound(0, currentRow + delta, rowCount - 1);
        selectRow(targetRow);
    }

    void FileListView::selectFirstRow() {
        selectRow(0);
    }

    void FileListView::selectLastRow() {
        if (!model())
            return;

        const int rowCount = model()->rowCount(rootIndex());
        if (rowCount <= 0)
            return;

        selectRow(rowCount - 1);
    }

    void FileListView::activateCurrentItem() {
        const auto *fsModel = qobject_cast<const model::AFileSystemModel *>(model());
        if (!fsModel)
            return;

        const QModelIndex index = currentIndex();
        if (!index.isValid())
            return;

        const int row = index.row();
        const auto &entries = fsModel->entries();

        if (row < 0 || row >= entries.size())
            return;

        const model::FileItem item = entries.at(row);
        if (item.isDir)
            emit directoryRequested(item.absolutePath);
        else
            emit fileActivated(item);
    }

} // namespace dentry::ui

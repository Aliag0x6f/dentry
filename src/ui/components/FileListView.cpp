/**
 * @file FileListView.cpp
 * @brief Implementation of FileListView.
 *
 * @author Hugo Fabresse
 */

#include "FileListView.h"

#include <QContextMenuEvent>
#include <QMenu>
#include <QMouseEvent>

namespace dentry::ui {

    FileListView::FileListView(QWidget *parent)
        : UIComponent(parent) {
        build();
    }


    void FileListView::setupSize() {
        setSortingEnabled(true);
        sortByColumn(0, Qt::AscendingOrder);
    }

    void FileListView::setupStyle() {
        setRootIsDecorated(false);
        setAlternatingRowColors(true);
        setUniformRowHeights(true);
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

    // ── Events ────────────────────────────────────────────────────────────────

    void FileListView::mouseDoubleClickEvent(QMouseEvent *event) {
        const QModelIndex index = indexAt(event->pos());

        if (!index.isValid()) {
            QTreeView::mouseDoubleClickEvent(event);
            return;
        }

        const auto *fsModel = qobject_cast<const model::AFileSystemModel *>(model());
        if (!fsModel) {
            QTreeView::mouseDoubleClickEvent(event);
            return;
        }

        const model::FileItem item = fsModel->entries().at(index.row());

        if (item.isDir)
            emit directoryRequested(item.absolutePath);
        else
            emit fileActivated(item);

        QTreeView::mouseDoubleClickEvent(event);
    }

    void FileListView::contextMenuEvent(QContextMenuEvent *event) {
        const auto *fsModel = qobject_cast<const model::AFileSystemModel *>(model());
        if (!fsModel)
            return;

        QStringList selectedPaths;
        const QModelIndexList indexes = selectionModel()->selectedRows();
        for (const QModelIndex &i : indexes)
            selectedPaths << fsModel->entries().at(i.row()).absolutePath;

        const QString currentDir = fsModel->currentPath();

        QMenu menu(this);

        if (!selectedPaths.isEmpty()) {
            menu.addAction("Copy",   this, [this, selectedPaths] { emit copyRequested(selectedPaths); });
            menu.addAction("Cut",    this, [this, selectedPaths] { emit cutRequested(selectedPaths); });
            menu.addSeparator();

            if (selectedPaths.count() == 1) {
                menu.addAction("Rename", this, [this, selectedPaths] {
                    emit renameRequested(selectedPaths.first());
                });
            }

            menu.addAction("Delete", this, [this, selectedPaths] { emit deleteRequested(selectedPaths); });
            menu.addSeparator();
        }

        menu.addAction("Paste",      this, [this, currentDir] { emit pasteRequested(currentDir); });
        menu.addSeparator();
        menu.addAction("New File",   this, [this, currentDir] { emit createFileRequested(currentDir); });
        menu.addAction("New Folder", this, [this, currentDir] { emit createFolderRequested(currentDir); });

        menu.exec(event->globalPos());
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

        emit selectionChanged(selected);
    }

} // namespace dentry::ui

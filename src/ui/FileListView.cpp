/**
 * @file FileListView.cpp
 * @brief Implementation of FileListView.
 *
 * @author Hugo Fabresse
 */

#include "FileListView.h"
#include <QContextMenuEvent>
#include <QMouseEvent>
#include <QMenu>

namespace Dentry::Ui {

    FileListView::FileListView(QWidget *parent)
        : QTreeView(parent) {
        configure();
    }

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

    void FileListView::configure() {
        setRootIsDecorated(false);
        setAlternatingRowColors(true);
        setUniformRowHeights(true);
        setSelectionMode(QAbstractItemView::ExtendedSelection);
        setSortingEnabled(true);
        setEditTriggers(QAbstractItemView::NoEditTriggers);
        setContextMenuPolicy(Qt::DefaultContextMenu);
        sortByColumn(0, Qt::AscendingOrder);
    }

    void FileListView::mouseDoubleClickEvent(QMouseEvent *event) {
        const QModelIndex index = indexAt(event->pos());

        if (!index.isValid()) {
            QTreeView::mouseDoubleClickEvent(event);
            return;
        }

        const auto *fsModel = qobject_cast<const Model::AFileSystemModel *>(model());
        if (!fsModel) {
            QTreeView::mouseDoubleClickEvent(event);
            return;
        }

        const Model::FileItem item = fsModel->entries().at(index.row());

        if (item.isDir)
            emit directoryRequested(item.absolutePath);
        else
            emit fileActivated(item);

        QTreeView::mouseDoubleClickEvent(event);
    }

    void FileListView::contextMenuEvent(QContextMenuEvent *event) {
        const QModelIndex index = indexAt(event->pos());
        const auto *fsModel = qobject_cast<const Model::AFileSystemModel *>(model());

        if (!fsModel)
          return;

        QStringList selectedPaths;
        const QModelIndexList indexes = selectionModel()->selectedRows();
        for (const QModelIndex &i : indexes)
          selectedPaths << fsModel->entries().at(i.row()).absolutePath;

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

        const QString currentDir = fsModel->currentPath();
        menu.addAction("Paste",      this, [this, currentDir] { emit pasteRequested(currentDir); });
        menu.addSeparator();
        menu.addAction("New File",   this, [this, currentDir] { emit createFileRequested(currentDir); });
        menu.addAction("New Folder", this, [this, currentDir] { emit createFolderRequested(currentDir); });

        menu.exec(event->globalPos());
    }

    void FileListView::onSelectionChanged() {
        const auto *fsModel = qobject_cast<const Model::AFileSystemModel *>(model());
        if (!fsModel)
            return;

        QList<Model::FileItem> selected;

        const QModelIndexList indexes = selectionModel()->selectedRows();
        selected.reserve(indexes.count());

        for (const QModelIndex &index : indexes)
            selected.append(fsModel->entries().at(index.row()));

        emit selectionChanged(selected);
    }

} // namespace Dentry::Ui

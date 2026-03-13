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

        QMenu menu(this);

        if (index.isValid()) {
            menu.addAction("Open");
            menu.addAction("Rename");
            menu.addSeparator();
            menu.addAction("Copy");
            menu.addAction("Move");
            menu.addSeparator();
            menu.addAction("Delete");
            menu.addSeparator();
        }

        menu.addAction("New File");
        menu.addAction("New Folder");

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

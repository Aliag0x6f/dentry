// src/ui/components/FileListView.cpp
#include "log/Logger.h"
#include "ui/components/FileListView.h"

#include <QLabel>

namespace dentry::ui {

    FileListView::FileListView(QWidget *parent)
        : UIComponent(parent) {
        setObjectName("FileListView");
        m_model = new model::FileSystemModel(this);
        setModel(m_model);
        build();

        log::info("Ui") << "FileListView built";
    }

    void FileListView::setupStyle() {
        if (m_model) {
            setSortingEnabled(true);
            sortByColumn(0, Qt::AscendingOrder);
        }
        setRootIsDecorated(false);
        setAlternatingRowColors(true);
        setUniformRowHeights(true);
        setFocusPolicy(Qt::StrongFocus);
        setSelectionMode(QAbstractItemView::ExtendedSelection);
        setEditTriggers(QAbstractItemView::NoEditTriggers);
        setContextMenuPolicy(Qt::DefaultContextMenu);
    }

    void FileListView::setupConnections() {
        if (m_model) {
            connect(m_model, &model::FileSystemModel::directoryLoaded, this, [this](){ sortByColumn(0, Qt::AscendingOrder); });
        }
    }

    void FileListView::setModel(model::FileSystemModel *model) {
        m_model = model;
        QTreeView::setModel(m_model);
    }

} // namespace dentry::ui

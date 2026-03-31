// src/ui/components/FileListView.cpp
#include "log/Logger.h"
#include "ui/components/FileListView.h"
#include "app/events/EventBus.h"
#include "app/input/InputMap.h"

#include <QLabel>
#include <QMenu>
#include <QContextMenuEvent>

namespace dentry::ui {

    FileListView::FileListView(QWidget *parent)
        : UIComponent(parent) {
        setObjectName("FileListView");

        m_model = new model::FileSystemModel(this);
        setModel(m_model);
        if (m_model)
            m_model->manageSelectionFocus(selectionModel(), this);

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

        log::debug("Ui") << "FileListView style is set up";
    }

    void FileListView::setupConnections() {
        if (m_model) {
            connect(m_model, &model::FileSystemModel::directoryLoaded, this, [this](){ sortByColumn(0, Qt::AscendingOrder); });
        }

        connect(app::events::EventBus::instance(), &app::events::EventBus::focusWidget, this, [this](const QString& name) {
            if (name == objectName())
                this->setFocus();
        });

        log::debug("Ui") << "FileListView connections is set up";
    }

    void FileListView::setModel(model::FileSystemModel *model) {
        m_model = model;
        QTreeView::setModel(m_model);

        log::debug("Ui") << "FileListView model is set up";
    }

    QStringList FileListView::selectedPaths() const {
        if (!m_model)
            return {};

        QStringList paths;
        const auto indexes = selectionModel()->selectedRows();
        paths.reserve(indexes.size());

        for (const QModelIndex &idx : indexes) {
            const int row = idx.row();
            if (row >= 0 && row < m_model->entries().count())
                paths << m_model->entries().at(row).absolutePath;
        }

        return paths;
    }

} // namespace dentry::ui

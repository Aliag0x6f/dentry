// src/app/controller/NavigationController.h

#include "NavigationController.h"
#include "ui/components/FileListView.h"
#include "model/FileSystemModel.h"
#include "log/Logger.h"
#include <QDir>

namespace dentry::app {
    NavigationController::NavigationController(ui::FileListView *view, model::FileSystemModel *model, QObject *parent)
        : QObject(parent)
        , m_view(view)
        , m_model(model) {
        if (view)
            connect(view, &QTreeView::activated, this, &NavigationController::onActivated);

        if (m_model && m_view) {
            connect(m_model, &model::FileSystemModel::directoryLoaded,
                    this, &NavigationController::onDirectoryLoaded);

            if (m_model->rowCount() > 0)
                onDirectoryLoaded(m_model->currentPath());
        }
    }

    void NavigationController::onActivated(const QModelIndex &index) {
        if (!m_model)
            return;

        if (!index.isValid()) {
            QDir cur(m_model->currentPath());
            if (!cur.isRoot()) {
                cur.cdUp();
                m_model->setDirectory(cur.absolutePath());
            }
            return;
        }

        const auto entries = m_model->entries();
        if (index.row() < 0 || index.row() >= entries.count())
            return;

        const auto item = entries.at(index.row());
        if (item.isDir)
            m_model->setDirectory(item.absolutePath);
        else
            log::info("Ui") << "File activated:" << item.absolutePath;
    }

    void NavigationController::onDirectoryLoaded(const QString & /*path*/) {
        if (!m_view || !m_model)
            return;

        m_view->sortByColumn(0, Qt::AscendingOrder);

        const QModelIndex first = m_model->index(0, 0);
        if (first.isValid() && m_view->selectionModel()) {
            m_view->selectionModel()->setCurrentIndex(first, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        }
    }

} // namespace dentry::app

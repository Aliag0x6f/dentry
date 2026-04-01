// src/app/controller/NavigationController.h

#include "NavigationController.h"
#include "ui/components/FileListView.h"
#include "log/Logger.h"
#include <QDir>
#include <QStandardPaths>

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

    bool NavigationController::canGoBack() const {
        return m_history.size() > 1;
    }

    QString NavigationController::currentPath() const {
        return m_history.isEmpty() ? QString() : m_history.top();
    }

    void NavigationController::navigateTo(const QString &path) {
        if (!m_model)
            return;

        if (!QDir(path).exists()) {
            log::warn("Navigation") << "Directory does not exist:" << path;
            return;
        }

        if (path == currentPath())
            return;

        const bool couldGoBack = canGoBack();

        m_history.push(path);
        m_model->setDirectory(path);

        emit pathChanged(path);

        if (canGoBack() != couldGoBack)
            emit canGoBackChanged(canGoBack());

        log::info("Navigation") << "Navigated to:" << path;
    }

    void NavigationController::navigateBack() {
        if (!m_model || !canGoBack())
            return;

        m_history.pop();
        const QString path = m_history.top();

        m_model->setDirectory(path);
        emit pathChanged(path);
        emit canGoBackChanged(canGoBack());

        log::info("Navigation") << "Navigated back to:" << path;
    }

    void NavigationController::navigateHome() {
        navigateTo(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    }

    void NavigationController::onActivated(const QModelIndex &index) {
        if (!m_model)
            return;

        if (!index.isValid()) {
            QDir cur(m_model->currentPath());
            if (!cur.isRoot()) {
                cur.cdUp();
                navigateTo(cur.absolutePath());
            }
            return;
        }

        const auto entries = m_model->entries();
        if (index.row() < 0 || index.row() >= entries.count())
            return;

        const auto item = entries.at(index.row());
        if (item.isDir)
            navigateTo(item.absolutePath);
        else
            log::info("Ui") << "File activated:" << item.absolutePath;
    }

    void NavigationController::onDirectoryLoaded(const QString &path) {
        if (!m_view || !m_model)
            return;

        // Initialize history with the first directory
        if (m_history.isEmpty())
            m_history.push(path);

        m_view->sortByColumn(0, Qt::AscendingOrder);

        const QModelIndex first = m_model->index(0, 0);
        if (first.isValid() && m_view->selectionModel()) {
            m_view->selectionModel()->setCurrentIndex(first, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        }
    }

} // namespace dentry::app

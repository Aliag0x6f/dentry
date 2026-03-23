/**
 * @file NavigationController.cpp
 * @brief Implementation of NavigationController.
 *
 * @author Hugo Fabresse
 */

#include "NavigationController.h"
#include "../../log/Logger.h"

#include <QDir>
#include <QStandardPaths>


namespace dentry::app {

    NavigationController::NavigationController(model::FileSystemModel *model, QObject *parent)
        : QObject(parent)
        , m_model(model) {
    }

    QString NavigationController::currentPath() const {
        return m_history.isEmpty() ? QString() : m_history.top();
    }

    bool NavigationController::canGoBack() const {
        return m_history.size() > 1;
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
        if (!m_model)
            return;

        if (!canGoBack())
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

} // namespace dentry::app

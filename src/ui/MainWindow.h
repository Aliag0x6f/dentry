/**
 * @file MainWindow.h
 * @brief Main window of the Dentry file manager.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "AUIComponent.h"
#include "components/CentralWidget.h"
#include "components/StatusBar.h"
#include "components/Toolbar.h"
#include "../model/FileSystemModel.h"
#include "../app/controllers/FileOperationController.h"
#include "../app/controllers/NavigationController.h"

#include <QMainWindow>
#include <QPointer>
#include <QStack>
#include <QString>

namespace dentry::ui {

/**
 * @class MainWindow
 * @brief Orchestrates UI components and manages navigation and file operations.
 *
 * Responsible only for window-level concerns: geometry, toolbar/statusbar
 * registration, navigation history, and wiring signals between components.
 * Layout and widget ownership are delegated to CentralWidget.
 *
 * Example:
 * @code
 * MainWindow window;
 * window.show();
 * @endcode
 */
class MainWindow : public QMainWindow, public AUIComponent {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override = default;

    MainWindow(const MainWindow &)            = delete;
    MainWindow &operator=(const MainWindow &) = delete;
    MainWindow(MainWindow &&)                 = delete;
    MainWindow &operator=(MainWindow &&)      = delete;

    void build() override;

protected:
    void setupSize()        override;
    void setupConnections() override;

private slots:

    /**
     * @brief Updates the status bar directory stats from the model.
     */
    void onDirectoryLoaded(const QString &);

private:
    QPointer<model::FileSystemModel>       m_model;
    QPointer<Toolbar>                      m_toolbar;
    QPointer<StatusBar>                    m_statusBar;
    QPointer<CentralWidget>                m_central;
    QPointer<app::FileOperationController> m_fileOperationController;
    QPointer<app::NavigationController>    m_navigationController;
};

} // namespace dentry::ui

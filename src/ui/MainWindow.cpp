/**
 * @file MainWindow.cpp
 * @brief Implementation of MainWindow.
 *
 * @author Hugo Fabresse
 */

#include "app/controllers/KeyboardController.h"
#include "app/controllers/NavigationController.h"
#include "app/bindings/FileListBindings.h"
#include "ui/MainWindow.h"
#include "log/Logger.h"

#include <QApplication>
#include <QDir>

namespace dentry::ui {

    MainWindow::MainWindow(QWidget *parent)
        : UIComponent(parent) {
        setupWidgets();
        build();

        log::info("Ui") << "MainWindow built";
    }

    void MainWindow::setupWidgets() {

        m_statusbar = new StatusBar(this);
        m_statusbar->setObjectName("StatusBar");
        setStatusBar(m_statusbar);

        m_centralWidget = new CentralWidget(this);
        m_centralWidget->setObjectName("CentralWidget");
        setCentralWidget(m_centralWidget);

        m_toolbar = new ToolBar(this);
        m_toolbar->setObjectName("ToolBar");
        addToolBar(m_toolbar);

        log::debug("Ui") << "MainWindow widgets is set up";
    }

    void MainWindow::setupStyle() {
        setWindowTitle(QApplication::applicationDisplayName());

        log::debug("Ui") << "MainWindow style is set up";
    }

    void MainWindow::setupSize() {
        setMinimumSize(800, 500);
        resize(1100, 650);

        log::debug("Ui") << "MainWindow size is set up";
    }

    void MainWindow::setupConnections() {
        m_inputRegistry = new app::InputRegistry(this);

        m_inputRegistry->installAll({
            app::bindings::fileList(m_centralWidget->fileListView())
        });

        auto *view = m_centralWidget->fileListView();
        auto *mdl = qobject_cast<model::FileSystemModel *>(view->model());
        if (view && mdl) {
            new app::NavigationController(view, mdl, this);
        }
    }

} // namespace dentry::ui

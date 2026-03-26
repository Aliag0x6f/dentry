/**
 * @file MainWindow.cpp
 * @brief Implementation of MainWindow.
 *
 * @author Hugo Fabresse
 */

#include "MainWindow.h"
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
    }

    void MainWindow::setupStyle() {
        setWindowTitle(QApplication::applicationDisplayName());
    }

    void MainWindow::setupSize() {
        setMinimumSize(800, 500);
        resize(1100, 650);
    }

    void MainWindow::setupConnections() {
        // No child components are wired for now.
    }

} // namespace dentry::ui

/**
 * @file MainWindow.cpp
 * @brief Implementation of MainWindow.
 *
 * @author Hugo Fabresse
 */

#include "MainWindow.h"
#include "log/Logger.h"

#include <QApplication>

namespace dentry::ui {

    MainWindow::MainWindow(QWidget *parent)
        : UIComponent(parent) {
        setupWidgets();
        build();

        log::info("Ui") << "MainWindow built";
    }

    void MainWindow::setupWidgets() {
        m_model = new model::FileSystemModel(this);

        m_statusbar = new StatusBar(this);
        m_statusbar->setObjectName("StatusBar");

        m_centralwidget = new CentralWidget(m_model, this);
        m_centralwidget->setObjectName("CentralWidget");

        m_toolbar = new ToolBar(this);
        m_toolbar->setObjectName("ToolBar");

        setStatusBar(m_statusbar);
        setCentralWidget(m_centralwidget);
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

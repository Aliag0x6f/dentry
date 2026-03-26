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
        build();
        log::info("Ui") << "MainWindow built";
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

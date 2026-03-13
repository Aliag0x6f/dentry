/**
* @file main.cpp
 * @brief Entry point for the Dentry file manager.
 *
 * @author Hugo Fabresse
 */

#include "app/Application.h"
#include "ui/MainWindow.h"

int main(int argc, char *argv[]) {
    Dentry::Application app(argc, argv);

    Dentry::Ui::MainWindow window;
    window.show();

    return app.exec();
}

/**
* @file Application.cpp
 * @brief Implementation of Application.
 *
 * @author Hugo Fabresse
 */

#include "Application.h"
#include "../ui/Style.h"

namespace Dentry {

    Application::Application(int &argc, char **argv)
        : QApplication(argc, argv) {
        initialize();
    }

    void Application::initialize() {
        setApplicationName("Dentry");
        setApplicationVersion("1.0.0");
        setOrganizationName("Aliag0x6f");

        setStyleSheet(Dentry::Ui::Style::sheet());
    }

} // namespace Dentry

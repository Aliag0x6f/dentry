/**
 * @file Application.cpp
 * @brief Implementation of Application.
 *
 * @author Hugo Fabresse
 */

#include "Application.h"
#include "../ui/Style.h"
#include "../util/Logger.h"

namespace Dentry {

    Application::Application(int &argc, char **argv)
        : QApplication(argc, argv) {
        initialize();
    }

    void Application::initialize() {
        Dentry::Util::Logger::install();

        setApplicationName("Dentry");
        setApplicationVersion("1.0.0");

        setStyleSheet(Dentry::Ui::Style::sheet());
    }

} // namespace Dentry

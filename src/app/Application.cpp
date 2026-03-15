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
        LOG_INFO("App") << "Logger installed";

        setApplicationName("Dentry");
        setApplicationVersion("1.0.0");

        LOG_INFO("App") << "Dentry" << applicationVersion() << "starting";

        setStyleSheet(Dentry::Ui::Style::sheet());
        LOG_INFO("App") << "Stylesheet loaded";
    }

} // namespace Dentry

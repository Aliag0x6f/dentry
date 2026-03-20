/**
 * @file Application.cpp
 * @brief Implementation of Application.
 *
 * @author Hugo Fabresse
 */

#include "Application.h"
#include "AppMetadata.h"
#include "../ui/Style.h"
#include "../log/Logger.h"

namespace Dentry {

    Application::Application(int &argc, char **argv)
        : QApplication(argc, argv) {
        initialize();
    }

    void Application::initialize() {
        Dentry::log::install();

        setApplicationName(Metadata::AppName);
        setApplicationDisplayName(Metadata::AppDisplayName);
        setApplicationVersion(Metadata::AppVersion);
        setStyleSheet(Dentry::Ui::Style::sheet());

        log::info("App") << Metadata::AppDisplayName << Metadata::AppVersion << "initialized";
    }

} // namespace Dentry

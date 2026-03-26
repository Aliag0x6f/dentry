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

namespace dentry {

    Application::Application(int &argc, char **argv)
        : QApplication(argc, argv) {
        initialize();
    }

    void Application::initialize() {
        log::install();

        setApplicationName(metadata::AppName);
        setApplicationDisplayName(metadata::AppDisplayName);
        setApplicationVersion(metadata::AppVersion);
        setStyleSheet(dentry::ui::Style::sheet());

        log::info("App") << metadata::AppDisplayName << metadata::AppVersion << "initialized";
    }

} // namespace dentry

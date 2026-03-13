/**
 * @file Application.h
 * @brief Main application class for Dentry.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QApplication>

namespace Dentry {

    /**
     * @class Application
     * @brief Wraps QApplication with Dentry-specific initialization.
     *
     * Handles stylesheet loading and any future application-wide setup.
     *
     * Example:
     * @code
     * int main(int argc, char *argv[]) {
     *     Application app(argc, argv);
     *     return app.exec();
     * }
     * @endcode
     */
    class Application : public QApplication {
        Q_OBJECT

    public:
        explicit Application(int &argc, char **argv);

        ~Application() override = default;

        Application(const Application &)            = delete;
        Application &operator=(const Application &) = delete;
        Application(Application &&)                 = delete;
        Application &operator=(Application &&)      = delete;

    private:
        /** @brief Initializes application metadata and stylesheet. */
        void initialize();
    };

} // namespace Dentry

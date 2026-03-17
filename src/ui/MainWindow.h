/**
 * @file MainWindow.h
 * @brief Main window of the Dentry file manager.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "AUIComponent.h"
#include "components/CentralWidget.h"
#include "components/StatusBar.h"
#include "components/Toolbar.h"
#include "../app/Clipboard.h"
#include "../model/FileSystemModel.h"

#include <QMainWindow>
#include <QStack>
#include <QString>

#include "navigation/NavigationController.h"

namespace Dentry::Ui {

/**
 * @class MainWindow
 * @brief Orchestrates UI components and manages navigation and file operations.
 *
 * Responsible only for window-level concerns: geometry, toolbar/statusbar
 * registration, navigation history, and wiring signals between components.
 * Layout and widget ownership are delegated to CentralWidget.
 *
 * Example:
 * @code
 * MainWindow window;
 * window.show();
 * @endcode
 */
class MainWindow : public QMainWindow, public AUIComponent {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override = default;

    MainWindow(const MainWindow &)            = delete;
    MainWindow &operator=(const MainWindow &) = delete;
    MainWindow(MainWindow &&)                 = delete;
    MainWindow &operator=(MainWindow &&)      = delete;

    void build() override;

protected:
    void setupSize()        override;
    void setupConnections() override;

private slots:

    /**
     * @brief Updates the status bar directory stats from the model.
     */
    void onDirectoryLoaded(const QString &);

    // ── Operations ────────────────────────────────────────────────────────

    /**
     * @brief Copies the given paths to the clipboard.
     */
    void onCopyRequested(const QStringList &paths);

    /**
     * @brief Cuts the given paths to the clipboard.
     */
    void onCutRequested(const QStringList &paths);

    /**
     * @brief Pastes the clipboard contents into the given directory.
     */
    void onPasteRequested(const QString &destination);

    /**
     * @brief Deletes the given paths.
     */
    void onDeleteRequested(const QStringList &paths);

    /**
     * @brief Prompts for a new name and renames the given path.
     */
    void onRenameRequested(const QString &path);

    /**
     * @brief Prompts for a name and creates a new file in the given directory.
     */
    void onCreateFileRequested(const QString &directory);

    /**
     * @brief Prompts for a name and creates a new folder in the given directory.
     */
    void onCreateFolderRequested(const QString &directory);

private:
    Model::FileSystemModel *m_model;
    Toolbar                *m_toolbar;
    StatusBar              *m_statusBar;
    CentralWidget          *m_central;
    App::Clipboard          m_clipboard;
    NavigationController   *m_navigationController;
};

} // namespace Dentry::Ui

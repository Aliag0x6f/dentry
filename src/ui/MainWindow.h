/**
 * @file MainWindow.h
 * @brief Main window of the Dentry file manager.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "FileListView.h"
#include "PreviewPanel.h"
#include "Sidebar.h"
#include "StatusBar.h"
#include "Toolbar.h"
#include "../app/Clipboard.h"
#include "../model/FileSystemModel.h"

#include <QMainWindow>
#include <QSplitter>
#include <QStack>
#include <QString>

namespace Dentry::Ui {

/**
 * @class MainWindow
 * @brief Orchestrates all UI components and connects them to the model.
 *
 * Builds the main layout, manages navigation history,
 * and wires all signals and slots between components.
 *
 * Example:
 * @code
 * MainWindow window;
 * window.show();
 * @endcode
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override = default;

    MainWindow(const MainWindow &)            = delete;
    MainWindow &operator=(const MainWindow &) = delete;
    MainWindow(MainWindow &&)                 = delete;
    MainWindow &operator=(MainWindow &&)      = delete;

private slots:
    /**
     * @brief Navigates to the given directory and pushes it to history.
     * @param path Absolute path of the directory to navigate to.
     */
    void navigateTo(const QString &path);

    /**
     * @brief Navigates back to the previous directory.
     */
    void navigateBack();

    /**
     * @brief Navigates to the home directory.
     */
    void navigateHome();

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
    /** @brief Builds the main window layout and all widgets. */
    void build();

    /** @brief Connects all signals and slots between components. */
    void connectSignals();

    Toolbar                *m_toolbar;
    Sidebar                *m_sidebar;
    FileListView           *m_fileListView;
    PreviewPanel           *m_previewPanel;
    StatusBar              *m_statusBar;
    Model::FileSystemModel *m_model;
    QSplitter              *m_splitter;
    QStack<QString>         m_history;

    /**
     * @brief Internal clipboard for copy/cut/paste operations.
     */
    App::Clipboard m_clipboard;
};

} // namespace Dentry::Ui

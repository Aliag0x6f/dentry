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

private:
    /** @brief Builds the main window layout and all widgets. */
    void build();

    /** @brief Connects all signals and slots between components. */
    void connectSignals();

    Toolbar                 *m_toolbar;
    Sidebar                 *m_sidebar;
    FileListView            *m_fileListView;
    PreviewPanel            *m_previewPanel;
    StatusBar               *m_statusBar;
    Model::FileSystemModel  *m_model;
    QSplitter               *m_splitter;
    QStack<QString>          m_history;
};

} // namespace Dentry::Ui

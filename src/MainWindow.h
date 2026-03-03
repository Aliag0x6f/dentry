/**
 * @file MainWindow.h
 * @brief Main window of the Dentry file manager.
 *
 * Dentry is a minimal, keyboard-driven file manager for Linux.
 * Built with Qt6, styled for any desktop environment.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QMainWindow>
#include <QTreeView>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include <QStatusBar>
#include <QStandardItemModel>
#include <QToolButton>

/**
 * @class MainWindow
 * @brief The main application window.
 *
 * Provides:
 * - Sidebar with quick-access places
 * - File list with columns: Name, Size, Type, Date Modified, Permissions
 * - Toolbar with navigation and file operation buttons
 * - Status bar with directory statistics
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Constructs the main window.
     * @param parent Optional parent widget.
     */
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    /**
     * @brief Handles double-click or Enter on a file/directory.
     * @param index Model index of the activated item.
     */
    void onItemActivated(const QModelIndex &index);

    /**
     * @brief Navigates to a place when clicked in the sidebar.
     * @param item The clicked sidebar item.
     */
    void onSidebarClicked(QListWidgetItem *item);

    /** @brief Navigates to the parent directory. */
    void goUp();

    /** @brief Creates a new empty file in the current directory. */
    void createFile();

    /** @brief Creates a new directory in the current directory. */
    void createFolder();

    /** @brief Renames the first selected item. */
    void renameSelected();

    /** @brief Deletes all selected items after confirmation. */
    void deleteSelected();

    /** @brief Toggles visibility of dot-files and updates the button style. */
    void toggleHidden();

    /**
     * @brief Filters the file list by name.
     * @param text The search string.
     */
    void onSearchChanged(const QString &text);

    /**
     * @brief Shows a context menu at the given viewport position.
     * @param pos Position in viewport coordinates.
     */
    void showContextMenu(const QPoint &pos);

private:
    /** @brief Builds and lays out all UI components. */
    void setupUI();

    /** @brief Applies the QSS stylesheet to the application. */
    void applyStyle();

    /**
     * @brief Loads and displays a directory's contents.
     * @param path Absolute path of the directory.
     */
    void loadDirectory(const QString &path);

    /** @brief Updates the status bar with current directory stats. */
    void updateStatusBar();

    /** @brief Builds and returns the sidebar widget. */
    QWidget* buildSidebarWidget();

    /** @brief Populates the sidebar list with standard places. */
    void buildSidebar();

    /** @brief Builds and returns the file tree view. */
    QWidget* buildFileView();

    /** @brief Creates and configures the toolbar. */
    void buildToolbar();

    /**
     * @brief Updates the visual style of the hidden files toggle button.
     *
     * White background when active (hidden files shown),
     * dark background when inactive.
     */
    void updateHiddenButtonStyle();

    /** @brief Custom item model for the file list. */
    QStandardItemModel *model;

    /** @brief Tree view displaying directory contents. */
    QTreeView *treeView;

    /** @brief Sidebar listing quick-access places. */
    QListWidget *sidebar;

    /** @brief Label showing the current directory path. */
    QLabel *pathLabel;

    /** @brief Search input for filtering file names. */
    QLineEdit *searchBox;

    /** @brief The application status bar. */
    QStatusBar *statusBar;

    /** @brief Label inside the status bar. */
    QLabel *statusLabel;

    /** @brief Toggle button for showing/hiding dot-files. */
    QToolButton *hiddenBtn;

    /** @brief Absolute path of the currently displayed directory. */
    QString currentPath;

    /** @brief Whether hidden dot-files are shown. */
    bool showHidden = false;

    /** @brief Current search filter string. */
    QString searchFilter;
};


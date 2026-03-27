/**
 * @file MainWindow.h
 * @brief Main window of the Dentry file manager.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "app/input/InputRegistry.h"
#include "model/FileSystemModel.h"
#include "ui/components/CentralWidget.h"
#include "ui/components/StatusBar.h"
#include "ui/components/Toolbar.h"
#include "ui/UIComponent.h"

#include <QMainWindow>
#include <QPointer>

namespace dentry::ui {

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
class MainWindow : public UIComponent<QMainWindow, void> {
    Q_OBJECT

public:
    /**
     * @brief Constructs the main application window.
     *
     * Creates and wires all top-level UI components through the UIComponent
     * setup pipeline.
     *
     * @param parent Optional Qt parent widget.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override = default;

    MainWindow(const MainWindow &)            = delete;
    MainWindow &operator=(const MainWindow &) = delete;
    MainWindow(MainWindow &&)                 = delete;
    MainWindow &operator=(MainWindow &&)      = delete;

protected:
    /** @brief Applies window geometry constraints (minimum size and initial size). */
    void setupSize()        override;

    /** @brief Applies top-level window style metadata (title, object names if needed). */
    void setupStyle()       override;

    /** @brief Connects cross-component signals/slots for navigation and file operations. */
    void setupConnections() override;

private:
    void setupWidgets();

    QPointer<StatusBar>     m_statusbar;
    QPointer<CentralWidget> m_centralWidget;
    QPointer<ToolBar>       m_toolbar;

    QPointer<app::InputRegistry> m_inputRegistry = nullptr;
};

} // namespace dentry::ui

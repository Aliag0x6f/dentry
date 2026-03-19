/**
 * @file NavigationController.h
 * @brief Navigation controller for the Dentry file manager.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "../../model/FileSystemModel.h"

#include <QObject>
#include <QPointer>
#include <QStack>
#include <QString>

namespace Dentry::App {

/**
 * @class NavigationController
 * @brief Manages directory navigation history and coordinates with the file system model.
 *
 * Encapsulates all navigation logic — history stack, path validation,
 * and model updates. MainWindow only needs to connect signals and slots;
 * it has no knowledge of how navigation is implemented.
 *
 * Example:
 * @code
 * auto nav = std::make_unique<NavigationController>(model, this);
 *
 * connect(toolbar,     &Toolbar::backRequested,            nav.get(), &NavigationController::navigateBack);
 * connect(toolbar,     &Toolbar::homeRequested,            nav.get(), &NavigationController::navigateHome);
 * connect(sidebar,     &Sidebar::placeSelected,            nav.get(), &NavigationController::navigateTo);
 * connect(fileList,    &FileListView::directoryRequested,  nav.get(), &NavigationController::navigateTo);
 * connect(nav.get(),   &NavigationController::pathChanged, toolbar,   &Toolbar::setPath);
 * nav.release(); // Qt parent now owns the controller.
 * @endcode
 */
class NavigationController : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructs the NavigationController.
     * @param model  The file system model to update on navigation.
     * @param parent Optional Qt parent object.
     */
    explicit NavigationController(Model::FileSystemModel *model, QObject *parent = nullptr);

    ~NavigationController() override = default;

    NavigationController(const NavigationController &)            = delete;
    NavigationController &operator=(const NavigationController &) = delete;
    NavigationController(NavigationController &&)                 = delete;
    NavigationController &operator=(NavigationController &&)      = delete;

    /**
     * @brief Returns the current directory path.
     * @return Absolute path of the current directory, or empty if none.
     */
    [[nodiscard]] QString currentPath() const;

    /**
     * @brief Returns whether backward navigation is possible.
     * @return True if the history stack has more than one entry.
     */
    [[nodiscard]] bool canGoBack() const;

public slots:
    /**
     * @brief Navigates to the given directory and pushes it to history.
     *
     * Does nothing if the path does not exist or is already the current path.
     *
     * @param path Absolute path of the directory to navigate to.
     */
    void navigateTo(const QString &path);

    /**
     * @brief Navigates back to the previous directory in history.
     *
     * Does nothing if there is no previous directory.
     */
    void navigateBack();

    /**
     * @brief Navigates to the user's home directory.
     */
    void navigateHome();

signals:
    /**
     * @brief Emitted when the current directory changes.
     * @param path Absolute path of the new current directory.
     */
    void pathChanged(const QString &path);

    /**
     * @brief Emitted when the ability to go back changes.
     *
     * Useful to enable/disable a back button in the toolbar.
     *
     * @param canGoBack True if backward navigation is now possible.
     */
    void canGoBackChanged(bool canGoBack);

private:
    QPointer<Model::FileSystemModel> m_model;
    QStack<QString>                  m_history;
};

} // namespace Dentry::App

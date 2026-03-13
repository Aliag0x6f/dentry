/**
 * @file Toolbar.h
 * @brief Toolbar widget for the Dentry file manager.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QAction>
#include <QLineEdit>
#include <QString>
#include <QToolBar>

namespace Dentry::Ui {

/**
 * @class Toolbar
 * @brief Provides navigation controls and search input.
 *
 * Contains back/home navigation buttons, a path label,
 * a hidden files toggle, and a search bar.
 *
 * Example:
 * @code
 * auto *toolbar = new Toolbar(this);
 * addToolBar(toolbar);
 * connect(toolbar, &Toolbar::backRequested,   this, &MainWindow::onBack);
 * connect(toolbar, &Toolbar::homeRequested,   this, &MainWindow::onHome);
 * connect(toolbar, &Toolbar::searchChanged,   model, &FileSystemModel::setFilter);
 * connect(toolbar, &Toolbar::hiddenToggled,   model, &FileSystemModel::setShowHidden);
 * @endcode
 */
class Toolbar : public QToolBar {
    Q_OBJECT

public:
    /**
     * @brief Constructs the Toolbar.
     * @param parent Optional Qt parent widget.
     */
    explicit Toolbar(QWidget *parent = nullptr);

    ~Toolbar() override = default;

    Toolbar(const Toolbar &)            = delete;
    Toolbar &operator=(const Toolbar &) = delete;
    Toolbar(Toolbar &&)                 = delete;
    Toolbar &operator=(Toolbar &&)      = delete;

    /**
     * @brief Updates the displayed path.
     * @param path Absolute path to display.
     */
    void setPath(const QString &path);

signals:
    /**
     * @brief Emitted when the user clicks the back button.
     */
    void backRequested();

    /**
     * @brief Emitted when the user clicks the home button.
     */
    void homeRequested();

    /**
     * @brief Emitted when the search text changes.
     * @param text The current search string.
     */
    void searchChanged(const QString &text);

    /**
     * @brief Emitted when the hidden files toggle changes.
     * @param show True if hidden files should be shown.
     */
    void hiddenToggled(bool show);

private:
    /** @brief Builds and populates the toolbar with actions and widgets. */
    void build();

    QAction   *m_backAction;
    QAction   *m_homeAction;
    QAction   *m_hiddenAction;
    QLineEdit *m_searchBar;
    QLineEdit *m_pathBar;
};

} // namespace Dentry::Ui

/**
 * @file Toolbar.h
 * @brief Toolbar widget for the Dentry file manager.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "../AUIComponent.h"

#include <QAction>
#include <QLabel>
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
     * connect(toolbar, &Toolbar::backRequested, this, &MainWindow::navigateBack);
     * connect(toolbar, &Toolbar::homeRequested, this, &MainWindow::navigateHome);
     * connect(toolbar, &Toolbar::searchChanged, model, &FileSystemModel::setFilter);
     * connect(toolbar, &Toolbar::hiddenToggled, sidebar, &Sidebar::setShowHidden);
     * @endcode
     */
    class Toolbar : public QToolBar, public AUIComponent {
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

        void build() override;

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

    protected:
        void setupStyle()       override;
        void setupSize()        override;
        void setupConnections() override;

    private:
        QAction   *m_backAction;
        QAction   *m_homeAction;
        QAction   *m_hiddenAction;
        QLineEdit *m_searchBar;
        QLabel    *m_pathBar;
    };

} // namespace Dentry::Ui

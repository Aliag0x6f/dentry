/**
 * @file Sidebar.h
 * @brief Sidebar widget displaying quick-access places.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "../AUIComponent.h"

#include <QFrame>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPointer>
#include <QString>
#include <QWidget>

namespace dentry::ui {

    /**
     * @class Sidebar
     * @brief Displays a list of quick-access places for fast navigation.
     *
     * Populates its list of places dynamically from directories under the user's
     * home directory (including hidden directories), allowing the user to click
     * an entry to navigate directly to that directory.
     *
     * Home is always listed first, Root always last.
     *
     * Example:
     * @code
     * auto sidebar = std::make_unique<Sidebar>(this);
     * connect(sidebar.get(), &Sidebar::placeSelected, this, &MainWindow::navigateTo);
     * sidebar.release(); // Qt parent now owns the sidebar.
     * @endcode
     */
    class Sidebar : public QFrame, public AUIComponent {
        Q_OBJECT

    public:
        /**
         * @brief Constructs the Sidebar.
         * @param parent Optional Qt parent widget.
         */
        explicit Sidebar(QWidget *parent = nullptr);

        ~Sidebar() override = default;

        Sidebar(const Sidebar &)            = delete;
        Sidebar &operator=(const Sidebar &) = delete;
        Sidebar(Sidebar &&)                 = delete;
        Sidebar &operator=(Sidebar &&)      = delete;

        /**
         * @brief Updates the sidebar to show or hide hidden directories.
         * @param show True to show hidden directories.
         */
        void setShowHidden(bool show);

        void build() override;

        signals:
            /**
             * @brief Emitted when the user clicks a place.
             * @param path Absolute path of the selected place.
             */
            void placeSelected(const QString &path);

    protected:
        void setupSize()        override;
        void setupStyle()       override;
        void setupConnections() override;

    private slots:
        /**
         * @brief Handles a click on a sidebar item.
         * @param item The clicked QListWidgetItem.
         */
        void onItemClicked(QListWidgetItem *item);

    private:
        /** @brief Builds and populates the list widget with standard places. */
        void buildPlaces();

        /** @brief The list widget displaying the places. */
        QPointer<QListWidget> m_list;

        /** @brief Whether hidden dot-files are shown. */
        bool m_showHidden = false;

    };

} // namespace dentry::ui

/**
 * @file Sidebar.h
 * @brief Sidebar widget displaying quick-access places.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "../DefaultLayout.h"
#include "../UIComponent.h"
#include "../../app/events/SidebarEvents.h"

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
    class Sidebar : public UIComponent<QFrame, VLayout> {
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

        /** @brief Returns the places list widget used by the sidebar. */
        [[nodiscard]] QListWidget *placesList() const;

        signals:
            /**
             * @brief Emitted when the user clicks a place.
             * @param path Absolute path of the selected place.
             */
            void placeSelected(const QString &path);

            /** @brief Emitted when keyboard flow should return focus to file list view. */
            void focusFileListRequested();

            /** @brief Emitted when the user requests toggling hidden files visibility. */
            void toggleHiddenRequested();

    protected:
        /** @brief Creates sidebar title, separator and list within the vertical layout. */
        void setupLayout(VLayout &layout) override;

        /** @brief Applies width constraints for the places list. */
        void setupSize()        override;

        /** @brief Applies object name used by styling rules. */
        void setupStyle()       override;

        /** @brief Connects list click handling to place selection behavior. */
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

        /** @brief Dedicated event orchestrator for keyboard interactions on places list. */
        app::events::SidebarEvents m_events;
    };

} // namespace dentry::ui

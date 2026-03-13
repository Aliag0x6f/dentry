/**
 * @file Sidebar.h
 * @brief Sidebar widget displaying quick-access places.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QListWidget>
#include <QListWidgetItem>
#include <QString>
#include <QWidget>

namespace Dentry::Ui {

    /**
     * @class Sidebar
     * @brief Displays a list of quick-access places for fast navigation.
     *
     * Provides a fixed list of standard locations (Home, Desktop, Documents, etc.)
     * that the user can click to navigate directly to that directory.
     *
     * Example:
     * @code
     * auto *sidebar = new Sidebar(this);
     * connect(sidebar, &Sidebar::placeSelected, model, &FileSystemModel::setDirectory);
     * @endcode
     */
    class Sidebar : public QWidget {
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

        signals:
            /**
             * @brief Emitted when the user clicks a place.
             * @param path Absolute path of the selected place.
             */
            void placeSelected(const QString &path);

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
        QListWidget *m_list;
    };

} // namespace Dentry::Ui

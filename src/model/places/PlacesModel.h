/**
* @file PlacesModel.h
 * @brief Model providing sidebar place entries.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "PlaceItem.h"

#include <QStandardItemModel>

namespace dentry::model {

/**
 * @class PlacesModel
 * @brief Populates and exposes the sidebar place list.
 *
 * Provides one QStandardItem per place. Each item carries:
 * - Qt::DisplayRole — display name ("Home", folder name, "Root")
 * - Qt::UserRole    — absolute path
 *
 * Home is always first, Root always last.
 * Hidden subdirectories are included or excluded via setShowHidden().
 *
 * Example:
 * @code
 * auto *model = new PlacesModel(this);
 * listView->setModel(model);
 * model->setShowHidden(true);
 * @endcode
 */
class PlacesModel : public QStandardItemModel {
    Q_OBJECT

public:
    explicit PlacesModel(QObject *parent = nullptr);

    ~PlacesModel() override = default;

    PlacesModel(const PlacesModel &)            = delete;
    PlacesModel &operator=(const PlacesModel &) = delete;
    PlacesModel(PlacesModel &&)                 = delete;
    PlacesModel &operator=(PlacesModel &&)      = delete;

    /** @brief Reloads places respecting the current showHidden flag. */
    void refresh();

    [[nodiscard]] QModelIndex defaultIndex() const;

private:
    void populate();
};

} // namespace dentry::model

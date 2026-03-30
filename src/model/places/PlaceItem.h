/**
* @file PlaceItem.h
 * @brief Plain data structure representing a single sidebar place entry.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QString>

namespace dentry::model {

    /**
     * @struct PlaceItem
     * @brief Lightweight value type representing a place in the sidebar.
     */
    struct PlaceItem {
        QString displayName;
        QString absolutePath;
    };

} // namespace dentry::model

/**
 * @file DateFormatter.cpp
 * @brief Implementation of date formatting free functions.
 *
 * @author Hugo Fabresse
 */

#include "DateFormatter.h"

namespace dentry::formatter {

    QString formatDateTime(const QDateTime &dateTime) {
        if (!dateTime.isValid())
            return QString();

        return dateTime.toString("MM/dd/yyyy HH:mm");
    }

    QString formatDateShort(const QDateTime &dateTime) {
        if (!dateTime.isValid())
            return QString();

        return dateTime.toString("MM/dd/yyyy");
    }

} // namespace dentry::formatter

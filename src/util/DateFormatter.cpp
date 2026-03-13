/**
 * @file DateFormatter.cpp
 * @brief Implementation of DateFormatter.
 *
 * @author Hugo Fabresse
 */

#include "DateFormatter.h"

namespace Dentry::Util {

    QString DateFormatter::format(const QDateTime &dateTime) {
        if (!dateTime.isValid())
            return QString();

        return dateTime.toString("MM/dd/yyyy HH:mm");
    }

    QString DateFormatter::formatShort(const QDateTime &dateTime) {
        if (!dateTime.isValid())
            return QString();

        return dateTime.toString("MM/dd/yyyy");
    }

} // namespace Dentry::Util

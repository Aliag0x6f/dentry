/**
* @file DateFormatter.cpp
 * @brief Implementation of DateFormatter.
 *
 * @author Hugo Fabresse
 */

#include "DateFormatter.h"

namespace Dentry::Util {

    QString DateFormatter::format(const QDateTime &dateTime) {
        // TODO: implement fixed-width "dd/MM/yyyy HH:mm" formatting
        Q_UNUSED(dateTime)
        return QStringLiteral("--/--/---- --:--");
    }

    QString DateFormatter::formatShort(const QDateTime &dateTime) {
        // TODO: implement fixed-width "dd/MM/yyyy" formatting
        Q_UNUSED(dateTime)
        return QStringLiteral("--/--/----");
    }

} // namespace Dentry::Util

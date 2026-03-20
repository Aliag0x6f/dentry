/**
 * @file DateFormatter.h
 * @brief Free functions for formatting QDateTime values into fixed-width strings.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QDateTime>
#include <QString>

namespace Dentry::formatter {

    /**
     * @brief Formats a QDateTime as a fixed-width date and time string.
     *
     * Output format: "MM/dd/yyyy HH:mm" (16 characters, always).
     * Returns an empty string if the QDateTime is invalid or null.
     *
     * @param dateTime The date and time to format.
     * @return A 16-character string or an empty string if invalid.
     */
    [[nodiscard]] QString formatDateTime(const QDateTime &dateTime);

    /**
     * @brief Formats a QDateTime as a date-only string.
     *
     * Output format: "MM/dd/yyyy" (10 characters, always).
     * Returns an empty string if the QDateTime is invalid or null.
     *
     * @param dateTime The date and time to format.
     * @return A 10-character string or an empty string if invalid.
     */
    [[nodiscard]] QString formatDateShort(const QDateTime &dateTime);

} // namespace Dentry::formatter

/**
 * @file DateFormatter.h
 * @brief Utility for formatting QDateTime values into fixed-width strings.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QDateTime>
#include <QString>

namespace Dentry::Util {

    /**
     * @class DateFormatter
     * @brief Formats QDateTime values into consistent, fixed-width strings.
     *
     * All output strings use a fixed-width format so that date columns
     * in the file list view remain visually aligned regardless of values.
     *
     * All methods are static. This class is not meant to be instantiated.
     *
     * Example output (US format):
     * @code
     * DateFormatter::format(dt)      // "03/14/2024 09:05" (MM/dd/yyyy HH:mm)
     * DateFormatter::formatShort(dt) // "03/14/2024" (MM/dd/yyyy)
     * @endcode
     */
    class DateFormatter {
    public:
        DateFormatter()  = delete;
        ~DateFormatter() = delete;

        DateFormatter(const DateFormatter &)            = delete;
        DateFormatter &operator=(const DateFormatter &) = delete;
        DateFormatter(DateFormatter &&)                 = delete;
        DateFormatter &operator=(DateFormatter &&)      = delete;

        /**
         * @brief Formats a QDateTime as a fixed-width date and time string.
         *
         * Output format: "MM/dd/yyyy HH:mm" (16 characters, always).
         * Returns an empty string if the QDateTime is invalid or null.
         *
         * @param dateTime The date and time to format.
         * @return A 16-character string or an empty string if invalid.
         */
        [[nodiscard]] static QString format(const QDateTime &dateTime);

        /**
         * @brief Formats a QDateTime as a date-only string.
         *
         * Output format: "MM/dd/yyyy" (10 characters, always).
         * Returns an empty string if the QDateTime is invalid or null.
         *
         * @param dateTime The date and time to format.
         * @return A 10-character string or an empty string if invalid.
         */
        [[nodiscard]] static QString formatShort(const QDateTime &dateTime);
    };

} // namespace Dentry::Util

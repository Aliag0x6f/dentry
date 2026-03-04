/**
* @file SizeFormatter.h
 * @brief Utility for formatting byte sizes into human-readable strings.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QString>
#include <QLocale>

namespace Dentry::Util {

    /**
     * @class SizeFormatter
     * @brief Converts raw byte counts into human-readable size strings.
     *
     * All methods are static. This class is not meant to be instantiated.
     *
     * @note Uses binary prefixes (1 K = 1024 bytes).
     *
     * Example output:
     * @code
     * SizeFormatter::format(0)           // "0 B"
     * SizeFormatter::format(1023)        // "1023 B"
     * SizeFormatter::format(1024)        // "1.0 K"
     * SizeFormatter::format(1536)        // "1.5 K"
     * SizeFormatter::format(1048576)     // "1.0 M"
     * SizeFormatter::format(1073741824)  // "1.0 G"
     * @endcode
     */
    class SizeFormatter {
    public:
        SizeFormatter()  = delete;
        ~SizeFormatter() = delete;

        SizeFormatter(const SizeFormatter &)            = delete;
        SizeFormatter &operator=(const SizeFormatter &) = delete;
        SizeFormatter(SizeFormatter &&)                 = delete;
        SizeFormatter &operator=(SizeFormatter &&)      = delete;

        /**
         * @brief Formats a byte count into a human-readable string.
         *
         * Selects the appropriate unit (B, K, M, G, T) based on magnitude.
         * Values are rounded to one decimal place for units above bytes.
         *
         * @param bytes The size in bytes. Must be >= 0.
         * @return A formatted string such as "4.2 M" or "512 B".
         */
        [[nodiscard]] static QString format(qint64 bytes);

        /**
         * @brief Formats a byte count with full unit name.
         *
         * @param bytes The size in bytes. Must be >= 0.
         * @return A formatted string such as "4.2 MB" or "512 B".
         */
        [[nodiscard]] static QString formatLong(qint64 bytes);
    };

} // namespace Dentry::Util

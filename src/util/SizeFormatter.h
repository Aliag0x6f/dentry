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
     * @note Uses IEC-style binary prefixes (1 K = 1024 bytes).
     *
     * The formatting relies on Qt locale-aware data size conversion.
     *
     * Example output:
     * @code
     * SizeFormatter::format(0)      // "0 B"
     * SizeFormatter::format(1024)   // "1.0 K"
     * SizeFormatter::format(1536)   // "1.5 K"
     * SizeFormatter::format(1048576)// "1.0 M"
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
         * @brief Formats a byte count into a human-readable string using traditional binary unit notation.
         *
         * Uses Qt locale data size formatting with 2 decimal precision and traditional unit style.
         *
         * @param bytes The size in bytes. Must be non-negative.
         * @return A formatted string such as "512 B", "1.0 K", or "4.2 M".
         */
        [[nodiscard]] static QString format(qint64 bytes);

        /**
         * @brief Formats a byte count into a human-readable string using IEC binary unit notation with full unit labels.
         *
         * Uses IEC-style unit representation (e.g., "KB", "MB", "GB").
         *
         * @param bytes The size in bytes. Must be non-negative.
         * @return A formatted string such as "512 B" or "4.2 MB".
         */
        [[nodiscard]] static QString formatLong(qint64 bytes);
    };

} // namespace Dentry::Util

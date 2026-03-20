/**
 * @file SizeFormatter.h
 * @brief Free functions for formatting byte sizes into human-readable strings.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QString>
#include <QLocale>

namespace Dentry::formatter {

    /**
     * @brief Formats a byte count into a human-readable string using traditional binary unit notation.
     *
     * Uses Qt locale data size formatting with 2 decimal precision and traditional unit style.
     *
     * @param bytes The size in bytes. Must be non-negative.
     * @return A formatted string such as "512 B", "1.0 K", or "4.2 M".
     */
    [[nodiscard]] QString formatSize(qint64 bytes);

    /**
     * @brief Formats a byte count into a human-readable string using IEC binary unit notation with full unit labels.
     *
     * Uses IEC-style unit representation (e.g., "KB", "MB", "GB").
     *
     * @param bytes The size in bytes. Must be non-negative.
     * @return A formatted string such as "512 B" or "4.2 MB".
     */
    [[nodiscard]] QString formatSizeLong(qint64 bytes);

} // namespace Dentry::formatter

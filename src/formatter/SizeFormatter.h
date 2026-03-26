/**
 * @file SizeFormatter.h
 * @brief Free functions for formatting byte sizes into human-readable strings.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QString>

namespace dentry::formatter {

    /**
     * @brief Formats a byte count into a human-readable string using traditional binary unit notation.
     *
     * Uses Qt locale data size formatting with 2 decimal precision and traditional unit style.
     *
     * @param bytes The size in bytes. Must be non-negative.
     * @return A locale-dependent formatted string with up to 2 fractional digits
     *         (traditional units, e.g. K/M/G).
     */
    [[nodiscard]] QString formatSize(qint64 bytes);

    /**
     * @brief Formats a byte count into a human-readable string using IEC binary unit notation with full unit labels.
     *
     * Uses IEC-style unit representation with binary prefixes (e.g., "KiB", "MiB", "GiB").
     *
     * @param bytes The size in bytes. Must be non-negative.
     * @return A locale-dependent formatted string with up to 2 fractional digits
     *         using IEC-style units.
     */
    [[nodiscard]] QString formatSizeLong(qint64 bytes);

} // namespace dentry::formatter

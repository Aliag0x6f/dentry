/**
 * @file PermissionFormatter.h
 * @brief Utility for formatting Qt file permissions into Unix-style strings.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QFile>
#include <QFileInfo>
#include <QString>

namespace Dentry::Util {

/**
 * @class PermissionFormatter
 * @brief Converts QFile::Permissions flags into Unix permission strings.
 *
 * All methods are static. This class is not meant to be instantiated.
 *
 * Example output:
 * @code
 * // Regular file, owner rwx, group r-x, others r-x
 * PermissionFormatter::format(info) // "-rwxr-xr-x"
 *
 * // Directory, owner rwx, group r-x, others r-x
 * PermissionFormatter::format(info) // "drwxr-xr-x"
 * @endcode
 */
class PermissionFormatter {
public:
    PermissionFormatter()  = delete;
    ~PermissionFormatter() = delete;

    PermissionFormatter(const PermissionFormatter &)            = delete;
    PermissionFormatter &operator=(const PermissionFormatter &) = delete;
    PermissionFormatter(PermissionFormatter &&)                 = delete;
    PermissionFormatter &operator=(PermissionFormatter &&)      = delete;

    /**
     * @brief Formats file permissions as a 10-character Unix-style string.
     *
     * The format is: [type][owner][group][others]
     * - type:   'd' for directory, '-' for regular file, 'l' for symlink
     * - owner:  rwx or --- based on QFile::ReadOwner/WriteOwner/ExeOwner
     * - group:  rwx or --- based on QFile::ReadGroup/WriteGroup/ExeGroup
     * - others: rwx or --- based on QFile::ReadOther/WriteOther/ExeOther
     *
     * @param info A valid QFileInfo object for the target file.
     * @return A 10-character permission string such as "-rwxr-xr-x".
     */
    [[nodiscard]] static QString format(const QFileInfo &info);

    /**
     * @brief Formats raw QFile::Permissions flags as a 9-character string.
     *
     * Does not include the file type character.
     * Useful when the type is already known and handled separately.
     *
     * @param permissions The permission flags to format.
     * @return A 9-character string such as "rwxr-xr-x".
     */
    [[nodiscard]] static QString formatRaw(QFile::Permissions permissions);
};

} // namespace Dentry::Util

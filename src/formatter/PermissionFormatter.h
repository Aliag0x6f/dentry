/**
 * @file PermissionFormatter.h
 * @brief Free functions for formatting Qt file permissions into Unix-style strings.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QFile>
#include <QFileInfo>
#include <QString>

namespace dentry::formatter {

/**
 * @brief Formats file permissions as a Unix-style string.
 *
 * For existing files, the format is: [type][owner][group][others]
 * - type:   'd' for directory, '-' for regular file, 'l' for symlink
 * - owner:  rwx or --- based on QFile::ReadOwner/WriteOwner/ExeOwner
 * - group:  rwx or --- based on QFile::ReadGroup/WriteGroup/ExeGroup
 * - others: rwx or --- based on QFile::ReadOther/WriteOther/ExeOther
 *
 * If @p info does not refer to an existing file (info.exists() is false),
 * an empty QString is returned.
 *
 * @param info A QFileInfo object for the target file.
 * @return A 10-character permission string such as "-rwxr-xr-x" for existing
 *         files, or an empty string if the file does not exist.
 */
[[nodiscard]] QString formatPermissions(const QFileInfo &info);

/**
 * @brief Formats raw QFile::Permissions flags as a 9-character string.
 *
 * Does not include the file type character.
 * Useful when the type is already known and handled separately.
 *
 * @param permissions The permission flags to format.
 * @return A 9-character string such as "rwxr-xr-x".
 */
[[nodiscard]] QString formatPermissionsRaw(QFile::Permissions permissions);

} // namespace dentry::formatter

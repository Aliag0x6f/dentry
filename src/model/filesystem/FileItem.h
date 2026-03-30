/**
* @file FileItem.h
 * @brief Plain data structure representing a single filesystem entry.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QDateTime>
#include <QFile>
#include <QString>

namespace dentry::model {

    /**
     * @struct FileItem
     * @brief Lightweight value type representing a file or directory entry.
     *
     * FileItem is a plain data struct with no methods and no Qt parent.
     * It is the core data unit passed between the filesystem, model, and UI layers.
     *
     * All fields are precomputed and cached — no lazy evaluation.
     */
    struct FileItem {
        QString            name;
        QString            absolutePath;
        QString            mimeType;
        qint64             size;
        QString            formattedSize;
        QDateTime          lastModified;
        QString            formattedDate;
        QFile::Permissions rawPermissions;
        QString            formattedPermissions;
        bool               isDir;
        bool               isHidden;
        bool               isSymlink;
    };

} // namespace dentry::model

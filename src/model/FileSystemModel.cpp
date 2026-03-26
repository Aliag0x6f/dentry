/**
 * @file FileSystemModel.cpp
 * @brief Implementation of FileSystemModel.
 *
 * @author Hugo Fabresse
 */

#include "FileSystemModel.h"
#include "fs/FileInfo.h"
#include "log/Logger.h"

#include <QDir>

namespace dentry::model {

    FileSystemModel::FileSystemModel(QObject *parent)
        : AFileSystemModel(parent) {
        setDirectory(QDir::homePath());
    }

    void FileSystemModel::setDirectory(const QString &path) {
        if (!QDir(path).exists()) {
            log::error("Model") << "Directory does not exist:" << path;
            emit errorOccurred(QString("Directory does not exist: %1").arg(path));
            return;
        }

        log::info("Model") << "Loading directory:" << path;

        m_currentPath = path;

        QDir dir(path);
        QDir::Filters filters = QDir::AllEntries | QDir::NoDotAndDotDot | QDir::System;

        if (m_showHidden)
            filters |= QDir::Hidden;

        const QFileInfoList rawEntries = dir.entryInfoList(filters);

        QList<FileItem> entries;
        entries.reserve(rawEntries.count());

        for (const QFileInfo &fi : rawEntries) {
            if (!m_filter.isEmpty() && !fi.fileName().contains(m_filter, Qt::CaseInsensitive))
                continue;

            const fs::FileInfo info(fi);

            FileItem item;
            item.name                 = info.name();
            item.absolutePath         = info.absolutePath();
            item.mimeType             = info.mimeType();
            item.size                 = info.size();
            item.formattedSize        = info.formattedSize();
            item.lastModified         = info.lastModified();
            item.formattedDate        = info.formattedDate();
            item.rawPermissions       = info.rawPermissions();
            item.formattedPermissions = info.formattedPermissions();
            item.isDir                = info.isDir();
            item.isHidden             = info.isHidden();
            item.isSymlink            = info.isSymlink();

            entries.append(item);
        }

        // Sort: directories first, then alphabetically
        std::sort(entries.begin(), entries.end(), [](const FileItem &a, const FileItem &b) {
            if (a.isDir != b.isDir)
                return a.isDir > b.isDir;
            return a.name.toLower() < b.name.toLower();
        });

        beginResetModel();
        m_entries = std::move(entries);
        endResetModel();

        log::info("Model") << "Loaded" << m_entries.count() << "entries in" << path;
        emit directoryLoaded(m_currentPath);
    }

    void FileSystemModel::refresh() {
        if (m_currentPath.isEmpty()) {
            log::warn("Model") << "refresh() called with empty path";
            emit errorOccurred(QStringLiteral("No directory set to refresh"));
            return;
        }

        log::debug("Model") << "Refreshing:" << m_currentPath;
        setDirectory(m_currentPath);
    }

} // namespace dentry::model

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
    : QAbstractItemModel(parent) {
    setDirectory(QDir::homePath());
}

// ── Domain API ────────────────────────────────────────────────────────────────

void FileSystemModel::setDirectory(const QString &path) {
    if (!QDir(path).exists()) {
        log::error("Model") << "Directory does not exist:" << path;
        emit errorOccurred(QString("Directory does not exist: %1").arg(path));
        return;
    }

    log::info("Model") << "Loading directory:" << path;
    m_currentPath = path;
    populate(path);
}

void FileSystemModel::refresh() {
    if (m_currentPath.isEmpty()) {
        log::warn("Model") << "refresh() called with empty path";
        emit errorOccurred(QStringLiteral("No directory set to refresh"));
        return;
    }

    log::debug("Model") << "Refreshing:" << m_currentPath;
    populate(m_currentPath);
}

void FileSystemModel::setShowHidden(bool show) {
    log::debug("Model") << "ShowHidden changed to:" << show;
    m_showHidden = show;
    refresh();
}

void FileSystemModel::setFilter(const QString &filter) {
    log::debug("Model") << "Filter changed to:" << filter;
    m_filter = filter;
    refresh();
}

// ── Private ───────────────────────────────────────────────────────────────────

void FileSystemModel::populate(const QString &path) {
    QDir::Filters filters = QDir::AllEntries | QDir::NoDotAndDotDot | QDir::System;
    if (m_showHidden)
        filters |= QDir::Hidden;

    const QFileInfoList rawEntries = QDir(path).entryInfoList(filters);

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

// ── QAbstractItemModel ────────────────────────────────────────────────────────

QModelIndex FileSystemModel::index(int row, int column, const QModelIndex &parent) const {
    if (parent.isValid() || row < 0 || row >= m_entries.count()
        || column < 0 || column >= ColumnCount)
        return {};
    return createIndex(row, column);
}

QModelIndex FileSystemModel::parent(const QModelIndex &) const {
    return {};
}

int FileSystemModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : m_entries.count();
}

int FileSystemModel::columnCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : ColumnCount;
}

QVariant FileSystemModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_entries.count())
        return {};

    const FileItem &item = m_entries.at(index.row());

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case Name:         return item.name;
            case Size:         return item.formattedSize;
            case Type:         return item.mimeType;
            case DateModified: return item.formattedDate;
            case Permissions:  return item.formattedPermissions;
            default:           return {};
        }
    }

    if (role == Qt::TextAlignmentRole && index.column() == Size)
        return QVariant(Qt::AlignRight | Qt::AlignVCenter);

    return {};
}

QVariant FileSystemModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return {};

    switch (section) {
        case Name:         return "Name";
        case Size:         return "Size";
        case Type:         return "Type";
        case DateModified: return "Date Modified";
        case Permissions:  return "Permissions";
        default:           return {};
    }
}

} // namespace dentry::model

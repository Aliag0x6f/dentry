/**
 * @file FileInfo.h
 * @brief Wrapper around QFileInfo with additional computed fields.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QFileInfo>
#include <QString>

namespace Dentry::Fs {

class FileInfo {
public:
    explicit FileInfo(const QString &path);
    explicit FileInfo(const QFileInfo &fileInfo);

    ~FileInfo()                           = default;
    FileInfo(const FileInfo &)            = default;
    FileInfo &operator=(const FileInfo &) = default;
    FileInfo(FileInfo &&)                 = default;
    FileInfo &operator=(FileInfo &&)      = default;

    // ── Identity ──────────────────────────────────────────────────────────

    /** @brief Returns the file or directory name (without path). */
    [[nodiscard]] const QString &name()         const { return m_name; }

    /** @brief Returns the absolute path of the file. */
    [[nodiscard]] const QString &absolutePath() const { return m_absolutePath; }

    // ── Type ──────────────────────────────────────────────────────────────

    /** @brief Returns whether this entry is a directory. */
    [[nodiscard]] bool isDir()     const { return m_fileInfo.isDir(); }

    /** @brief Returns whether this entry is a symbolic link. */
    [[nodiscard]] bool isSymlink() const { return m_fileInfo.isSymLink(); }

    /** @brief Returns whether this is a hidden file (name starts with '.'). */
    [[nodiscard]] bool isHidden()  const { return m_fileInfo.isHidden(); }

    /** @brief Returns the resolved MIME type string. */
    [[nodiscard]] const QString &mimeType() const { return m_mimeType; }

    // ── Size ──────────────────────────────────────────────────────────────

    /** @brief Returns the raw size in bytes, or 0 for directories. */
    [[nodiscard]] qint64 size() const { return m_size; }

    /** @brief Returns the preformatted human-readable size string. */
    [[nodiscard]] const QString &formattedSize() const { return m_formattedSize; }

    // ── Date ──────────────────────────────────────────────────────────────

    /** @brief Returns the last modification date and time. */
    [[nodiscard]] const QDateTime &lastModified() const { return m_lastModified; }

    /** @brief Returns the preformatted fixed-width date string. */
    [[nodiscard]] const QString &formattedDate() const { return m_formattedDate; }

    // ── Permissions ───────────────────────────────────────────────────────

    /** @brief Returns the raw Qt permission flags. */
    [[nodiscard]] QFile::Permissions rawPermissions() const { return m_fileInfo.permissions(); }

    /** @brief Returns the preformatted Unix-style permission string. */
    [[nodiscard]] const QString &formattedPermissions() const { return m_formattedPermissions; }

    // ── Raw access ────────────────────────────────────────────────────────

    /** @brief Returns the underlying QFileInfo. Use only when strictly necessary. */
    [[nodiscard]] const QFileInfo &raw() const { return m_fileInfo; }

private:
    QFileInfo m_fileInfo;
    QString   m_name;
    QString   m_absolutePath;
    QString   m_mimeType;
    qint64    m_size;
    QString   m_formattedSize;
    QDateTime m_lastModified;
    QString   m_formattedDate;
    QString   m_formattedPermissions;

    /** @brief Computes and caches all derived fields. Called once at construction. */
    void compute();
};

} // namespace Dentry::Fs
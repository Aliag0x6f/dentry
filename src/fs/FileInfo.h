/**
 * @file FileInfo.h
 * @brief Wrapper around QFileInfo with additional computed fields.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QString>

namespace Dentry::Fs {

/**
 * @class FileInfo
 * @brief Enriched wrapper around QFileInfo.
 *
 * Extends QFileInfo with precomputed fields used throughout the application:
 * MIME type, human-readable size, formatted date, and permission string.
 *
 * All expensive computations (MIME resolution, formatting) are performed
 * once at construction time and cached as members.
 *
 * Instances are cheap to copy since all members are value types.
 *
 * @note This class does not inherit QFileInfo — it wraps it by composition
 *       to avoid exposing irrelevant QFileInfo API surface.
 *
 * Example:
 * @code
 * Dentry::Fs::FileInfo info("/home/user/document.pdf");
 * qDebug() << info.mimeType();       // "application/pdf"
 * qDebug() << info.formattedSize();  // "4.2 M"
 * qDebug() << info.formattedDate();  // "14/03/2024 09:05"
 * qDebug() << info.permissions();    // "-rw-r--r--"
 * @endcode
 */
class FileInfo {
public:
    /**
     * @brief Constructs a FileInfo for the given path.
     *
     * Resolves MIME type and precomputes all formatted fields.
     *
     * @param path Absolute path to the file or directory.
     */
    explicit FileInfo(const QString &path);

    /**
     * @brief Constructs a FileInfo from an existing QFileInfo.
     *
     * Avoids a redundant stat() call if QFileInfo is already available.
     *
     * @param fileInfo A valid QFileInfo object.
     */
    explicit FileInfo(const QFileInfo &fileInfo);

    ~FileInfo()                            = default;
    FileInfo(const FileInfo &)             = default;
    FileInfo &operator=(const FileInfo &)  = default;
    FileInfo(FileInfo &&)                  = default;
    FileInfo &operator=(FileInfo &&)       = default;

    // ── Identity ──────────────────────────────────────────────────────────

    /**
     * @brief Returns the file or directory name (without path).
     * @return The base name, e.g. "document.pdf".
     */
    [[nodiscard]] const QString &name() const;

    /**
     * @brief Returns the absolute path of the file.
     * @return Absolute path, e.g. "/home/user/document.pdf".
     */
    [[nodiscard]] const QString &absolutePath() const;

    // ── Type ──────────────────────────────────────────────────────────────

    /**
     * @brief Returns whether this entry is a directory.
     */
    [[nodiscard]] bool isDir() const;

    /**
     * @brief Returns whether this entry is a symbolic link.
     */
    [[nodiscard]] bool isSymlink() const;

    /**
     * @brief Returns whether this is a hidden file (name starts with '.').
     */
    [[nodiscard]] bool isHidden() const;

    /**
     * @brief Returns the resolved MIME type string.
     * @return A MIME type such as "application/pdf" or "inode/directory".
     */
    [[nodiscard]] const QString &mimeType() const;

    // ── Size ──────────────────────────────────────────────────────────────

    /**
     * @brief Returns the raw size in bytes.
     * @return Size in bytes, or 0 for directories.
     */
    [[nodiscard]] qint64 size() const;

    /**
     * @brief Returns the preformatted human-readable size string.
     * @return A string such as "4.2 M" or an empty string for directories.
     */
    [[nodiscard]] const QString &formattedSize() const;

    // ── Date ──────────────────────────────────────────────────────────────

    /**
     * @brief Returns the last modification date and time.
     */
    [[nodiscard]] const QDateTime &lastModified() const;

    /**
     * @brief Returns the preformatted fixed-width date string.
     * @return A 16-character string such as "14/03/2024 09:05".
     */
    [[nodiscard]] const QString &formattedDate() const;

    // ── Permissions ───────────────────────────────────────────────────────

    /**
     * @brief Returns the raw Qt permission flags.
     */
    [[nodiscard]] QFile::Permissions rawPermissions() const;

    /**
     * @brief Returns the preformatted Unix-style permission string.
     * @return A 10-character string such as "-rw-r--r--".
     */
    [[nodiscard]] const QString &formattedPermissions() const;

    // ── Raw access ────────────────────────────────────────────────────────

    /**
     * @brief Returns the underlying QFileInfo.
     *
     * Use only when raw QFileInfo access is strictly necessary.
     */
    [[nodiscard]] const QFileInfo &raw() const;

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

    /**
     * @brief Computes and caches all derived fields.
     *
     * Called once at construction time.
     */
    void compute();
};

} // namespace Dentry::Fs

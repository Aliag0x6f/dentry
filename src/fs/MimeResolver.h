/**
 * @file MimeResolver.h
 * @brief Wrapper around QMimeDatabase for MIME type resolution.
 *
 * @author Hugo Fabresse
 */

#pragma once

class QFileInfo;
class QMimeDatabase;
class QString;

namespace dentry::fs {

/**
 * @class MimeResolver
 * @brief Resolves MIME types for files and directories.
 *
 * Wraps QMimeDatabase to isolate MIME resolution logic from the rest
 * of the application. A single QMimeDatabase instance is shared across
 * all calls — Qt documents this as the correct usage pattern since
 * QMimeDatabase caches its database internally.
 *
 * MimeResolver is a non-copyable, non-movable singleton-style utility.
 * All resolution is done through the static interface.
 *
 * Example:
 * @code
 * QString mime = dentry::fs::MimeResolver::resolve("/home/user/photo.jpg");
 * // mime == "image/jpeg"
 *
 * QString mime = dentry::fs::MimeResolver::resolve("/home/user/");
 * // mime == "inode/directory"
 * @endcode
 */
class MimeResolver {
public:
    MimeResolver()  = delete;
    ~MimeResolver() = delete;

    MimeResolver(const MimeResolver &)            = delete;
    MimeResolver &operator=(const MimeResolver &) = delete;
    MimeResolver(MimeResolver &&)                 = delete;
    MimeResolver &operator=(MimeResolver &&)      = delete;

    /**
     * @brief Resolves the MIME type of a file by content and name.
     *
     * Uses QMimeDatabase::MatchDefault which checks both file name
     * and file content (magic bytes). Falls back to name-only matching
     * if the file is not readable.
     *
     * @param path Absolute path to the file or directory.
     * @return A MIME type string such as "image/jpeg" or "inode/directory".
     *         Returns "application/octet-stream" if resolution fails.
     */
    [[nodiscard]] static QString resolve(const QString &path);

    /**
     * @brief Resolves the MIME type from a QFileInfo.
     *
     * Preferred overload when QFileInfo is already available,
     * avoids constructing it twice.
     *
     * @param info A valid QFileInfo object.
     * @return A MIME type string.
     */
    [[nodiscard]] static QString resolve(const QFileInfo &info);

    /**
     * @brief Returns the generic icon name associated with a MIME type.
     *
     * Useful for selecting an icon to display in the file list.
     *
     * @param mimeType A MIME type string such as "image/jpeg".
     * @return A freedesktop icon name such as "image-x-generic".
     */
    [[nodiscard]] static QString iconName(const QString &mimeType);

private:
    /**
     * @brief Returns the shared QMimeDatabase instance.
     *
     * Constructed once on first use. Thread-safe in Qt.
     *
     * @return Reference to the shared QMimeDatabase.
     */
    [[nodiscard]] static const QMimeDatabase &database();
};

} // namespace dentry::fs

/**
 * @file Clipboard.h
 * @brief Internal clipboard for file operations.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QStringList>

namespace dentry::app {

/**
 * @class Clipboard
 * @brief Holds the paths of files pending a copy or move operation.
 *
 * The clipboard stores a list of absolute paths and a flag indicating
 * whether the operation is a copy or a cut. It is populated by the user
 * via Copy/Cut actions and consumed by Paste.
 *
 * This class has no Qt dependency beyond QStringList and is not a QObject.
 * It is intended to be held as a value member of MainWindow.
 *
 * Example:
 * @code
 * dentry::app::Clipboard clipboard;
 * clipboard.copy({ "/home/user/file.txt" });
 *
 * if (!clipboard.isEmpty()) {
 *     if (clipboard.isCut())
 *         // MoveOperation
 *     else
 *         // CopyOperation
 *     clipboard.clear();
 * }
 * @endcode
 */
class Clipboard {
public:
    Clipboard()  = default;
    ~Clipboard() = default;

    Clipboard(const Clipboard &)            = default;
    Clipboard &operator=(const Clipboard &) = default;
    Clipboard(Clipboard &&)                 = default;
    Clipboard &operator=(Clipboard &&)      = default;

    /**
     * @brief Stores paths for a copy operation.
     *
     * Replaces any previously stored paths.
     *
     * @param paths Absolute paths of the items to copy.
     */
    void copy(const QStringList &paths);

    /**
     * @brief Stores paths for a cut (move) operation.
     *
     * Replaces any previously stored paths.
     *
     * @param paths Absolute paths of the items to cut.
     */
    void cut(const QStringList &paths);

    /**
     * @brief Clears all stored paths and resets the cut flag.
     */
    void clear();

    /**
     * @brief Returns the stored paths.
     * @return List of absolute paths, empty if clipboard is empty.
     */
    [[nodiscard]] const QStringList &paths() const;

    /**
     * @brief Returns whether the clipboard was filled by a cut operation.
     * @return True if the operation is a move, false if it is a copy.
     */
    [[nodiscard]] bool isCut() const;

    /**
     * @brief Returns whether the clipboard is empty.
     * @return True if no paths are stored.
     */
    [[nodiscard]] bool isEmpty() const;

private:
    QStringList m_paths;
    bool        m_isCut = false;
};

} // namespace dentry::app

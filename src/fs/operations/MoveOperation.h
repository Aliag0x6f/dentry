/**
 * @file MoveOperation.h
 * @brief Concrete implementation of a file move operation.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "fs/AFileOperation.h"

#include <QFuture>
#include <QString>
#include <QStringList>

namespace dentry::fs {

/**
 * @class MoveOperation
 * @brief Moves one or more files or directories asynchronously.
 *
 * Attempts a simple rename first (same filesystem, instant).
 * Falls back to copy + delete if source and destination are on
 * different filesystems.
 *
 * Emits progress() during the operation and finished() on completion or error.
 * Supports cancellation via cancel().
 *
 * Example:
 * @code
 * auto op = std::make_unique<MoveOperation>(sources, destination, this);
 * connect(op.get(), &MoveOperation::finished, this, &MainWindow::onOperationFinished);
 * op->execute();
 * op.release(); // Qt parent now owns the operation.
 * @endcode
 */
class MoveOperation final : public AFileOperation {
    Q_OBJECT

public:
    /**
     * @brief Constructs a MoveOperation.
     *
     * @param sources     List of absolute paths to move.
     * @param destination Absolute path of the destination directory.
     * @param parent      Optional Qt parent object.
     */
    explicit MoveOperation(const QStringList &sources,
                           const QString     &destination,
                           QObject           *parent = nullptr);

    ~MoveOperation() override = default;

    MoveOperation(const MoveOperation &)            = delete;
    MoveOperation &operator=(const MoveOperation &) = delete;
    MoveOperation(MoveOperation &&)                 = delete;
    MoveOperation &operator=(MoveOperation &&)      = delete;

    /**
     * @brief Starts the move asynchronously.
     *
     * Returns immediately. Progress is reported via signals.
     */
    void execute() override;

    /**
     * @brief Returns a human-readable description of this operation.
     * @return A string such as "Moving 3 items to /home/user/Downloads".
     */
    [[nodiscard]] QString description() const override;

private:
    /**
     * @brief Attempts to move a single entry.
     *
     * Tries rename() first. Falls back to copy + delete on failure.
     *
     * @param source      Absolute path of the source.
     * @param destination Absolute path of the destination.
     * @return True on success, false on any error or cancellation.
     */
    bool moveEntry(const QString &source, const QString &destination);

    /**
     * @brief Recursively copies a directory then deletes the source.
     *
     * Used as fallback when rename() fails across filesystems.
     *
     * @param source      Absolute path of the source directory.
     * @param destination Absolute path of the destination directory.
     * @return True on success, false on any error or cancellation.
     */
    bool copyThenDelete(const QString &source, const QString &destination);

    QStringList   m_sources;
    QString       m_destination;
    QFuture<void> m_future;
};

} // namespace dentry::fs

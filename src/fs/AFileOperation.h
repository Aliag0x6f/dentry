/**
 * @file AFileOperation.h
 * @brief Abstract base class for asynchronous file operations.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "IFileOperation.h"
#include <QObject>
#include <QString>
#include <atomic>

namespace dentry::fs {

/**
 * @class AFileOperation
 * @brief Abstract Qt-based implementation of IFileOperation.
 *
 * Adds Qt signal/slot infrastructure on top of IFileOperation.
 * Provides shared state management (running flag, cancellation)
 * and common signal definitions used by all concrete operations.
 *
 * Concrete operations inherit from this class and implement:
 * - execute()     — the actual filesystem logic
 * - description() — a human-readable label
 *
 * @note Inherits both IFileOperation and QObject. The QObject
 *       inheritance must always come first in the list.
 */
class AFileOperation : public QObject, public IFileOperation {
    Q_OBJECT

public:
    explicit AFileOperation(QObject *parent = nullptr);

    ~AFileOperation() override = default;

    AFileOperation(const AFileOperation &)            = delete;
    AFileOperation &operator=(const AFileOperation &) = delete;
    AFileOperation(AFileOperation &&)                 = delete;
    AFileOperation &operator=(AFileOperation &&)      = delete;

    /**
     * @brief Cancels the operation.
     *
     * Sets the internal cancellation flag. Concrete implementations
     * must check isCancelled() regularly during execution.
     */
    void cancel() override;

    /**
     * @brief Returns whether the operation is currently running.
     */
    [[nodiscard]] bool isRunning() const override;

signals:
    /**
     * @brief Emitted periodically during execution.
     *
     * @param percent Completion percentage in range [0, 100].
     */
    void progress(int percent);

    /**
     * @brief Emitted when the operation completes or fails.
     *
     * @param success True if the operation completed without error.
     * @param error   Human-readable error message, empty on success.
     */
    void finished(bool success, const QString &error);

protected:
    /**
     * @brief Returns whether cancellation has been requested.
     *
     * Concrete implementations must call this regularly and
     * abort cleanly if it returns true.
     */
    [[nodiscard]] bool isCancelled() const;

    /**
     * @brief Sets the running state.
     *
     * Called by concrete implementations at start and end of execute().
     *
     * @param running True when starting, false when done.
     */
    void setRunning(bool running);

private:
    std::atomic_bool m_running   = false;
    std::atomic_bool m_cancelled = false;
};

} // namespace dentry::fs

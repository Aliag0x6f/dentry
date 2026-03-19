/**
 * @file DeleteOperation.h
 * @brief Concrete implementation of a file delete operation.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "../AFileOperation.h"
#include <QFuture>
#include <QString>
#include <QStringList>

namespace Dentry::Fs {

/**
 * @class DeleteOperation
 * @brief Deletes one or more files or directories asynchronously.
 *
 * Emits progress() during deletion and finished() on completion or error.
 * Supports cancellation via cancel().
 *
 * Example:
 * @code
 * auto op = std::make_unique<DeleteOperation>(targets, this);
 * connect(op.get(), &DeleteOperation::progress, progressDialog, &ProgressDialog::onProgress);
 * connect(op.get(), &DeleteOperation::finished, this, &MainWindow::onOperationFinished);
 * op->execute();
 * op.release(); // Qt parent now owns the operation.
 * @endcode
 */
class DeleteOperation final : public AFileOperation {
    Q_OBJECT

public:
    /**
     * @brief Constructs a DeleteOperation.
     *
     * @param targets List of absolute paths to delete.
     * @param parent  Optional Qt parent object.
     */
    explicit DeleteOperation(const QStringList &targets,
                             QObject           *parent = nullptr);

    ~DeleteOperation() override = default;

    DeleteOperation(const DeleteOperation &)            = delete;
    DeleteOperation &operator=(const DeleteOperation &) = delete;
    DeleteOperation(DeleteOperation &&)                 = delete;
    DeleteOperation &operator=(DeleteOperation &&)      = delete;

    /**
     * @brief Starts the deletion asynchronously.
     *
     * Returns immediately. Progress is reported via signals.
     */
    void execute() override;

    /**
     * @brief Returns a human-readable description of this operation.
     * @return A string such as "Deleting 3 items".
     */
    [[nodiscard]] QString description() const override;

private:
    /**
     * @brief Deletes a single entry (file or directory).
     *
     * Directories are deleted recursively.
     *
     * @param path Absolute path of the entry to delete.
     * @return True on success, false on any error.
     */
    bool deleteEntry(const QString &path);

    QStringList   m_targets;
    QFuture<void> m_future;
};

} // namespace Dentry::Fs

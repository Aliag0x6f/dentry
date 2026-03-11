/**
 * @file RenameOperation.h
 * @brief Concrete implementation of a file rename operation.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "../AFileOperation.h"
#include <QFuture>
#include <QString>

namespace Dentry::Fs {

/**
 * @class RenameOperation
 * @brief Renames a single file or directory asynchronously.
 *
 * Emits progress() during the operation and finished() on completion or error.
 * Supports cancellation via cancel().
 *
 * Example:
 * @code
 * auto *op = new RenameOperation(source, newName, parent);
 * connect(op, &RenameOperation::progress, progressDialog, &ProgressDialog::onProgress);
 * connect(op, &RenameOperation::finished, this, &MainWindow::onOperationFinished);
 * op->execute();
 * @endcode
 */
class RenameOperation final : public AFileOperation {
    Q_OBJECT

public:
    /**
     * @brief Constructs a RenameOperation.
     *
     * @param source  Absolute path of the file or directory to rename.
     * @param newName The new name (not a full path, just the name).
     * @param parent  Optional Qt parent object.
     */
    explicit RenameOperation(const QString &source,
                             const QString &newName,
                             QObject       *parent = nullptr);

    ~RenameOperation() override = default;

    RenameOperation(const RenameOperation &)            = delete;
    RenameOperation &operator=(const RenameOperation &) = delete;
    RenameOperation(RenameOperation &&)                 = delete;
    RenameOperation &operator=(RenameOperation &&)      = delete;

    /**
     * @brief Starts the rename asynchronously.
     *
     * Returns immediately. Progress is reported via signals.
     */
    void execute() override;

    /**
     * @brief Returns a human-readable description of this operation.
     * @return A string such as "Renaming file.txt to newname.txt".
     */
    [[nodiscard]] QString description() const override;

private:
    /**
     * @brief Rename a single entry (file or directory).
     *
     * @param path Absolute path of the entry to rename.
     * @return True on success, false on any error.
     */
    bool renameEntry(const QString &path);

    QString       m_source;
    QString       m_newName;
    QFuture<void> m_future;
};

} // namespace Dentry::Fs

/**
 * @file CopyOperation.h
 * @brief Concrete implementation of a file copy operation.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "../AFileOperation.h"
#include <QString>
#include <QStringList>
#include <QFuture>

namespace dentry::fs {

    /**
     * @class CopyOperation
     * @brief Copies one or more files or directories asynchronously.
     *
     * Emits progress() during the copy and finished() on completion or error.
     * Supports cancellation via cancel().
     *
     * Example:
     * @code
     * auto op = std::make_unique<CopyOperation>(sources, destination, this);
     * connect(op.get(), &CopyOperation::progress, progressDialog, &ProgressDialog::onProgress);
     * connect(op.get(), &CopyOperation::finished, this, &MainWindow::onOperationFinished);
     * op->execute();
     * op.release(); // Qt parent now owns the operation.
     * @endcode
     */
    class CopyOperation final : public AFileOperation {
        Q_OBJECT

    public:
        explicit CopyOperation(const QStringList &sources,
                               const QString     &destination,
                               QObject           *parent = nullptr);

        ~CopyOperation() override = default;

        CopyOperation(const CopyOperation &)            = delete;
        CopyOperation &operator=(const CopyOperation &) = delete;
        CopyOperation(CopyOperation &&)                 = delete;
        CopyOperation &operator=(CopyOperation &&)      = delete;

        void execute() override;

        [[nodiscard]] QString description() const override;

    private:
        /**
         * @brief Recursively copies a directory and its contents.
         *
         * Checks isCancelled() between each entry.
         *
         * @param source      Absolute path of the source directory.
         * @param destination Absolute path of the destination directory.
         * @return True on success, false on any error or cancellation.
         */
        bool copyDir(const QString &source, const QString &destination);

        QStringList   m_sources;
        QString       m_destination;
        QFuture<void> m_future;
    };

} // namespace dentry::fs

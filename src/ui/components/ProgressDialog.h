/**
 * @file ProgressDialog.h
 * @brief Dialog displaying the progress of a file operation.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "../AUIComponent.h"
#include "../../fs/AFileOperation.h"

#include <QDialog>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>

namespace Dentry::Ui {

    /**
     * @class ProgressDialog
     * @brief Modal dialog that tracks and displays a file operation's progress.
     *
     * Connects to an AFileOperation's signals to update the progress bar
     * and description label. Provides a cancel button to abort the operation.
     *
     * Example:
     * @code
     * auto *op = new CopyOperation(sources, destination, this);
     * auto *dialog = new ProgressDialog(op, this);
     * op->execute();
     * dialog->exec();
     * @endcode
     */
    class ProgressDialog : public QDialog, public AUIComponent {
        Q_OBJECT

    public:
        /**
         * @brief Constructs a ProgressDialog for the given operation.
         * @param operation The file operation to track. Must not be null.
         * @param parent    Optional Qt parent widget.
         */
        explicit ProgressDialog(Fs::AFileOperation *operation, QWidget *parent = nullptr);

        ~ProgressDialog() override = default;

        ProgressDialog(const ProgressDialog &)            = delete;
        ProgressDialog &operator=(const ProgressDialog &) = delete;
        ProgressDialog(ProgressDialog &&)                 = delete;
        ProgressDialog &operator=(ProgressDialog &&)      = delete;

        void build() override;

    protected:
        void setupSize()        override;
        void setupConnections() override;

    private slots:
        /**
         * @brief Updates the progress bar.
         * @param percent Completion percentage in range [0, 100].
         */
        void onProgress(int percent);

        /**
         * @brief Handles operation completion.
         *
         * Closes the dialog on success or displays an error message on failure.
         *
         * @param success True if the operation completed without error.
         * @param error   Human-readable error message, empty on success.
         */
        void onFinished(bool success, const QString &error);

        /**
         * @brief Cancels the operation and closes the dialog.
         */
        void onCancelled();

    private:
        Fs::AFileOperation *m_operation;
        QLabel             *m_descriptionLabel;
        QProgressBar       *m_progressBar;
        QPushButton        *m_cancelButton;
    };

} // namespace Dentry::Ui

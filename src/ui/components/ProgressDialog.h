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
#include <QPointer>
#include <QProgressBar>
#include <QPushButton>

namespace dentry::ui {

    /**
     * @class ProgressDialog
     * @brief Modal dialog that tracks and displays a file operation's progress.
     *
     * Connects to an AFileOperation's signals to update the progress bar
     * and description label.
     *
     * During execution, Cancel requests operation cancellation. Once the
     * operation reaches a terminal state (success/cancel/error), the dialog
     * remains open until the user explicitly chooses Finished or Canceled.
     *
     * Example:
     * @code
     * auto op = std::make_unique<CopyOperation>(sources, destination);
     * ProgressDialog dialog(op.get(), this);
     * op->execute();
     * dialog.exec();
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
        explicit ProgressDialog(fs::AFileOperation *operation, QWidget *parent = nullptr);

        ~ProgressDialog() override = default;

        ProgressDialog(const ProgressDialog &)            = delete;
        ProgressDialog &operator=(const ProgressDialog &) = delete;
        ProgressDialog(ProgressDialog &&)                 = delete;
        ProgressDialog &operator=(ProgressDialog &&)      = delete;

        void build() override;

    protected:
        /**
         * @brief Intercepts close/escape requests.
         *
         * While running, close requests are treated as cancellation requests.
         * The dialog only closes through explicit Finished/Canceled buttons.
         */
        void reject() override;
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
         * Updates the dialog state for success/cancel/error and enables
         * explicit user acknowledgment buttons.
         *
         * @param success True if the operation completed without error.
         * @param error   Human-readable error message, empty on success.
         */
        void onFinished(bool success, const QString &error);

        /**
         * @brief Requests cancellation, or acknowledges canceled state if done.
         */
        void onCancelled();

        /**
         * @brief Closes the dialog as a successful user acknowledgment.
         */
        void onFinishedClicked();

    private:
        // Non-owning observer. Ownership is provided externally (typically by Qt parent-child).
        QPointer<fs::AFileOperation> m_operation;
        QPointer<QLabel>             m_descriptionLabel;
        QPointer<QProgressBar>       m_progressBar;
        QPointer<QPushButton>        m_finishedButton;
        QPointer<QPushButton>        m_cancelButton;
        bool                m_cancelRequested  = false;
        bool                m_finishedState    = false;

        /**
         * @brief Updates dialog UI to show completion state and enable user choice.
         *
         * Called when the operation finishes. Sets appropriate description and
         * progress bar state based on success/cancellation/error, then enables
         * the Finished and Canceled buttons for user acknowledgment.
         *
         * @param success   True if the operation completed without error.
         * @param cancelled True if the operation ended due to cancellation.
         * @param error     Human-readable error message, empty on success.
         */
        void completeAndRequireChoice(bool success, bool cancelled, const QString &error);

        /**
         * @brief Enables the Finished and Canceled buttons for user interaction.
         *
         * Sets focus to Finished button and marks the operation as ready
         * for user choice.
         */
        void unlockChoiceUi();

        /**
         * @brief Determines whether a finished() result represents cancellation.
         *
         * Interprets the terminal operation outcome from the emitted
         * finished(success, error) arguments, independent from UI intent flags.
         *
         * @param success True if the operation reported success.
         * @param error   Terminal error message emitted by the operation.
         * @return True when the outcome corresponds to a cancellation result.
         */
        static bool isCancelledResult(bool success, const QString &error);
    };

} // namespace dentry::ui

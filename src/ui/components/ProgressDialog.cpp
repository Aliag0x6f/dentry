/**
 * @file ProgressDialog.cpp
 * @brief Implementation of ProgressDialog.
 *
 * @author Hugo Fabresse
 */

#include "ProgressDialog.h"

#include <QHBoxLayout>
#include <QtGlobal>
#include <QVBoxLayout>

namespace dentry::ui {

    bool ProgressDialog::isCancelledResult(bool success, const QString &error) {
        return !success
            && (error.compare(QStringLiteral("Operation cancelled"), Qt::CaseInsensitive) == 0
                || error.compare(QStringLiteral("Operation canceled"), Qt::CaseInsensitive) == 0);
    }

    ProgressDialog::ProgressDialog(fs::AFileOperation *operation, QWidget *parent)
        : QDialog(parent)
        , m_operation(operation) {
        Q_ASSERT(m_operation != nullptr);
        build();
    }

    void ProgressDialog::build() {
        setupSize();

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setSpacing(12);
        layout->setContentsMargins(16, 16, 16, 16);

        m_descriptionLabel = new QLabel(m_operation->description(), this);
        m_descriptionLabel->setWordWrap(true);
        layout->addWidget(m_descriptionLabel);

        m_progressBar = new QProgressBar(this);
        m_progressBar->setRange(0, 100);
        m_progressBar->setValue(0);
        layout->addWidget(m_progressBar);

        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->addStretch();

        m_cancelButton = new QPushButton(tr("Cancel"), this);
        buttonLayout->addWidget(m_cancelButton);

        m_finishedButton = new QPushButton(tr("Finished"), this);
        m_finishedButton->setEnabled(false);
        m_finishedButton->setDefault(true);
        m_finishedButton->setAutoDefault(true);
        buttonLayout->addWidget(m_finishedButton);

        layout->addLayout(buttonLayout);

        setupConnections();
    }

    void ProgressDialog::setupSize() {
        setWindowTitle("Operation in progress");
        setMinimumWidth(400);
        setModal(true);
    }

    void ProgressDialog::reject() {
        if (m_operation->isRunning() && !m_cancelRequested) {
            onCancelled();
        }
    }

    void ProgressDialog::setupConnections() {
        connect(m_operation,    &fs::AFileOperation::progress, this, &ProgressDialog::onProgress);
        connect(m_operation,    &fs::AFileOperation::finished, this, &ProgressDialog::onFinished);
        connect(m_finishedButton, &QPushButton::clicked,         this, &ProgressDialog::onFinishedClicked);
        connect(m_cancelButton, &QPushButton::clicked,         this, &ProgressDialog::onCancelled);
    }

    void ProgressDialog::onProgress(int percent) {
        m_progressBar->setValue(percent);
    }

    void ProgressDialog::onFinished(bool success, const QString &error) {
        const bool cancelled = isCancelledResult(success, error);
        completeAndRequireChoice(success, cancelled, error);
    }

    void ProgressDialog::onCancelled() {
        if (!m_operation->isRunning()) {
            m_finishedState = true;
            QDialog::reject();
            return;
        }

        if (m_cancelRequested)
            return;

        m_cancelRequested = true;
        m_operation->cancel();

        m_descriptionLabel->setText(tr("Cancelling..."));
        m_cancelButton->setEnabled(false);
        m_finishedButton->setEnabled(false);
        m_progressBar->setRange(0, 0);
    }

    void ProgressDialog::onFinishedClicked() {
        if (!m_finishedState)
            return;

        accept();
    }

    void ProgressDialog::completeAndRequireChoice(bool success, bool cancelled, const QString &error) {
        if (success) {
            m_descriptionLabel->setText(tr("Operation finished successfully."));
            m_progressBar->setRange(0, 100);
            m_progressBar->setValue(100);
        } else if (cancelled) {
            m_descriptionLabel->setText(tr("Operation cancelled."));
            m_progressBar->setRange(0, 100);
            m_progressBar->setValue(0);
        }  else {
            m_descriptionLabel->setText(tr("Error: %1").arg(error));
            m_progressBar->setRange(0, 100);
            m_progressBar->setValue(0);
        }

        m_finishedButton->setText(tr("Finished"));
        m_cancelButton->setText(tr("Cancelled"));

        unlockChoiceUi();
    }

    void ProgressDialog::unlockChoiceUi() {
        m_finishedState = true;

        m_finishedButton->setEnabled(true);
        m_finishedButton->setFocus(Qt::TabFocusReason);

        m_cancelButton->setEnabled(true);
    }

} // namespace dentry::ui

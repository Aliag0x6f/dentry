/**
 * @file ProgressDialog.cpp
 * @brief Implementation of ProgressDialog.
 *
 * @author Hugo Fabresse
 */

#include "ProgressDialog.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

namespace Dentry::Ui {

    ProgressDialog::ProgressDialog(Fs::AFileOperation *operation, QWidget *parent)
        : QDialog(parent)
        , m_operation(operation) {
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

        layout->addLayout(buttonLayout);

        setupConnections();
    }

    void ProgressDialog::setupSize() {
        setWindowTitle("Operation in progress");
        setMinimumWidth(400);
        setModal(true);
    }

    void ProgressDialog::reject() {
        if (!m_cancelRequested && m_operation->isRunning()) {
            onCancelled();
            return;
        }

        QDialog::reject();
    }

    void ProgressDialog::setupConnections() {
        connect(m_operation,    &Fs::AFileOperation::progress, this, &ProgressDialog::onProgress);
        connect(m_operation,    &Fs::AFileOperation::finished, this, &ProgressDialog::onFinished);
        connect(m_cancelButton, &QPushButton::clicked,         this, &ProgressDialog::onCancelled);
    }

    void ProgressDialog::onProgress(int percent) {
        m_progressBar->setValue(percent);
    }

    void ProgressDialog::onFinished(bool success, const QString &error) {
        if (m_cancelRequested) {
            QDialog::reject();
            return;
        }

        if (success) {
            accept();
            return;
        }

        m_descriptionLabel->setText(QString("Error: %1").arg(error));
        m_progressBar->setRange(0, 100);
        m_progressBar->setValue(0);

        m_cancelButton->setEnabled(true);
        m_cancelButton->setText(tr("Close"));

        disconnect(m_cancelButton, &QPushButton::clicked, this, &ProgressDialog::onCancelled);
        connect(m_cancelButton, &QPushButton::clicked, this, [this] { reject(); });
    }

    void ProgressDialog::onCancelled() {
        if (m_cancelRequested)
            return;

        m_cancelRequested = true;
        m_operation->cancel();

        m_descriptionLabel->setText(tr("Cancelling..."));
        m_cancelButton->setEnabled(false);
        m_progressBar->setRange(0, 0);
    }

} // namespace Dentry::Ui

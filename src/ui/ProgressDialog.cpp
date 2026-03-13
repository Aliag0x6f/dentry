/**
 * @file ProgressDialog.cpp
 * @brief Implementation of ProgressDialog.
 *
 * @author Hugo Fabresse
 */

#include "ProgressDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

namespace Dentry::Ui {

    ProgressDialog::ProgressDialog(Fs::AFileOperation *operation, QWidget *parent)
        : QDialog(parent)
        , m_operation(operation) {
        build();

        connect(m_operation,    &Fs::AFileOperation::progress, this, &ProgressDialog::onProgress);
        connect(m_operation,    &Fs::AFileOperation::finished, this, &ProgressDialog::onFinished);
        connect(m_cancelButton, &QPushButton::clicked,         this, &ProgressDialog::onCancelled);
    }

    void ProgressDialog::build() {
        setWindowTitle("Operation in progress");
        setMinimumWidth(400);
        setModal(true);

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
    }

    void ProgressDialog::onProgress(int percent) {
        m_progressBar->setValue(percent);
    }

    void ProgressDialog::onFinished(bool success, const QString &error) {
        if (success) {
            accept();
            return;
        }

        m_descriptionLabel->setText(QString("Error: %1").arg(error));
        m_progressBar->setValue(0);
        m_cancelButton->setText(tr("Close"));
    }

    void ProgressDialog::onCancelled() {
        m_operation->cancel();
        reject();
    }

} // namespace Dentry::Ui

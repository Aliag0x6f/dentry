/**
 * @file PreviewPanel.cpp
 * @brief Implementation of PreviewPanel.
 *
 * @author Hugo Fabresse
 */

#include "PreviewPanel.h"

#include <QFile>
#include <QFileInfo>
#include <QPixmap>
#include <QTextStream>
#include <QVBoxLayout>

namespace Dentry::Ui {

    PreviewPanel::PreviewPanel(QWidget *parent)
        : QWidget(parent) {
        build();
    }

    void PreviewPanel::build() {
        setupSize();
        setupStyle();

        m_layout = new QVBoxLayout(this);
        m_layout->setContentsMargins(12, 12, 12, 12);
        m_layout->setSpacing(8);

        m_nameLabel = new QLabel(this);
        m_nameLabel->setWordWrap(true);
        m_nameLabel->setAlignment(Qt::AlignCenter);
        m_layout->addWidget(m_nameLabel);

        m_metaLabel = new QLabel(this);
        m_metaLabel->setWordWrap(true);
        m_metaLabel->setAlignment(Qt::AlignCenter);
        m_layout->addWidget(m_metaLabel);

        m_stack = new QStackedWidget(this);

        m_imageLabel = new QLabel(this);
        m_imageLabel->setAlignment(Qt::AlignCenter);
        m_stack->addWidget(m_imageLabel);

        m_textEdit = new QTextEdit(this);
        m_textEdit->setReadOnly(true);
        m_stack->addWidget(m_textEdit);

        m_layout->addWidget(m_stack);
    }

    void PreviewPanel::setupSize() {
        setMinimumWidth(200);
    }

    void PreviewPanel::setupStyle() {
        setObjectName("previewPanel");
        setAttribute(Qt::WA_StyledBackground, true);
    }

    void PreviewPanel::preview(const Model::FileItem &item) {
        showMetadata(item);

        if (item.mimeType.startsWith("image/"))
            showImage(item.absolutePath);
        else if (item.mimeType.startsWith("text/"))
            showText(item.absolutePath);
        else
            m_stack->hide();
    }

    void PreviewPanel::clear() {
        m_nameLabel->clear();
        m_metaLabel->clear();
        m_textEdit->clear();
        m_imageLabel->clear();
        m_stack->hide();
    }

    void PreviewPanel::showText(const QString &path) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QTextStream stream(&file);
        m_textEdit->setPlainText(stream.read(4096));
        m_stack->setCurrentWidget(m_textEdit);
        m_stack->show();
    }

    void PreviewPanel::showImage(const QString &path) {
        QPixmap pixmap(path);
        if (pixmap.isNull())
            return;

        m_imageLabel->setPixmap(
            pixmap.scaled(m_imageLabel->width(), 200, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
        m_stack->setCurrentWidget(m_imageLabel);
        m_stack->show();
    }

    void PreviewPanel::showMetadata(const Model::FileItem &item) {
        m_nameLabel->setText(item.name);
        m_metaLabel->setText(
            QString("%1\n%2\n%3")
                .arg(item.mimeType)
                .arg(item.formattedSize)
                .arg(item.formattedDate)
        );
    }

} // namespace Dentry::Ui

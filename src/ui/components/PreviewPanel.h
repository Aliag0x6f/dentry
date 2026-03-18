/**
 * @file PreviewPanel.h
 * @brief Preview panel widget for the Dentry file manager.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "../AUIComponent.h"
#include "../../model/FileItem.h"

#include <QLabel>
#include <QStackedWidget>
#include <QTextEdit>
#include <QWidget>
#include <QVBoxLayout>

namespace Dentry::Ui {

    /**
     * @class PreviewPanel
     * @brief Displays a preview of the selected file.
     *
     * Supports plain text preview and image preview.
     * Shows file metadata (name, size, type, date) for all file types.
     *
     * Example:
     * @code
     * auto *preview = new PreviewPanel(this);
     * connect(view, &FileListView::fileActivated, preview, &PreviewPanel::preview);
     * @endcode
     */
    class PreviewPanel : public QWidget, public AUIComponent {
        Q_OBJECT

    public:
        explicit PreviewPanel(QWidget *parent = nullptr);

        ~PreviewPanel() override = default;

        PreviewPanel(const PreviewPanel &)            = delete;
        PreviewPanel &operator=(const PreviewPanel &) = delete;
        PreviewPanel(PreviewPanel &&)                 = delete;
        PreviewPanel &operator=(PreviewPanel &&)      = delete;

        void build() override;

    public slots:
        /**
         * @brief Previews the given file.
         * @param item The FileItem to preview.
         */
        void preview(const Model::FileItem &item);

        /**
         * @brief Clears the preview panel.
         */
        void clear();

    protected:
        void setupSize()  override;
        void setupStyle() override;

    private:
        /** @brief Shows a text preview of the given file. */
        void showText(const QString &path);

        /** @brief Shows an image preview of the given file. */
        void showImage(const QString &path);

        /** @brief Shows file metadata. */
        void showMetadata(const Model::FileItem &item);

        QLabel         *m_nameLabel;
        QLabel         *m_metaLabel;
        QLabel         *m_imageLabel;
        QTextEdit      *m_textEdit;
        QStackedWidget *m_stack;
        QVBoxLayout    *m_layout;
    };

} // namespace Dentry::Ui

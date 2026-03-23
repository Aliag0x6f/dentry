/**
 * @file PreviewPanel.h
 * @brief Preview panel widget for the Dentry file manager.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "../DefaultLayout.h"
#include "../UIComponent.h"
#include "../../model/FileItem.h"

#include <QLabel>
#include <QPointer>
#include <QStackedWidget>
#include <QTextEdit>
#include <QWidget>

namespace dentry::ui {

    /**
     * @class PreviewPanel
     * @brief Displays a preview of the selected file.
     *
     * Supports plain text preview and image preview.
     * Shows file metadata (name, size, type, date) for all file types.
     *
     * Example:
     * @code
     * auto preview = std::make_unique<PreviewPanel>(this);
     * connect(view, &FileListView::fileActivated, preview.get(), &PreviewPanel::preview);
     * preview.release(); // Qt parent now owns the panel.
     * @endcode
     */
    class PreviewPanel : public UIComponent<QWidget, VLayout> {
        Q_OBJECT

    public:
        explicit PreviewPanel(QWidget *parent = nullptr);

        ~PreviewPanel() override = default;

        PreviewPanel(const PreviewPanel &)            = delete;
        PreviewPanel &operator=(const PreviewPanel &) = delete;
        PreviewPanel(PreviewPanel &&)                 = delete;
        PreviewPanel &operator=(PreviewPanel &&)      = delete;

    public slots:
        /**
         * @brief Previews the given file.
         * @param item The FileItem to preview.
         */
        void preview(const model::FileItem &item);

        /**
         * @brief Clears the preview panel.
         */
        void clear();

    protected:
        /** @brief Builds the vertical preview layout and child widgets. */
        void setupLayout(VLayout &layout) override;

        /** @brief Applies size constraints for the preview panel. */
        void setupSize()  override;

        /** @brief Applies object name and style-related flags. */
        void setupStyle() override;

    private:
        /** @brief Shows a text preview of the given file. */
        void showText(const QString &path);

        /** @brief Shows an image preview of the given file. */
        void showImage(const QString &path);

        /** @brief Shows file metadata. */
        void showMetadata(const model::FileItem &item);

        QPointer<QLabel>         m_nameLabel;
        QPointer<QLabel>         m_metaLabel;
        QPointer<QLabel>         m_imageLabel;
        QPointer<QTextEdit>      m_textEdit;
        QPointer<QStackedWidget> m_stack;
    };

} // namespace dentry::ui

/**
 * @file CentralWidget.cpp
 * @brief Implementation of CentralWidget.
 *
 * @author Hugo Fabresse
 */

#include "CentralWidget.h"

namespace dentry::ui {

    CentralWidget::CentralWidget(model::FileSystemModel *model, QWidget *parent)
        : QWidget(parent)
        , m_model(model) {
        build();
    }

    void CentralWidget::build() {
        m_layout   = new QHBoxLayout(this);
        m_splitter = new QSplitter(Qt::Horizontal, this);

        m_sidebar      = new Sidebar(m_splitter);
        m_fileListView = new FileListView(m_splitter);
        m_previewPanel = new PreviewPanel(m_splitter);

        m_fileListView->setModel(m_model);

        m_splitter->addWidget(m_sidebar);
        m_splitter->addWidget(m_fileListView);
        m_splitter->addWidget(m_previewPanel);
        m_splitter->setStretchFactor(0, 0);
        m_splitter->setStretchFactor(1, 1);
        m_splitter->setStretchFactor(2, 0);

        m_layout->addWidget(m_splitter);

        setupSize();
        setupStyle();
    }

    void CentralWidget::setSidebarShowHidden(bool show) {
        m_sidebar->setShowHidden(show);
    }

    void CentralWidget::clearPreview() {
        m_previewPanel->clear();
    }

    void CentralWidget::updatePreviewFromSelection(const QList<model::FileItem> &selected) {
        if (selected.count() == 1 && !selected.first().isDir) {
            m_previewPanel->preview(selected.first());
            return;
        }

        m_previewPanel->clear();
    }

    void CentralWidget::setupSize() {
        m_layout->setContentsMargins(6, 6, 6, 6);
        m_layout->setSpacing(6);
    }

    void CentralWidget::setupStyle() {
        setObjectName("centralWidget");
        m_splitter->setObjectName("mainSplitter");
    }

} // namespace dentry::ui

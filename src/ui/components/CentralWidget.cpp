/**
 * @file CentralWidget.cpp
 * @brief Implementation of CentralWidget.
 *
 * @author Hugo Fabresse
 */

#include "CentralWidget.h"

namespace Dentry::Ui {

    CentralWidget::CentralWidget(Model::FileSystemModel *model, QWidget *parent)
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

        m_previewPanel->hide();

        m_layout->addWidget(m_splitter);

        setupSize();
        setupStyle();
    }

    void CentralWidget::setupSize() {
        m_layout->setContentsMargins(6, 6, 6, 6);
        m_layout->setSpacing(6);
    }

    void CentralWidget::setupStyle() {
        setObjectName("centralWidget");
        m_splitter->setObjectName("mainSplitter");
    }

} // namespace Dentry::Ui

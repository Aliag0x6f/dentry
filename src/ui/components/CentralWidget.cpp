// src/ui/components/CentralWidget.cpp
#include "log/Logger.h"
#include "ui/components/CentralWidget.h"
#include "ui/components/Sidebar.h"

namespace dentry::ui {

    CentralWidget::CentralWidget(model::FileSystemModel *model, QWidget *parent)
        : UIComponent(parent)
        , m_model(model) {
        build();

        log::info("Ui") << "CentralWidget built";
    }

    void CentralWidget::setupLayout(HLayout &layout) {
        m_splitter = new QSplitter(Qt::Horizontal, this);
        m_splitter->setObjectName("mainSplitter");

        m_sidebar = new SideBar(m_splitter);
        m_filelistview = new FileListView(m_splitter);
        m_previewpanel = new PreviewPanel(m_splitter);

        m_splitter->addWidget(m_sidebar);
        m_splitter->addWidget(m_filelistview);
        m_splitter->addWidget(m_previewpanel);

        layout.addWidget(m_splitter);
    }

} // namespace dentry::ui

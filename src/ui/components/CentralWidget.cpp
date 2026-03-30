#include <QObject>
#include "log/Logger.h"
#include "ui/components/CentralWidget.h"
#include "ui/components/Sidebar.h"

namespace dentry::ui {

    CentralWidget::CentralWidget(QWidget *parent)
        : UIComponent(parent) {
        build();

        log::info("Ui") << "CentralWidget built";
    }

    void CentralWidget::setupLayout(HLayout &layout) {
        m_splitter = new QSplitter(Qt::Horizontal, this);
        m_splitter->setObjectName("mainSplitter");

        m_sidebar = new SideBar(m_splitter);
        m_fileListView = new FileListView(m_splitter);
        m_previewPanel = new PreviewPanel(m_splitter);

        m_splitter->addWidget(m_sidebar);
        m_splitter->addWidget(m_fileListView);
        m_splitter->addWidget(m_previewPanel);

        layout.addWidget(m_splitter);

        auto* fileModel = m_fileListView->model();
        if (auto* fsModel = qobject_cast<dentry::model::FileSystemModel*>(fileModel)) {
            fsModel->manageSelectionFocus(m_fileListView->selectionModel(), m_fileListView);
        }
        auto* placesModel = m_sidebar->model();
        if (placesModel) {
            placesModel->manageSelectionFocus(m_sidebar->listView()->selectionModel(), m_sidebar->listView());
        }
    }

} // namespace dentry::ui

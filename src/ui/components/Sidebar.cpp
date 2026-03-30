#include "ui/components/Sidebar.h"
#include "log/Logger.h"
#include "app/events/EventBus.h"
#include "app/input/InputMap.h"

#include <QLabel>
#include <QFrame>

namespace dentry::ui {

    SideBar::SideBar(QWidget *parent)
        : UIComponent(parent) {
        setObjectName("SideBar");
        m_model = new model::PlacesModel(this);
        build();

        log::info("Ui") << "SideBar built";
    }

    void SideBar::setupSize() {
        setMinimumWidth(220);
    }

    void SideBar::setupLayout(VLayout &layout) {
        layout.setContentsMargins(0, 0, 0, 0);
        layout.setSpacing(0);

        auto title = new QLabel("PLACES", this);
        title->setObjectName("SideBarTitle");
        layout.addWidget(title);

        auto separator = new QFrame(this);
        separator->setFrameShape(QFrame::HLine);
        separator->setObjectName("SideBarSeparator");
        layout.addWidget(separator);

        m_list = new NoNativeKeyListView(this);
        m_list->setModel(m_model);
        m_list->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_list->setMinimumWidth(0);
        m_list->setFocusPolicy(Qt::StrongFocus);
        m_list->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
        m_list->setCurrentIndex(model()->defaultIndex());
        layout.addWidget(m_list);
    }

    void SideBar::setupConnections() {
        connect(m_list, &QListView::activated, this, [this](const QModelIndex &index) {
            const QString path = index.data(Qt::UserRole).toString();
            if (!path.isEmpty())
                emit placeActivated(path);
        });

        connect(app::events::EventBus::instance(), &app::events::EventBus::focusWidget, this, [this](const QString& name) {
            if (name == objectName())
                focusList();
        });
    }

} // namespace dentry::ui

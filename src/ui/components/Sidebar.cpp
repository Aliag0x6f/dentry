// src/ui/components/Sidebar.cpp
#include "log/Logger.h"
#include "ui/components/Sidebar.h"

#include <QLabel>

namespace dentry::ui {

    SideBar::SideBar(QWidget *parent)
        : UIComponent(parent) {
        build();

        setObjectName("SideBar");
        setMinimumWidth(220);

        log::info("Ui") << "SideBar built";
    }

} // namespace dentry::ui

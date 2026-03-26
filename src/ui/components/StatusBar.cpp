// src/ui/components/StatusBar.cpp
#include "log/Logger.h"
#include "ui/components/StatusBar.h"

namespace dentry::ui {

    StatusBar::StatusBar(QWidget *parent)
        : UIComponent(parent) {
        build();

        setObjectName("StatusBar");

        log::info("Ui") << "StatusBar built";
    }

} // namespace dentry::ui

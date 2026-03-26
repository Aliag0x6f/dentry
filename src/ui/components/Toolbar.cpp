// src/ui/components/Toolbar.cpp
#include "log/Logger.h"
#include "ui/components/Toolbar.h"

namespace dentry::ui {

    ToolBar::ToolBar(QWidget *parent)
        : UIComponent(parent) {
        build();

        setObjectName("ToolBar");

        log::info("Ui") << "StatusBar built";
    }

} // namespace dentry::ui

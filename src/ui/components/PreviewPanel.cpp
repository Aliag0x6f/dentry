// src/ui/components/PreviewPanel.cpp
#include "log/Logger.h"
#include "ui/components/PreviewPanel.h"

namespace dentry::ui {

    PreviewPanel::PreviewPanel(QWidget *parent)
        : UIComponent(parent) {
        build();

        setObjectName("PreviewPanel");
        setMinimumWidth(220);

        log::info("Ui") << "PreviewPanel built";
    }

} // namespace dentry::ui

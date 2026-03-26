// src/ui/components/FileListView.cpp
#include "log/Logger.h"
#include "ui/components/FileListView.h"

#include <QLabel>

namespace dentry::ui {

    FileListView::FileListView(QWidget *parent)
        : UIComponent(parent) {
        build();

        setObjectName("FileListView");
        setMinimumWidth(220);

        log::info("Ui") << "FileListView built";
    }

} // namespace dentry::ui

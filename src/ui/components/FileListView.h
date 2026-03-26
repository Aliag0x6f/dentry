// src/ui/components/FileListView.h
#pragma once

#include "ui/DefaultLayout.h"
#include "ui/UIComponent.h"

#include <QTreeView>

namespace dentry::ui {

    class FileListView : public UIComponent<QTreeView, VLayout> {
        Q_OBJECT

    public:
        explicit FileListView(QWidget *parent = nullptr);
        ~FileListView() override = default;

        FileListView(const FileListView &)            = delete;
        FileListView &operator=(const FileListView &) = delete;
        FileListView(FileListView &&)                 = delete;
        FileListView &operator=(FileListView &&)      = delete;
    };

} // namespace dentry::ui

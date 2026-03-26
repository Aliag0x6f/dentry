// src/ui/components/FileListView.h
#pragma once

#include "model/FileSystemModel.h"
#include "ui/DefaultLayout.h"
#include "ui/UIComponent.h"

#include <QPointer>
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

    void setModel(model::FileSystemModel *model);

protected:
    void setupStyle() override;
    void setupConnections() override;

private:
    QPointer<model::FileSystemModel> m_model;
};

} // namespace dentry::ui

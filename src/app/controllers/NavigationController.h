// src/app/controller/NavigationController.h
#pragma once

#include "ui/components/FileListView.h"
#include "model/filesystem/FileSystemModel.h"

#include <QObject>
#include <QPointer>

namespace dentry::app {

class NavigationController : public QObject {
    Q_OBJECT

public:
    NavigationController(ui::FileListView *view,
                         model::FileSystemModel *model,
                         QObject *parent = nullptr);

private slots:
    void onActivated(const QModelIndex &index) const;
    void onDirectoryLoaded(const QString &path) const;

private:
    QPointer<ui::FileListView> m_view;
    QPointer<model::FileSystemModel> m_model;
};

} // namespace dentry::app

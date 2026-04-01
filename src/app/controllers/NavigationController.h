// src/app/controller/NavigationController.h
#pragma once

#include "ui/components/FileListView.h"
#include "model/filesystem/FileSystemModel.h"

#include <QObject>
#include <QPointer>
#include <QStack>

namespace dentry::app {

class NavigationController : public QObject {
    Q_OBJECT

public:
    NavigationController(ui::FileListView *view,
                         model::FileSystemModel *model,
                         QObject *parent = nullptr);

    [[nodiscard]] bool canGoBack() const;
    [[nodiscard]] QString currentPath() const;

public slots:
    void navigateBack();
    void navigateHome();
    void navigateTo(const QString &path);

signals:
    void pathChanged(const QString &path);
    void canGoBackChanged(bool canGoBack);

private slots:
    void onActivated(const QModelIndex &index);
    void onDirectoryLoaded(const QString &path);

private:
    QPointer<ui::FileListView> m_view;
    QPointer<model::FileSystemModel> m_model;
    QStack<QString> m_history;
};

} // namespace dentry::app

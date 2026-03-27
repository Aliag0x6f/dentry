// src/ui/components/CentralWidget.h
#pragma once

#include "model/FileSystemModel.h"
#include "ui/DefaultLayout.h"
#include "ui/UIComponent.h"
#include "ui/components/FileListView.h"
#include "ui/components/PreviewPanel.h"
#include "ui/components/Sidebar.h"

#include <QPointer>
#include <QSplitter>
#include <QWidget>

namespace dentry::ui {

class CentralWidget : public UIComponent<QWidget, HLayout> {
    Q_OBJECT

public:
    explicit CentralWidget(QWidget *parent = nullptr);
    ~CentralWidget() override = default;

    [[nodiscard]] FileListView *fileListView() const { return m_fileListView; }
    [[nodiscard]] SideBar      *sidebar()      const { return m_sidebar; }
    [[nodiscard]] PreviewPanel *previewPanel() const { return m_previewPanel; }

protected:
    void setupLayout(HLayout &layout) override;

private:
    QPointer<QSplitter>    m_splitter;
    QPointer<SideBar>      m_sidebar;
    QPointer<FileListView> m_fileListView;
    QPointer<PreviewPanel> m_previewPanel;
};

} // namespace dentry::ui

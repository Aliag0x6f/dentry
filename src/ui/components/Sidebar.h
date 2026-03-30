// src/ui/components/Sidebar.h
#pragma once

#include "model/places/PlacesModel.h"
#include "ui/DefaultLayout.h"
#include "ui/UIComponent.h"

#include <QFrame>
#include <QListView>
#include <QPointer>
#include <QKeyEvent>

namespace dentry::ui {

class NoNativeKeyListView : public QListView {
    Q_OBJECT
public:
    using QListView::QListView;
protected:
    void keyPressEvent(QKeyEvent *event) override {
        event->ignore();
    }
};

class SideBar : public UIComponent<QFrame, VLayout> {
    Q_OBJECT

public:
    explicit SideBar(QWidget *parent = nullptr);
    ~SideBar() override = default;

    SideBar(const SideBar &)            = delete;
    SideBar &operator=(const SideBar &) = delete;
    SideBar(SideBar &&)                 = delete;
    SideBar &operator=(SideBar &&)      = delete;

    [[nodiscard]] model::PlacesModel *model() const { return m_model; }

    [[nodiscard]] QPointer<QListView> listView() const { return m_list; }
    void focusList() const { if (m_list) m_list->setFocus(); }

signals:
    void placeActivated(const QString &path);

protected:
    void setupSize()        override;
    void setupLayout(VLayout &layout) override;
    void setupConnections() override;

private:
    QPointer<QListView>          m_list;
    QPointer<model::PlacesModel> m_model;
};

} // namespace dentry::ui

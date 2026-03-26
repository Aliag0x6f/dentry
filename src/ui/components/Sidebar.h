// src/ui/components/Sidebar.h
#pragma once

#include "ui/DefaultLayout.h"
#include "ui/UIComponent.h"

#include <QFrame>

namespace dentry::ui {

class SideBar : public UIComponent<QFrame, VLayout> {
    Q_OBJECT

public:
    explicit SideBar(QWidget *parent = nullptr);
    ~SideBar() override = default;

    SideBar(const SideBar &)            = delete;
    SideBar &operator=(const SideBar &) = delete;
    SideBar(SideBar &&)                 = delete;
    SideBar &operator=(SideBar &&)      = delete;
};

} // namespace dentry::ui

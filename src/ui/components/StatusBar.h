// src/ui/components/StatusBar.h
#pragma once

#include "ui/DefaultLayout.h"
#include "ui/UIComponent.h"

#include <QStatusBar>

namespace dentry::ui {

class StatusBar : public UIComponent<QStatusBar, HLayout> {
    Q_OBJECT

public:
    explicit StatusBar(QWidget *parent = nullptr);
    ~StatusBar() override = default;

    StatusBar(const StatusBar &)            = delete;
    StatusBar &operator=(const StatusBar &) = delete;
    StatusBar(StatusBar &&)                 = delete;
    StatusBar &operator=(StatusBar &&)      = delete;
};

} // namespace dentry::ui

// src/ui/components/Toolbar.h
#pragma once

#include "ui/DefaultLayout.h"
#include "ui/UIComponent.h"

#include <QToolBar>

namespace dentry::ui {

class ToolBar : public UIComponent<QToolBar, HLayout> {
    Q_OBJECT

public:
    explicit ToolBar(QWidget *parent = nullptr);
    ~ToolBar() override = default;

    ToolBar(const ToolBar &)            = delete;
    ToolBar &operator=(const ToolBar &) = delete;
    ToolBar(ToolBar &&)                 = delete;
    ToolBar &operator=(ToolBar &&)      = delete;
};

} // namespace dentry::ui

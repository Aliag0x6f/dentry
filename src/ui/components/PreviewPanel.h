// src/ui/components/PreviewPanel.h
#pragma once

#include "ui/DefaultLayout.h"
#include "ui/UIComponent.h"

#include <QFrame>

namespace dentry::ui {

class PreviewPanel : public UIComponent<QFrame, VLayout> {
    Q_OBJECT

public:
    explicit PreviewPanel(QWidget *parent = nullptr);
    ~PreviewPanel() override = default;

    PreviewPanel(const PreviewPanel &)            = delete;
    PreviewPanel &operator=(const PreviewPanel &) = delete;
    PreviewPanel(PreviewPanel &&)                 = delete;
    PreviewPanel &operator=(PreviewPanel &&)      = delete;
};

} // namespace dentry::ui

// src/app/bindings/SidebarBindings.h
#pragma once

#include "app/bindings/WidgetBindings.h"
#include "ui/components/Sidebar.h"
#include "ui/components/Toolbar.h"

namespace dentry::app::bindings {
    WidgetBindings sidebar(ui::SideBar* sidebar, ui::ToolBar *toolbar);
}

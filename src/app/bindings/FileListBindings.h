/**
* @file FileListBindings.h
 * @brief Keyboard bindings for FileListView.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "app/bindings/WidgetBindings.h"
#include "ui/components/FileListView.h"
#include "ui/components/Toolbar.h"

namespace dentry::app::bindings {
    WidgetBindings fileList(ui::FileListView* view, ui::ToolBar *toolbar);
}

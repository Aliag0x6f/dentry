/**
* @file FileListBindings.h
 * @brief Keyboard bindings for FileListView.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "app/input/WidgetBindings.h"
#include "ui/components/FileListView.h"

namespace dentry::app::bindings {
    WidgetBindings fileList(ui::FileListView* view);
}

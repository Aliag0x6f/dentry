/**
* @file FileListBindings.h
 * @brief Keyboard bindings for FileListView.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "app/input/WidgetBindings.h"

namespace dentry::ui { class FileListView; }

namespace dentry::app::bindings {

    /**
     * @brief Returns the WidgetBindings descriptor for a FileListView instance.
     *
     * Registers vim-style navigation:
     * - j / k         move selection down / up
     * - gg            jump to first entry
     * - G (Shift+g)   jump to last entry
     * - l / Enter     open/activate selected entry
     * - h / Backspace navigate back (emits activated with invalid index)
     */
    [[nodiscard]] WidgetBindings fileList(ui::FileListView *view);

} // namespace dentry::app::bindings

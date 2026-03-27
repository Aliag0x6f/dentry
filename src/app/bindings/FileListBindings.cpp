/**
 * @file FileListBindings.cpp
 * @brief Implementation of FileListView keyboard bindings.
 *
 * @author Hugo Fabresse
 */

#include "FileListBindings.h"
#include "app/input/InputMap.h"
#include "ui/components/FileListView.h"

#include <QItemSelectionModel>

namespace dentry::app::bindings {

WidgetBindings fileList(ui::FileListView *view) {
    return {
        view,
        [view](InputMap *map) {
            map->bind(Qt::Key_J, [view] {
                auto *sm   = view->selectionModel();
                auto  next = view->model()->index(sm->currentIndex().row() + 1, 0);
                if (next.isValid())
                    sm->setCurrentIndex(next, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            });

            map->bind(Qt::Key_K, [view] {
                auto *sm   = view->selectionModel();
                auto  prev = view->model()->index(sm->currentIndex().row() - 1, 0);
                if (prev.isValid())
                    sm->setCurrentIndex(prev, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            });

            map->bind({Qt::Key_G, Qt::Key_G}, [view] {
                auto first = view->model()->index(0, 0);
                if (first.isValid())
                    view->selectionModel()->setCurrentIndex(
                        first, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            });

            map->bind(Qt::Key_G, [view] {
                auto last = view->model()->index(view->model()->rowCount() - 1, 0);
                if (last.isValid())
                    view->selectionModel()->setCurrentIndex(
                        last, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            }, Qt::ShiftModifier);

            map->bind(Qt::Key_L, [view] {
                auto cur = view->currentIndex();
                if (cur.isValid()) emit view->activated(cur);
            });

            map->bind(Qt::Key_Return, [view] {
                auto cur = view->currentIndex();
                if (cur.isValid()) emit view->activated(cur);
            });

            map->bind(Qt::Key_H,         [view] { emit view->activated({}); });
            map->bind(Qt::Key_Backspace, [view] { emit view->activated({}); });
        }
    };
}

} // namespace dentry::app::bindings

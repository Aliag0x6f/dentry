#include "SidebarBindings.h"
#include "app/events/EventBus.h"
#include "app/input/InputMap.h"

namespace dentry::app::bindings {

    WidgetBindings sidebar(ui::SideBar* sidebar) {
        return WidgetBindings{
            sidebar,
            [sidebar](InputMap* map) {
                map->bind(Qt::Key_J, [sidebar] {
                    auto *sm = sidebar->listView()->selectionModel();
                    auto next = sidebar->listView()->model()->index(sm->currentIndex().row() + 1, 0);
                    if (next.isValid())
                        sm->setCurrentIndex(next, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
                });

                map->bind(Qt::Key_K, [sidebar] {
                    auto *sm = sidebar->listView()->selectionModel();
                    auto prev = sidebar->listView()->model()->index(sm->currentIndex().row() - 1, 0);
                    if (prev.isValid())
                        sm->setCurrentIndex(prev, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
                });

                map->bind({Qt::Key_G, Qt::Key_G}, [sidebar] {
                    auto first = sidebar->listView()->model()->index(0, 0);
                    if (first.isValid())
                        sidebar->listView()->selectionModel()->setCurrentIndex(first, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
                });

                map->bind(Qt::Key_G, [sidebar] {
                    auto last = sidebar->listView()->model()->index(sidebar->listView()->model()->rowCount() - 1, 0);
                    if (last.isValid())
                        sidebar->listView()->selectionModel()->setCurrentIndex(last, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
                }, Qt::ShiftModifier);

                map->bind(Qt::Key_L, [sidebar] {
                    auto cur = sidebar->listView()->currentIndex();
                    if (cur.isValid()) {
                        emit sidebar->listView()->activated(cur);
                        emit events::EventBus::instance()->focusWidget("FileListView");
                    }
                });
                map->bind(Qt::Key_Return, [sidebar] {
                    auto cur = sidebar->listView()->currentIndex();
                    if (cur.isValid()) {
                        emit sidebar->listView()->activated(cur);
                        emit events::EventBus::instance()->focusWidget("FileListView");
                    }
                });

                map->bind(Qt::Key_P, [] {
                    emit events::EventBus::instance()->focusWidget("FileListView");
                });
            }
        };
    }

} // namespace dentry::app::bindings

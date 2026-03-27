/**
* @file KeyboardController.cpp
 * @brief Implementation of KeyboardController.
 *
 * @author Hugo Fabresse
 */

#include "app/controllers/KeyboardController.h"
#include "log/Logger.h"

#include <QAbstractItemView>
#include <QEvent>
#include <QKeyEvent>
#include <QWidget>

namespace dentry::app {

    KeyboardController::KeyboardController(QWidget *target, InputMap *map,
                                           QObject *parent)
        : QObject(parent)
        , m_map(map)
        , m_target(target) {
        Q_ASSERT(target);
        Q_ASSERT(map);
        target->installEventFilter(this);

        if (auto *view = qobject_cast<QAbstractItemView *>(target)) {
            if (view->viewport())
                view->viewport()->installEventFilter(this);
        }

        log::debug("Kb") << "Installed on" << target->objectName();
    }

    bool KeyboardController::eventFilter(QObject *watched, QEvent *event) {
        if (watched != m_target)
            return false;

        if (event->type() != QEvent::KeyPress)
            return false;

        auto *ke = static_cast<QKeyEvent *>(event);
        log::debug("Kb") << "KeyboardController got key" << ke->key()
                         << "mods" << ke->modifiers()
                         << "from watched" << watched->objectName();

        if (!m_map)
            return false;
        return m_map->dispatch(static_cast<QKeyEvent *>(event));
    }

} // namespace dentry::app

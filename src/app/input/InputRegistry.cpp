/**
* @file InputRegistry.cpp
 * @brief Implementation of InputRegistry.
 *
 * @author Hugo Fabresse
 */

#include "InputMap.h"
#include "InputRegistry.h"
#include "app/controllers/KeyboardController.h"
#include "log/Logger.h"

namespace dentry::app {

    InputRegistry::InputRegistry(QObject *parent)
        : QObject(parent) {}

    void InputRegistry::installAll(const QList<WidgetBindings> &bindings) {
        for (const WidgetBindings &wb : bindings) {
            if (!wb.target || !wb.populate) {
                log::warn("Kb") << "Skipping invalid WidgetBindings entry";
                continue;
            }

            auto *map = new InputMap(this);
            wb.populate(map);
            new KeyboardController(wb.target, map, this);

            log::debug("Kb") << "Bindings installed for" << wb.target->objectName();
        }
    }

} // namespace dentry::app

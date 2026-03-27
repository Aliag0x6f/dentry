/**
* @file WidgetBindings.h
 * @brief Declarative binding descriptor for a single widget target.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QPointer>
#include <QWidget>

#include <functional>

namespace dentry::app {

    class InputMap;

    /**
     * @struct WidgetBindings
     * @brief Pairs a widget with a function that populates its InputMap.
     *
     * The populate function is responsible for calling InputMap::bind()
     * for every desired trigger. It receives a pre-constructed InputMap
     * and the target widget is captured by the caller via lambda.
     *
     * Example:
     * @code
     * WidgetBindings {
     *     view,
     *     [view](InputMap *map) {
     *         map->bind(Qt::Key_J, [view]{ ... });
     *     }
     * };
     * @endcode
     */
    struct WidgetBindings {
        QPointer<QWidget>               target;
        std::function<void(InputMap *)> populate;
    };

} // namespace dentry::app

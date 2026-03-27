/**
* @file InputRegistry.h
 * @brief Installs a set of WidgetBindings onto their respective targets.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "app/input/WidgetBindings.h"

#include <QList>
#include <QObject>

namespace dentry::app {

    /**
     * @class InputRegistry
     * @brief Owns and installs all InputMap + KeyboardController pairs.
     *
     * Receives a flat list of WidgetBindings, creates one InputMap and one
     * KeyboardController per entry, and owns them via Qt parent.
     *
     * Example:
     * @code
     * auto *reg = new InputRegistry(this);
     * reg->installAll({
     *     bindings::fileList(m_centralWidget->fileListView()),
     *     bindings::sidebar(m_centralWidget->sidebar()),
     * });
     * @endcode
     */
    class InputRegistry : public QObject {
        Q_OBJECT

    public:
        explicit InputRegistry(QObject *parent = nullptr);

        /**
         * @brief Creates and installs one InputMap + KeyboardController
         *        for each entry in @p bindings.
         */
        void installAll(const QList<WidgetBindings> &bindings);
    };

} // namespace dentry::app

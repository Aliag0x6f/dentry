/**
* @file AUIComponent.h
 * @brief Abstract base class for all UI components in Dentry.
 *
 * Provides a default implementation of build() as a template method
 * that sequentially calls setupSize(), setupStyle() and setupConnections().
 * Subclasses override only the steps they need.
 *
 * Inherit alongside the Qt widget base class:
 * @code
 * class StatusBar : public QStatusBar, public AUIComponent { ... };
 * @endcode
 *
 * Call build() as the last statement of the constructor:
 * @code
 * StatusBar::StatusBar(QWidget *parent)
 *     : QStatusBar(parent) {
 *     build();
 * }
 * @endcode
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "IUIComponent.h"

namespace dentry::ui {

    /**
     * @class AUIComponent
     * @brief Template method implementation of IUIComponent.
     *
     * Implements build() as an ordered sequence of setup steps.
     * Each step is a no-op by default — subclasses override only what they need.
     *
     * Override order: setupSize() → setupStyle() → setupConnections()
     */
    class AUIComponent : public IUIComponent {
    public:
        ~AUIComponent() override = default;

        /**
         * @brief Builds the component by running all setup steps in order.
         *
         * Calls setupSize(), setupStyle(), then setupConnections().
         * Subclasses should not override build() unless the order must change.
         */
        void build() override {
            setupSize();
            setupStyle();
            setupConnections();
        }

    protected:
        /** @brief Sets fixed/minimum/maximum dimensions. No-op by default. */
        void setupSize()        override {}

        /** @brief Applies object names, style hints, or QSS properties. No-op by default. */
        void setupStyle()       override {}

        /** @brief Connects internal signals and slots. No-op by default. */
        void setupConnections() override {}
    };

} // namespace dentry::ui

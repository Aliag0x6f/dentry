/**
 * @file IUIComponent.h
 * @brief Abstract base class for all UI components in Dentry.
 *
 * Enforces a common construction contract: every component must
 * implement build(), and may optionally override setupSize(),
 * setupStyle() and setupConnections().
 *
 * Convention: the constructor of each subclass calls build() as
 * its last statement, so the object is fully initialized on return.
 *
 * @author Hugo Fabresse
 */

#pragma once

namespace Dentry::Ui {

/**
 * @class IUIComponent
 * @brief Interface for buildable UI components.
 *
 * Inherit from this alongside the Qt widget base class:
 * @code
 * class StatusBar : public QStatusBar, public UIComponent { ... };
 * @endcode
 *
 * The constructor must call build():
 * @code
 * StatusBar::StatusBar(QWidget *parent)
 *     : QStatusBar(parent) {
 *     build();
 * }
 * @endcode
 */
class IUIComponent {
public:
    virtual ~IUIComponent() = default;

    /**
     * @brief Builds the component.
     *
     * Orchestrates setupSize(), setupStyle() and setupConnections().
     * Must be called once from the constructor.
     */
    virtual void build() = 0;

protected:
    /** @brief Sets fixed/minimum/maximum dimensions. */
    virtual void setupSize() {}

    /** @brief Applies object names, style hints, or QSS properties. */
    virtual void setupStyle() {}

    /** @brief Connects internal signals and slots. */
    virtual void setupConnections() {}
};

} // namespace Dentry::Ui

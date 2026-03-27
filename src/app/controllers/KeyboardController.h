/**
* @file KeyboardController.h
 * @brief Event filter that routes key presses through an InputMap.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "app/input/InputMap.h"

#include <QObject>
#include <QPointer>

class QWidget;

namespace dentry::app {

    /**
     * @class KeyboardController
     * @brief Installs as a QObject event filter and dispatches key presses
     *        through a configured InputMap.
     *
     * The target widget is left completely unmodified. Adding a new binding
     * requires only one call to InputMap::bind() — no changes to the widget class.
     *
     * Ownership:
     * - Does NOT own @p target or @p map.
     * - Both must outlive this controller, or share the same Qt parent.
     *
     * Example:
     * @code
     * auto *map = new InputMap(this);
     * map->bind(Qt::Key_J, [view]{ ... });
     * map->bind({Qt::Key_G, Qt::Key_G}, [view]{ ... });
     *
     * new KeyboardController(m_fileListView, map, this);
     * @endcode
     */
    class KeyboardController : public QObject {
        Q_OBJECT

    public:
        explicit KeyboardController(QWidget *target, InputMap *map,
                                    QObject *parent = nullptr);

        ~KeyboardController() override = default;

        KeyboardController(const KeyboardController &)            = delete;
        KeyboardController &operator=(const KeyboardController &) = delete;
        KeyboardController(KeyboardController &&)                 = delete;
        KeyboardController &operator=(KeyboardController &&)      = delete;

    protected:
        bool eventFilter(QObject *watched, QEvent *event) override;

    private:
        QPointer<InputMap> m_map;
        QPointer<QWidget>  m_target;
    };

} // namespace dentry::app

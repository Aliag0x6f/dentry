/**
 * @file InputMap.h
 * @brief Declarative registry of input trigger → action bindings.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QHash>
#include <QList>
#include <QObject>
#include <QPointer>
#include <Qt>

#include <functional>

class QKeyEvent;
class QTimer;

namespace dentry::app {

/**
 * @brief Describes a single key press (key + optional modifiers).
 */
struct KeyTrigger {
    Qt::Key               key;
    Qt::KeyboardModifiers modifiers = Qt::NoModifier;

    bool operator==(const KeyTrigger &o) const noexcept {
        return key == o.key && modifiers == o.modifiers;
    }
};

inline size_t qHash(const KeyTrigger &t, size_t seed = 0) noexcept {
    return qHashMulti(seed, static_cast<int>(t.key), static_cast<int>(t.modifiers));
}

/**
 * @class InputMap
 * @brief Maps key triggers (single keys or sequences) to callable actions.
 *
 * Supports:
 *   - Single key:   bind(Qt::Key_J,  action)
 *   - With mods:    bind(Qt::Key_G,  action, Qt::ShiftModifier)   // "G"
 *   - Sequence:     bind({Qt::Key_G, Qt::Key_G}, action)          // "gg"
 *
 * Sequence state resets after a configurable timeout (default 500 ms).
 * Pending keys that do not form any registered sequence fall back to
 * single-key lookup.
 *
 * Example:
 * @code
 * auto *map = new InputMap(this);
 * map->bind(Qt::Key_J, [view]{ ... });
 * map->bind({Qt::Key_G, Qt::Key_G}, [view]{ ... });
 * @endcode
 */
class InputMap : public QObject {
    Q_OBJECT

public:
    using Action = std::function<void()>;

    explicit InputMap(QObject *parent = nullptr);

    /** @brief Binds a single key (+ optional modifiers) to an action. */
    void bind(Qt::Key key, Action action,
              Qt::KeyboardModifiers modifiers = Qt::NoModifier);

    /**
     * @brief Binds a key sequence (no modifiers per key) to an action.
     *
     * Each element of @p sequence is treated with Qt::NoModifier.
     * Use this for vim-style combos such as "gg".
     */
    void bind(QList<Qt::Key> sequence, Action action);

    /**
     * @brief Processes a QKeyEvent.
     * @return true if the event was consumed and should not propagate further.
     */
    bool dispatch(QKeyEvent *event);

    /** @brief Sets the sequence reset timeout (default: 500 ms). */
    void setSequenceTimeout(int ms);

private:
    void clearPending();

    // ── Single key bindings ───────────────────────────────────────────
    QHash<KeyTrigger, Action> m_single;

    // ── Sequence bindings ─────────────────────────────────────────────
    struct SeqBinding {
        QList<Qt::Key> keys;
        Action         action;
    };
    QList<SeqBinding> m_sequences;
    QList<Qt::Key>    m_pending;   ///< Keys accumulated toward a sequence.
    QPointer<QTimer>  m_timer;     ///< Resets pending on inactivity.
};

} // namespace dentry::app

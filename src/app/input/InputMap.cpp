/**
 * @file InputMap.cpp
 * @brief Implementation of InputMap.
 *
 * @author Hugo Fabresse
 */

#include "app/input/InputMap.h"
#include "log/Logger.h"

#include <QKeyEvent>
#include <QTimer>

namespace dentry::app {

InputMap::InputMap(QObject *parent)
    : QObject(parent)
    , m_timer(new QTimer(this)) {
    m_timer->setSingleShot(true);
    m_timer->setInterval(500);
    connect(m_timer, &QTimer::timeout, this, &InputMap::clearPending);
}

void InputMap::bind(Qt::Key key, Action action,
                    Qt::KeyboardModifiers modifiers) {
    m_single.insert({key, modifiers}, std::move(action));
}

void InputMap::bind(QList<Qt::Key> sequence, Action action) {
    Q_ASSERT(!sequence.isEmpty());
    m_sequences.append({std::move(sequence), std::move(action)});
}

void InputMap::setSequenceTimeout(int ms) {
    m_timer->setInterval(ms);
}

bool InputMap::dispatch(QKeyEvent *event) {
    const auto key  = static_cast<Qt::Key>(event->key());
    const auto mods = event->modifiers();

    if (!m_sequences.isEmpty() && mods == Qt::NoModifier) {
        m_pending.append(key);
        m_timer->start();

        for (const SeqBinding &b : m_sequences) {
            if (b.keys == m_pending) {
                log::debug("Kb") << "Sequence" << &b << "matched, executing action";
                clearPending();
                b.action();
                return true;
            }
        }

        for (const SeqBinding &b : m_sequences) {
            if (b.keys.size() > m_pending.size()) {
                bool prefix = true;
                for (int i = 0; i < m_pending.size(); ++i) {
                    if (b.keys[i] != m_pending[i]) { prefix = false; break; }
                }
                if (prefix)
                    return true;
            }
        }

        clearPending();
    }

    auto it = m_single.find({key, mods});
    if (it == m_single.end())
        return false;

    log::debug("Kb") << "Single key" << key << "matched, executing action";
    it.value()();
    return true;
}

void InputMap::clearPending() {
    m_pending.clear();
    m_timer->stop();
}

} // namespace dentry::app

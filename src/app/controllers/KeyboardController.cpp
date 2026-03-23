/**
 * @file KeyboardController.cpp
 * @brief Implementation of KeyboardController.
 *
 * @author Hugo Fabresse
 */

#include "KeyboardController.h"

#include <QKeyEvent>

namespace dentry::app {

    KeyboardController::KeyboardController() {
        installDefaultBindings();
    }

    bool KeyboardController::handleKeyPress(const QKeyEvent &event, FileListCommand &command) {
        const Qt::KeyboardModifiers blockedModifiers = Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier;
        if ((event.modifiers() & blockedModifiers) != Qt::NoModifier) {
            resetSequence();
            return false;
        }

        const KeyStroke stroke = normalize(event);
        if (stroke.key == 0) {
            resetSequence();
            return false;
        }

        m_pending.append(stroke);

        MatchState state = match(m_pending, command);
        if (state == MatchState::Exact) {
            m_pending.clear();
            return true;
        }

        if (state == MatchState::Prefix)
            return false;

        if (m_pending.size() > 1) {
            const KeyStroke lastStroke = m_pending.back();
            m_pending = { lastStroke };

            state = match(m_pending, command);
            if (state == MatchState::Exact) {
                m_pending.clear();
                return true;
            }

            if (state == MatchState::Prefix)
                return false;
        }

        m_pending.clear();
        return false;
    }

    void KeyboardController::resetSequence() {
        m_pending.clear();
    }

    void KeyboardController::setLeaderKey(Qt::Key key) {
        m_leaderKey = key;
    }

    Qt::Key KeyboardController::leaderKey() const {
        return m_leaderKey;
    }

    void KeyboardController::setBinding(const QList<QPair<int, Qt::KeyboardModifiers>> &sequence,
                                        FileListCommand command) {
        if (sequence.isEmpty())
            return;

        Binding binding;
        binding.command = command;

        for (const auto &step : sequence) {
            const int key = step.first;
            const Qt::KeyboardModifiers modifiers = step.second & Qt::ShiftModifier;
            if (key == 0)
                return;

            binding.sequence.append({ key, modifiers });
        }

        for (Binding &existing : m_bindings) {
            if (existing.sequence.size() != binding.sequence.size())
                continue;

            bool same = true;
            for (int i = 0; i < existing.sequence.size(); ++i) {
                if (!equals(existing.sequence.at(i), binding.sequence.at(i))) {
                    same = false;
                    break;
                }
            }

            if (same) {
                existing.command = command;
                return;
            }
        }

        m_bindings.append(binding);
    }

    void KeyboardController::setLeaderBinding(const QList<int> &sequence, FileListCommand command) {
        if (m_leaderKey == Qt::Key_unknown || sequence.isEmpty())
            return;

        QList<QPair<int, Qt::KeyboardModifiers>> fullSequence;
        fullSequence.append({ m_leaderKey, Qt::NoModifier });

        for (const int key : sequence) {
            if (key == 0)
                return;
            fullSequence.append({ key, Qt::NoModifier });
        }

        setBinding(fullSequence, command);
    }

    KeyboardController::KeyStroke KeyboardController::normalize(const QKeyEvent &event) {
        return { event.key(), event.modifiers() & Qt::ShiftModifier };
    }

    bool KeyboardController::equals(const KeyStroke &left, const KeyStroke &right) {
        return left.key == right.key && left.modifiers == right.modifiers;
    }

    bool KeyboardController::startsWith(const QList<KeyStroke> &sequence, const QList<KeyStroke> &prefix) {
        if (prefix.size() > sequence.size())
            return false;

        for (int i = 0; i < prefix.size(); ++i) {
            if (!equals(sequence.at(i), prefix.at(i)))
                return false;
        }

        return true;
    }

    KeyboardController::MatchState
    KeyboardController::match(const QList<KeyStroke> &candidate, FileListCommand &command) const {
        bool hasPrefix = false;

        for (const Binding &binding : m_bindings) {
            if (!startsWith(binding.sequence, candidate))
                continue;

            if (candidate.size() == binding.sequence.size()) {
                command = binding.command;
                return MatchState::Exact;
            }

            hasPrefix = true;
        }

        return hasPrefix ? MatchState::Prefix : MatchState::NoMatch;
    }

    void KeyboardController::installDefaultBindings() {
        setBinding({ { Qt::Key_J, Qt::NoModifier } }, FileListCommand::MoveDown);
        setBinding({ { Qt::Key_K, Qt::NoModifier } }, FileListCommand::MoveUp);
        setBinding({ { Qt::Key_G, Qt::NoModifier }, { Qt::Key_G, Qt::NoModifier } }, FileListCommand::FirstEntry);
        setBinding({ { Qt::Key_G, Qt::ShiftModifier } }, FileListCommand::LastEntry);
        setBinding({ { Qt::Key_L, Qt::NoModifier } }, FileListCommand::Activate);
        setBinding({ { Qt::Key_Return, Qt::NoModifier } }, FileListCommand::Activate);
        setBinding({ { Qt::Key_Enter, Qt::NoModifier } }, FileListCommand::Activate);
        setBinding({ { Qt::Key_H, Qt::NoModifier } }, FileListCommand::NavigateBack);
        setBinding({ { Qt::Key_Backspace, Qt::NoModifier } }, FileListCommand::NavigateBack);
    }

} // namespace dentry::app

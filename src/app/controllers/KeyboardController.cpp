/**
 * @file KeyboardController.cpp
 * @brief Implementation of KeyboardController.
 *
 * @author Hugo Fabresse
 */

#include "KeyboardController.h"
#include "../../log/Logger.h"

#include <QKeyEvent>
#include <QKeySequence>

namespace dentry::app {

    const char *fileListCommandToString(const FileListCommand command) {
        switch (command) {
            case FileListCommand::MoveDown:
                return "MoveDown";
            case FileListCommand::MoveUp:
                return "MoveUp";
            case FileListCommand::FirstEntry:
                return "FirstEntry";
            case FileListCommand::LastEntry:
                return "LastEntry";
            case FileListCommand::Activate:
                return "Activate";
            case FileListCommand::NavigateBack:
                return "NavigateBack";
            case FileListCommand::FocusSidebar:
                return "FocusSidebar";
        }

        return "Unknown";
    }

    KeyboardController::KeyboardController() {
        installDefaultBindings();
    }

    bool KeyboardController::handleKeyPress(const QKeyEvent &event, FileListCommand &command) {
        log::debug("Keyboard") << "Key press received"
                               << "key=" << event.key()
                               << "keyText=" << QKeySequence(event.key()).toString()
                               << "text=" << event.text()
                               << "modifiers=" << static_cast<int>(event.modifiers());

        const Qt::KeyboardModifiers blockedModifiers = Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier;
        if ((event.modifiers() & blockedModifiers) != Qt::NoModifier) {
            log::debug("Keyboard") << "Ignoring key press: unsupported modifiers";
            resetSequence();
            return false;
        }

        const KeyStroke stroke = normalize(event);
        if (stroke.key == 0) {
            log::debug("Keyboard") << "Ignoring key press: normalized key is 0";
            resetSequence();
            return false;
        }

        m_pending.append(stroke);

        MatchState state = match(m_pending, command);
        if (state == MatchState::Exact) {
            log::info("Keyboard") << "Keyboard action matched:" << fileListCommandToString(command);
            m_pending.clear();
            return true;
        }

        if (state == MatchState::Prefix) {
            log::debug("Keyboard") << "Partial key sequence matched; waiting for next key";
            return false;
        }

        if (m_pending.size() > 1) {
            const KeyStroke lastStroke = m_pending.back();
            m_pending = { lastStroke };

            state = match(m_pending, command);
            if (state == MatchState::Exact) {
                log::info("Keyboard") << "Keyboard action matched:" << fileListCommandToString(command);
                m_pending.clear();
                return true;
            }

            if (state == MatchState::Prefix) {
                log::debug("Keyboard") << "Partial key sequence matched after reset; waiting for next key";
                return false;
            }
        }

        log::debug("Keyboard") << "No keyboard binding matched";
        m_pending.clear();
        return false;
    }

    void KeyboardController::resetSequence() {
        m_pending.clear();
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
        for (const auto &binding : defaultBindings()) {
            setBinding(binding.sequence, binding.command);
        }
    }

    const QList<KeyBinding> &KeyboardController::defaultBindings() {
        return DEFAULT_BINDINGS;
    }

} // namespace dentry::app

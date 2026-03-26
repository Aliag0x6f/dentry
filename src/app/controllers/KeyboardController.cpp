/**
 * @file KeyboardController.cpp
 * @brief Implementation of KeyboardController.
 *
 * @author Hugo Fabresse
 */

#include "KeyboardController.h"
#include "../../log/Logger.h"

#include <QAbstractItemView>
#include <QKeyEvent>
#include <QKeySequence>
#include <QModelIndex>
#include <QAbstractItemModel>

#include <QtGlobal>

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
            case FileListCommand::ToggleHidden:
                return "ToggleHidden";
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

    bool KeyboardController::handleKeyPress(const QKeyEvent &event, const CommandContext &context) {
        FileListCommand command;
        if (!handleKeyPress(event, command))
            return false;

        executeCommand(command, context);
        return true;
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

    void KeyboardController::executeCommand(const FileListCommand command, const CommandContext &context) {
        switch (command) {
            case FileListCommand::MoveDown:
                selectRelativeRow(context.view, +1);
                break;
            case FileListCommand::MoveUp:
                selectRelativeRow(context.view, -1);
                break;
            case FileListCommand::FirstEntry:
                selectRow(context.view, 0);
                break;
            case FileListCommand::LastEntry:
                if (context.view && context.view->model()) {
                    const int rowCount = context.view->model()->rowCount(context.view->rootIndex());
                    if (rowCount > 0)
                        selectRow(context.view, rowCount - 1);
                }
                break;
            case FileListCommand::Activate:
                if (context.onActivate)
                    context.onActivate();
                break;
            case FileListCommand::NavigateBack:
                if (context.onNavigateBack)
                    context.onNavigateBack();
                break;
            case FileListCommand::FocusSidebar:
                if (context.onFocusSidebar)
                    context.onFocusSidebar();
                else if (context.onFocusFileListView)
                    context.onFocusFileListView();
                break;
            case FileListCommand::ToggleHidden:
                if (context.onToggleHidden)
                    context.onToggleHidden();
                break;
        }
    }

    void KeyboardController::selectRow(QAbstractItemView *view, const int row) {
        if (!view || !view->model())
            return;

        const int rowCount = view->model()->rowCount(view->rootIndex());
        if (row < 0 || row >= rowCount)
            return;

        const QModelIndex index = view->model()->index(row, 0, view->rootIndex());
        if (!index.isValid())
            return;

        view->setCurrentIndex(index);
        view->scrollTo(index, QAbstractItemView::PositionAtCenter);
    }

    void KeyboardController::selectRelativeRow(QAbstractItemView *view, const int delta) {
        if (!view || !view->model())
            return;

        const int rowCount = view->model()->rowCount(view->rootIndex());
        if (rowCount <= 0)
            return;

        const QModelIndex current = view->currentIndex();
        const int currentRow = current.isValid() ? current.row() : (delta > 0 ? -1 : rowCount);
        const int targetRow = qBound(0, currentRow + delta, rowCount - 1);
        selectRow(view, targetRow);
    }

} // namespace dentry::app

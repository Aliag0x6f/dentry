/**
 * @file KeyboardController.h
 * @brief Keybinding controller for file list keyboard interactions.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QList>
#include <QPair>
#include <Qt>

class QKeyEvent;

namespace dentry::app {

/**
 * @brief Semantic commands produced from keybindings in the file list.
 */
enum class FileListCommand {
    MoveDown,
    MoveUp,
    FirstEntry,
    LastEntry,
    Activate,
    NavigateBack
};

/**
 * @brief Resolves key presses into semantic file list commands.
 *
 * Keeps key-sequence parsing state outside widgets so bindings can be
 * customized in one place (including a leader-based style in future).
 */
class KeyboardController {
public:
    KeyboardController();

    /**
     * @brief Parses a key event and outputs a command when a binding matches.
     * @param event Input key event.
     * @param command Output command if the method returns true.
     * @return True when a binding produced a command.
     */
    bool handleKeyPress(const QKeyEvent &event, FileListCommand &command);

    /** @brief Clears any pending partially typed sequence. */
    void resetSequence();

    /** @brief Sets a leader key used for custom multi-key mappings. */
    void setLeaderKey(Qt::Key key);

    /** @brief Returns the currently configured leader key, or Key_unknown if disabled. */
    [[nodiscard]] Qt::Key leaderKey() const;

    /**
     * @brief Registers or replaces a raw key sequence binding.
     *
     * Each pair stores (Qt::Key, modifiers). Only Shift is considered as a
     * binding modifier. Ctrl/Alt/Meta are reserved and ignored.
     */
    void setBinding(const QList<QPair<int, Qt::KeyboardModifiers>> &sequence, FileListCommand command);

    /**
     * @brief Registers a leader-based binding sequence.
     *
     * Example with leader set to Space: sequence {Qt::Key_H} maps to "Space h".
     */
    void setLeaderBinding(const QList<int> &sequence, FileListCommand command);

private:
    struct KeyStroke {
        int key = 0;
        Qt::KeyboardModifiers modifiers = Qt::NoModifier;
    };

    struct Binding {
        QList<KeyStroke> sequence;
        FileListCommand command = FileListCommand::MoveDown;
    };

    enum class MatchState {
        NoMatch,
        Prefix,
        Exact
    };

    static KeyStroke normalize(const QKeyEvent &event);
    static bool equals(const KeyStroke &left, const KeyStroke &right);
    static bool startsWith(const QList<KeyStroke> &sequence, const QList<KeyStroke> &prefix);

    MatchState match(const QList<KeyStroke> &candidate, FileListCommand &command) const;

    void installDefaultBindings();

    QList<Binding>   m_bindings;
    QList<KeyStroke> m_pending;
    Qt::Key          m_leaderKey = Qt::Key_unknown;
};

} // namespace dentry::app

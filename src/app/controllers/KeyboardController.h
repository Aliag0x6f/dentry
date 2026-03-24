/**
 * @file KeyboardController.h
 * @brief Keybinding controller for file list keyboard interactions.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <qevent.h>
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
    NavigateBack,
    FocusSidebar
};

/** @brief Returns a stable string name for a FileListCommand. */
[[nodiscard]] const char *fileListCommandToString(FileListCommand command);

/**
 * @brief Keybinding definition: associates a key sequence with a command and description.
 */
struct KeyBinding {
    QList<QPair<int, Qt::KeyboardModifiers>> sequence;
    FileListCommand command;
    const char *description;
};

/**
 * @brief Default keybindings: vim-inspired navigation.
 *
 * Easy to modify here - just change the list and recompile.
 * The list follows this pattern:
 * - Movement: j/k for down/up
 * - Jump: gg/G for first/last
 * - Activate: l/Return/Enter to open
 * - Back: h/Backspace to go back
 */
inline static const QList<KeyBinding> DEFAULT_BINDINGS = {
    // Movement
    { { { Qt::Key_Down, Qt::NoModifier } }, FileListCommand::MoveDown, "Down: Move down" },
    { { { Qt::Key_Up, Qt::NoModifier } }, FileListCommand::MoveUp, "Up: Move up" },
    { { { Qt::Key_J, Qt::NoModifier } }, FileListCommand::MoveDown, "j: Move down" },
    { { { Qt::Key_K, Qt::NoModifier } }, FileListCommand::MoveUp, "k: Move up" },

    // Jump
    { { { Qt::Key_G, Qt::NoModifier }, { Qt::Key_G, Qt::NoModifier } }, FileListCommand::FirstEntry, "gg: Jump to first" },
    { { { Qt::Key_G, Qt::ShiftModifier } }, FileListCommand::LastEntry, "G: Jump to last" },

    // Activation
    { { { Qt::Key_Right, Qt::NoModifier } }, FileListCommand::Activate, "Right: Open/Activate" },
    { { { Qt::Key_L, Qt::NoModifier } }, FileListCommand::Activate, "l: Open/Activate" },
    { { { Qt::Key_Return, Qt::NoModifier } }, FileListCommand::Activate, "Return: Open/Activate" },
    { { { Qt::Key_Enter, Qt::NoModifier } }, FileListCommand::Activate, "Enter: Open/Activate" },

    // Navigation back
    { { { Qt::Key_Left, Qt::NoModifier } }, FileListCommand::NavigateBack, "Left: Back" },
    { { { Qt::Key_H, Qt::NoModifier } }, FileListCommand::NavigateBack, "h: Back" },
    { { { Qt::Key_Backspace, Qt::NoModifier } }, FileListCommand::NavigateBack, "Backspace: Back" },

    // Focus sidebar
    { { { Qt::Key_P, Qt::NoModifier } }, FileListCommand::FocusSidebar, "p: Go to Places" },
};

/**
 * @brief Resolves key presses into semantic file list commands.
 *
 * Keeps key-sequence parsing state outside widgets so bindings can be
 * customized in one place.
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


    /**
     * @brief Registers or replaces a raw key sequence binding.
     *
     * Each pair stores (Qt::Key, modifiers). Only Shift is considered as a
     * binding modifier. Ctrl/Alt/Meta are reserved and ignored.
     */
    void setBinding(const QList<QPair<int, Qt::KeyboardModifiers>> &sequence, FileListCommand command);


    /**
     * @brief Returns the list of default keybindings.
     *
     * Useful for documentation, remapping, or config file generation.
     */
    [[nodiscard]] static const QList<KeyBinding> &defaultBindings();

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
};

} // namespace dentry::app

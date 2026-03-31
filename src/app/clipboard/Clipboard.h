/**
* @file Clipboard.h
 * @brief Internal clipboard for file operations.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QObject>
#include <QStringList>

namespace dentry::app {

    /**
     * @class Clipboard
     * @brief Stores the state of the current file operation clipboard.
     *
     * Tracks whether the stored paths were copied or cut, and emits
     * stateChanged() on every mutation so observers (e.g. ContextMenuModel)
     * can react without polling.
     */
    class Clipboard : public QObject {
        Q_OBJECT

    public:
        explicit Clipboard(QObject *parent = nullptr);

        ~Clipboard() override = default;

        Clipboard(const Clipboard &)            = delete;
        Clipboard &operator=(const Clipboard &) = delete;
        Clipboard(Clipboard &&)                 = delete;
        Clipboard &operator=(Clipboard &&)      = delete;

        /** @brief Stores paths for a copy operation and emits stateChanged(). */
        void copy(const QStringList &paths);

        /** @brief Stores paths for a cut (move) operation and emits stateChanged(). */
        void cut(const QStringList &paths);

        /** @brief Clears all stored paths and emits stateChanged(). */
        void clear();

        [[nodiscard]] const QStringList &paths()   const;
        [[nodiscard]] bool               isCut()   const;
        [[nodiscard]] bool               isEmpty() const;

        signals:
            /** @brief Emitted after any state mutation (copy, cut, or clear). */
            void stateChanged();

    private:
        QStringList m_paths;
        bool        m_isCut = false;
    };

} // namespace dentry::app

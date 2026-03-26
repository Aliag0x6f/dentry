/**
 * @file SidebarEvents.h
 * @brief Event state and keyboard orchestration for Sidebar interactions.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "../controllers/KeyboardController.h"

#include <QObject>
#include <QPointer>
#include <functional>

class QEvent;
class QKeyEvent;
class QListWidget;

namespace dentry::app::events {

struct SidebarEventCallbacks {
    std::function<void(FileListCommand)> onCommand;
};

/**
 * @brief Event filter that centralizes sidebar keyboard handling and logging.
 */
class SidebarEvents : public QObject {
public:
    explicit SidebarEvents(QObject *parent = nullptr);

    /** @brief Sets callbacks used to dispatch semantic UI actions. */
    void setCallbacks(SidebarEventCallbacks callbacks);

    /** @brief Installs this filter on the given sidebar list widget. */
    void installOn(QListWidget *list);

    /** @brief Logs the command that has just been executed by the sidebar. */
    void onCommandExecuted(FileListCommand command) const;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    bool handleKeyPress(QKeyEvent *event);

    QPointer<QListWidget> m_list;
    SidebarEventCallbacks m_callbacks;
    KeyboardController    m_keyboardController;
};

} // namespace dentry::app::events


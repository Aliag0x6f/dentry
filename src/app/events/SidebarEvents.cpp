/**
 * @file SidebarEvents.cpp
 * @brief Implementation of SidebarEvents.
 *
 * @author Hugo Fabresse
 */

#include "SidebarEvents.h"
#include "../../log/Logger.h"

#include <QEvent>
#include <QKeyEvent>
#include <QListWidget>

#include <utility>

namespace dentry::app::events {

SidebarEvents::SidebarEvents(QObject *parent)
    : QObject(parent) {
}

void SidebarEvents::setCallbacks(SidebarEventCallbacks callbacks) {
    m_callbacks = std::move(callbacks);
}

void SidebarEvents::installOn(QListWidget *list) {
    if (m_list)
        m_list->removeEventFilter(this);

    m_list = list;
    if (m_list)
        m_list->installEventFilter(this);
}

void SidebarEvents::onCommandExecuted(const FileListCommand command) const {
    log::info("Keyboard") << "Sidebar executed keyboard action:" << fileListCommandToString(command);
}

bool SidebarEvents::eventFilter(QObject *watched, QEvent *event) {
    if (watched != m_list || !event)
        return QObject::eventFilter(watched, event);

    if (event->type() != QEvent::KeyPress)
        return QObject::eventFilter(watched, event);

    return handleKeyPress(static_cast<QKeyEvent *>(event));
}

bool SidebarEvents::handleKeyPress(QKeyEvent *event) {
    if (!event)
        return false;

    FileListCommand command;
    if (!m_keyboardController.handleKeyPress(*event, command)) {
        // Block QListWidget default keyboard search (first-letter jump).
        event->accept();
        return true;
    }

    if (m_callbacks.onCommand)
        m_callbacks.onCommand(command);

    onCommandExecuted(command);
    event->accept();
    return true;
}

} // namespace dentry::app::events


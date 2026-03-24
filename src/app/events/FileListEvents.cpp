/**
 * @file FileListEvents.cpp
 * @brief Implementation of FileListEvents.
 *
 * @author Hugo Fabresse
 */

#include "FileListEvents.h"
#include "../../log/Logger.h"
#include "../../model/AFileSystemModel.h"

#include <QContextMenuEvent>
#include <QEvent>
#include <QKeyEvent>
#include <QKeySequence>
#include <QMenu>
#include <QMouseEvent>
#include <QTreeView>

#include <utility>

namespace dentry::app::events {

FileListEvents::FileListEvents(QObject *parent)
	: QObject(parent) {
}

void FileListEvents::setCallbacks(FileListEventCallbacks callbacks) {
	m_callbacks = std::move(callbacks);
}

void FileListEvents::installOn(QTreeView *view) {
	if (m_view)
		m_view->removeEventFilter(this);

	m_view = view;
	if (m_view)
		m_view->installEventFilter(this);
}

void FileListEvents::onCommandExecuted(const FileListCommand command) const {
	log::info("Keyboard") << "FileListView executed keyboard action:" << fileListCommandToString(command);
}

void FileListEvents::onSelectionChanged(const QList<model::FileItem> &selected) {
	const char *source = selectionInputSourceToString(m_lastSelectionInputSource);
	if (selected.isEmpty()) {
		log::info("Selection") << "Selection cleared" << "source=" << source;
	} else {
		const model::FileItem &first = selected.first();
		log::info("Selection") << "Selection changed"
							   << "source=" << source
							   << "count=" << selected.size()
							   << "first=" << first.name
							   << "path=" << first.absolutePath;
	}

	m_lastSelectionInputSource = SelectionInputSource::Unknown;
}

bool FileListEvents::eventFilter(QObject *watched, QEvent *event) {
	if (!hasEventView() || watched != m_view)
		return QObject::eventFilter(watched, event);

	switch (event->type()) {
		case QEvent::MouseButtonPress:
			m_lastSelectionInputSource = SelectionInputSource::Mouse;
			return false;

		case QEvent::MouseButtonDblClick:
			return handleMouseDoubleClick(static_cast<QMouseEvent *>(event));

		case QEvent::ContextMenu:
			return handleContextMenu(static_cast<QContextMenuEvent *>(event));

		case QEvent::KeyPress:
			return handleKeyPress(static_cast<QKeyEvent *>(event));

		default:
			return QObject::eventFilter(watched, event);
	}
}

bool FileListEvents::handleKeyPress(QKeyEvent *event) {
	m_lastSelectionInputSource = SelectionInputSource::Keyboard;

	log::debug("Keyboard") << "FileListView key press"
						   << "key=" << event->key()
						   << "keyText=" << QKeySequence(event->key()).toString()
						   << "text=" << event->text()
						   << "modifiers=" << static_cast<int>(event->modifiers());

	FileListCommand command;
	if (!m_keyboardController.handleKeyPress(*event, command))
		return false;

	if (m_callbacks.onCommand)
		m_callbacks.onCommand(command);

	onCommandExecuted(command);
	event->accept();
	return true;
}

bool FileListEvents::handleMouseDoubleClick(QMouseEvent *event) const {
	if (!hasEventView() || !m_callbacks.onDirectoryRequested || !m_callbacks.onFileActivated)
		return false;

	const QModelIndex index = m_view->indexAt(event->pos());
	if (!index.isValid())
		return false;

	const auto *fsModel = qobject_cast<const model::AFileSystemModel *>(m_view->model());
	if (!fsModel)
		return false;

	const model::FileItem item = fsModel->entries().at(index.row());
	if (item.isDir)
		m_callbacks.onDirectoryRequested(item.absolutePath);
	else
		m_callbacks.onFileActivated(item);

	return false;
}

bool FileListEvents::handleContextMenu(QContextMenuEvent *event) const {
	if (!hasEventView())
		return false;

	const auto *fsModel = qobject_cast<const model::AFileSystemModel *>(m_view->model());
	if (!fsModel || !m_view->selectionModel())
		return false;

	QStringList selectedPaths;
	const QModelIndexList indexes = m_view->selectionModel()->selectedRows();
	for (const QModelIndex &i : indexes)
		selectedPaths << fsModel->entries().at(i.row()).absolutePath;

	const QString currentDir = fsModel->currentPath();

	QMenu menu(m_view);

	if (!selectedPaths.isEmpty()) {
		if (m_callbacks.onCopyRequested)
			menu.addAction("Copy", m_view, [this, selectedPaths] { m_callbacks.onCopyRequested(selectedPaths); });
		if (m_callbacks.onCutRequested)
			menu.addAction("Cut", m_view, [this, selectedPaths] { m_callbacks.onCutRequested(selectedPaths); });
		menu.addSeparator();

		if (selectedPaths.count() == 1 && m_callbacks.onRenameRequested) {
			menu.addAction("Rename", m_view, [this, selectedPaths] {
				m_callbacks.onRenameRequested(selectedPaths.first());
			});
		}

		if (m_callbacks.onDeleteRequested)
			menu.addAction("Delete", m_view, [this, selectedPaths] { m_callbacks.onDeleteRequested(selectedPaths); });
		menu.addSeparator();
	}

	if (m_callbacks.onPasteRequested)
		menu.addAction("Paste", m_view, [this, currentDir] { m_callbacks.onPasteRequested(currentDir); });
	menu.addSeparator();
	if (m_callbacks.onCreateFileRequested)
		menu.addAction("New File", m_view, [this, currentDir] { m_callbacks.onCreateFileRequested(currentDir); });
	if (m_callbacks.onCreateFolderRequested)
		menu.addAction("New Folder", m_view, [this, currentDir] { m_callbacks.onCreateFolderRequested(currentDir); });

	menu.exec(event->globalPos());
	return true;
}

bool FileListEvents::hasEventView() const {
	return !m_view.isNull();
}

const char *FileListEvents::selectionInputSourceToString(const SelectionInputSource source) {
	switch (source) {
		case SelectionInputSource::Keyboard:
			return "keyboard";
		case SelectionInputSource::Mouse:
			return "mouse";
		case SelectionInputSource::Unknown:
			return "unknown/programmatic";
	}

	return "unknown/programmatic";
}

} // namespace dentry::app::events

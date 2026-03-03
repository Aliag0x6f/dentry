/**
 * @file Style.h
 * @brief Qt stylesheet (QSS) for the Dentry file manager.
 *
 * Dark theme visually matching the original Python/GTK version.
 * Portable across any Linux desktop environment.
 *
 * Color palette:
 * - Background:  #0a0a0a
 * - Surface:     #080808 / #111111
 * - Text:        #d7d7d7 / #eeeeee
 * - Selection:   #ffffff (bg) / #000000 (fg)
 * - Border:      rgba(255,255,255,0.08-0.15)
 *
 * @author Hugo Fabresse
 */

#pragma once

/**
 * @brief Returns the QSS stylesheet string for the Dentry application.
 * @return A QString containing the full QSS stylesheet.
 */
inline QString dentryStyle() {
    return R"(

/* ── Global ── */
* {
    font-family: "Maple Mono", "JetBrains Mono", "Fira Code", monospace;
    font-size: 9pt;
    color: #d7d7d7;
}

QMainWindow, QWidget {
    background-color: #0a0a0a;
}

/* ── Toolbar ── */
QToolBar {
    background-color: rgba(10, 10, 10, 240);
    border: none;
    border-bottom: 1px solid rgba(255, 255, 255, 20);
    padding: 4px 8px;
    spacing: 2px;
}

QToolBar::separator {
    background: rgba(255, 255, 255, 20);
    width: 1px;
    margin: 4px 4px;
}

QToolBar QToolButton {
    background: rgba(255, 255, 255, 13);
    color: #d7d7d7;
    border: 1px solid rgba(255, 255, 255, 25);
    border-radius: 4px;
    padding: 4px 10px;
    margin: 0px 2px;
}

QToolBar QToolButton:hover {
    background: rgba(255, 255, 255, 30);
    color: #ffffff;
    border-color: rgba(255, 255, 255, 51);
}

QToolBar QToolButton:pressed {
    background: #ffffff;
    color: #000000;
    border-color: #ffffff;
}

/* ── Search bar ── */
QLineEdit {
    background: rgba(255, 255, 255, 13);
    border: 1px solid rgba(255, 255, 255, 38);
    border-radius: 4px;
    color: #eeeeee;
    padding: 4px 8px;
    margin: 0px 4px;
    selection-background-color: #ffffff;
    selection-color: #000000;
}

QLineEdit:focus {
    border-color: rgba(255, 255, 255, 89);
    background: rgba(255, 255, 255, 20);
}

/* ── Path bar ── */
QLabel#pathLabel {
    background-color: rgba(255, 255, 255, 10);
    border-bottom: 1px solid rgba(255, 255, 255, 20);
    color: #eeeeee;
    font-weight: bold;
    font-size: 8pt;
    padding: 5px 12px;
}

/* ── Sidebar title ── */
QLabel#sidebarTitle {
    background-color: #080808;
    color: rgba(255, 255, 255, 76);
    font-size: 7pt;
    font-weight: bold;
    padding: 10px 12px 4px 12px;
}

/* ── Sidebar list ── */
QListWidget {
    background-color: #080808;
    color: #aaaaaa;
    border: none;
    border-right: 1px solid rgba(255, 255, 255, 20);
    outline: none;
}

QListWidget::item {
    padding: 4px 12px;
    border-radius: 4px;
    margin: 1px 6px;
}

QListWidget::item:hover {
    background: rgba(255, 255, 255, 15);
    color: #d7d7d7;
}

QListWidget::item:selected {
    background-color: #ffffff;
    color: #000000;
    border-radius: 4px;
}

QListWidget::item:selected:hover {
    background-color: #eeeeee;
    color: #000000;
}

/* ── File tree view ── */
QTreeView {
    background-color: #0a0a0a;
    color: #d7d7d7;
    border: none;
    alternate-background-color: #0d0d0d;
    outline: none;
    show-decoration-selected: 1;
}

QTreeView::item {
    padding: 3px 4px;
    border-radius: 4px;
}

QTreeView::item:hover {
    background-color: rgba(255, 255, 255, 10);
}

QTreeView::item:selected {
    background-color: #ffffff;
    color: #000000;
    border-radius: 4px;
}

QTreeView::item:selected:hover {
    background-color: #eeeeee;
    color: #000000;
}

/* ── Column headers ── */
QHeaderView {
    background-color: #111111;
    border: none;
}

QHeaderView::section {
    background-color: #111111;
    color: rgba(255, 255, 255, 102);
    border: none;
    border-right: 1px solid rgba(255, 255, 255, 15);
    border-bottom: none;
    padding: 5px 8px;
    font-size: 8pt;
    font-weight: bold;
}

QHeaderView::section:hover {
    background-color: #161616;
    color: #ffffff;
}

QHeaderView::section:last {
    border-right: none;
}

/* ── Status bar ── */
QStatusBar {
    background-color: #080808;
    border-top: 1px solid rgba(255, 255, 255, 20);
    color: rgba(255, 255, 255, 89);
    font-size: 8pt;
    padding: 3px 12px;
}

QStatusBar QLabel {
    color: rgba(255, 255, 255, 89);
    font-size: 8pt;
    padding: 0px;
    background: transparent;
}

/* ── Splitter ── */
QSplitter::handle {
    background-color: rgba(255, 255, 255, 15);
}

QSplitter::handle:horizontal {
    width: 1px;
}

/* ── Scrollbars ── */
QScrollBar:vertical {
    background: transparent;
    width: 6px;
    margin: 0px;
}

QScrollBar::handle:vertical {
    background: rgba(255, 255, 255, 30);
    border-radius: 3px;
    min-height: 20px;
}

QScrollBar::handle:vertical:hover {
    background: rgba(255, 255, 255, 64);
}

QScrollBar::add-line:vertical,
QScrollBar::sub-line:vertical,
QScrollBar::add-page:vertical,
QScrollBar::sub-page:vertical {
    background: transparent;
    height: 0px;
}

QScrollBar:horizontal {
    background: transparent;
    height: 6px;
    margin: 0px;
}

QScrollBar::handle:horizontal {
    background: rgba(255, 255, 255, 30);
    border-radius: 3px;
    min-width: 20px;
}

QScrollBar::handle:horizontal:hover {
    background: rgba(255, 255, 255, 64);
}

QScrollBar::add-line:horizontal,
QScrollBar::sub-line:horizontal,
QScrollBar::add-page:horizontal,
QScrollBar::sub-page:horizontal {
    background: transparent;
    width: 0px;
}

/* ── Context menu ── */
QMenu {
    background-color: #111111;
    color: #d7d7d7;
    border: 1px solid rgba(255, 255, 255, 30);
    border-radius: 6px;
    padding: 4px;
}

QMenu::item {
    padding: 6px 20px;
    border-radius: 4px;
    background: transparent;
}

QMenu::item:selected {
    background-color: #ffffff;
    color: #000000;
}

QMenu::separator {
    height: 1px;
    background: rgba(255, 255, 255, 20);
    margin: 3px 8px;
}

/* ── Input dialogs ── */
QDialog {
    background-color: #0f0f0f;
}

QInputDialog QLabel {
    color: #d7d7d7;
    background: transparent;
}

QInputDialog QLineEdit {
    background: rgba(255, 255, 255, 13);
    color: #eeeeee;
    border: 1px solid rgba(255, 255, 255, 38);
    border-radius: 4px;
    padding: 6px 10px;
    margin: 2px;
}

QInputDialog QLineEdit:focus {
    border-color: rgba(255, 255, 255, 89);
}

QInputDialog QPushButton {
    background: rgba(255, 255, 255, 18);
    color: #d7d7d7;
    border: 1px solid rgba(255, 255, 255, 30);
    border-radius: 4px;
    padding: 6px 16px;
    min-width: 70px;
}

QInputDialog QPushButton:hover {
    background: rgba(255, 255, 255, 35);
    color: #ffffff;
}

QInputDialog QPushButton:default {
    background: #ffffff;
    color: #000000;
    border: none;
}

/* ── Message boxes ── */
QMessageBox {
    background-color: #0f0f0f;
}

QMessageBox QLabel {
    color: #d7d7d7;
    background: transparent;
}

QMessageBox QPushButton {
    background: rgba(255, 255, 255, 18);
    color: #d7d7d7;
    border: 1px solid rgba(255, 255, 255, 30);
    border-radius: 4px;
    padding: 6px 16px;
    min-width: 70px;
}

QMessageBox QPushButton:hover {
    background: rgba(255, 255, 255, 35);
    color: #ffffff;
}

QMessageBox QPushButton:default {
    background: #ffffff;
    color: #000000;
    border: none;
}

)";
}


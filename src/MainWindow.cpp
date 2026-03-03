/**
 * @file MainWindow.cpp
 * @brief Implementation of the MainWindow class for the Dentry file manager.
 *
 * @author Hugo Fabresse
 */

#include "MainWindow.h"
#include "Style.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QToolBar>
#include <QAction>
#include <QInputDialog>
#include <QMessageBox>
#include <QMenu>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHeaderView>
#include <QDesktopServices>
#include <QUrl>
#include <QClipboard>
#include <QStandardPaths>
#include <QDateTime>
#include <QStandardItem>
#include <QToolButton>

// ── Helpers ────────────────────────────────────────────────────────────────

/**
 * @brief Converts a byte size to a human-readable string.
 * @param bytes File size in bytes.
 * @return Formatted string (e.g. "4.2 M").
 */
static QString humanSize(qint64 bytes) {
    if (bytes < 1024)             return QString("%1 B").arg(bytes);
    if (bytes < 1024 * 1024)      return QString("%1 K").arg(bytes / 1024.0, 0, 'f', 1);
    if (bytes < 1024 * 1024 * 1024) return QString("%1 M").arg(bytes / (1024.0 * 1024), 0, 'f', 1);
    return QString("%1 G").arg(bytes / (1024.0 * 1024 * 1024), 0, 'f', 1);
}

/**
 * @brief Returns a fixed-width formatted date string for column alignment.
 * @param info QFileInfo of the file.
 * @return Date string in "dd/MM/yyyy HH:mm" format.
 */
static QString formattedDate(const QFileInfo &info) {
    return info.lastModified().toString("dd/MM/yyyy HH:mm");
}

/**
 * @brief Returns the Unix permission string (e.g. "-rwxr-xr-x").
 * @param info QFileInfo of the file.
 * @return 10-character permission string.
 */
static QString permissionString(const QFileInfo &info) {
    QFile::Permissions p = info.permissions();
    QString s;
    s += info.isDir()                  ? "d" : "-";
    s += (p & QFile::ReadOwner)        ? "r" : "-";
    s += (p & QFile::WriteOwner)       ? "w" : "-";
    s += (p & QFile::ExeOwner)         ? "x" : "-";
    s += (p & QFile::ReadGroup)        ? "r" : "-";
    s += (p & QFile::WriteGroup)       ? "w" : "-";
    s += (p & QFile::ExeGroup)         ? "x" : "-";
    s += (p & QFile::ReadOther)        ? "r" : "-";
    s += (p & QFile::WriteOther)       ? "w" : "-";
    s += (p & QFile::ExeOther)         ? "x" : "-";
    return s;
}

// ── Constructor ────────────────────────────────────────────────────────────

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    currentPath = QDir::homePath();
    setupUI();
    applyStyle();
    loadDirectory(currentPath);
}

void MainWindow::applyStyle() {
    qApp->setStyleSheet(dentryStyle());
}

// ── UI Setup ───────────────────────────────────────────────────────────────

void MainWindow::setupUI() {
    resize(1100, 680);
    setWindowTitle("Dentry");

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    buildToolbar();

    // Path bar: full width, fixed height
    pathLabel = new QLabel;
    pathLabel->setObjectName("pathLabel");
    pathLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mainLayout->addWidget(pathLabel);

    // Splitter: sidebar | file view
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setHandleWidth(1);
    splitter->addWidget(buildSidebarWidget());
    splitter->addWidget(buildFileView());
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    splitter->setSizes({170, 900});

    mainLayout->addWidget(splitter);

    // Status bar
    statusBar = new QStatusBar(this);
    statusLabel = new QLabel;
    statusBar->addWidget(statusLabel);
    setStatusBar(statusBar);
}

// ── Sidebar ───────────────────────────────────────────────────────────────

QWidget* MainWindow::buildSidebarWidget() {
    QWidget *sideWidget = new QWidget;
    QVBoxLayout *sideLayout = new QVBoxLayout(sideWidget);
    sideLayout->setContentsMargins(0, 0, 0, 0);
    sideLayout->setSpacing(0);

    QLabel *sideTitle = new QLabel("PLACES");
    sideTitle->setObjectName("sidebarTitle");
    sideLayout->addWidget(sideTitle);

    sidebar = new QListWidget;
    sidebar->setMinimumWidth(160);
    sidebar->setMaximumWidth(220);
    buildSidebar();
    connect(sidebar, &QListWidget::itemClicked, this, &MainWindow::onSidebarClicked);
    sideLayout->addWidget(sidebar);

    return sideWidget;
}

void MainWindow::buildSidebar() {
    struct Place { QString label; QString path; };

    const QList<Place> places = {
        { "  Home",       QDir::homePath() },
        { "  Desktop",    QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) },
        { "  Documents",  QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) },
        { "  Downloads",  QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) },
        { "  Pictures",   QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) },
        { "  Videos",     QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) },
        { "  Music",      QStandardPaths::writableLocation(QStandardPaths::MusicLocation) },
        { "  .config",    QDir::homePath() + "/.config" },
        { "  /",          "/" },
    };

    for (const auto &p : places) {
        QListWidgetItem *item = new QListWidgetItem(p.label);
        item->setData(Qt::UserRole, p.path);
        sidebar->addItem(item);
    }
}

// ── File view ─────────────────────────────────────────────────────────────

QWidget* MainWindow::buildFileView() {
    model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({
        "Name", "Size", "Type", "Date Modified", "Permissions"
    });

    treeView = new QTreeView;
    treeView->setModel(model);
    treeView->setAlternatingRowColors(true);
    treeView->setSortingEnabled(true);
    treeView->sortByColumn(0, Qt::AscendingOrder);
    treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    treeView->header()->setSectionsMovable(false);
    treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeView->setRootIsDecorated(false);
    treeView->setItemsExpandable(false);
    treeView->setUniformRowHeights(true);

    treeView->header()->resizeSection(0, 300);
    treeView->header()->resizeSection(1,  80);
    treeView->header()->resizeSection(2, 110);
    treeView->header()->resizeSection(3, 145);
    treeView->header()->resizeSection(4, 110);

    connect(treeView, &QTreeView::activated,
            this, &MainWindow::onItemActivated);
    connect(treeView, &QTreeView::customContextMenuRequested,
            this, &MainWindow::showContextMenu);
    connect(treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, [this]{ updateStatusBar(); });

    return treeView;
}

// ── Toolbar ───────────────────────────────────────────────────────────────

void MainWindow::buildToolbar() {
    QToolBar *toolbar = addToolBar("main");
    toolbar->setMovable(false);
    toolbar->setFloatable(false);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextOnly);

    toolbar->addAction("↑ Back",   this, &MainWindow::goUp)->setToolTip("Parent directory");
    toolbar->addAction("⌂ Home",   this, [this]{ loadDirectory(QDir::homePath()); })->setToolTip("Home");
    toolbar->addSeparator();
    toolbar->addAction("+ File",   this, &MainWindow::createFile)->setToolTip("Create file");
    toolbar->addAction("+ Folder", this, &MainWindow::createFolder)->setToolTip("Create folder");
    toolbar->addAction("✎ Rename", this, &MainWindow::renameSelected)->setToolTip("Rename");
    toolbar->addAction("✕ Delete", this, &MainWindow::deleteSelected)->setToolTip("Delete");
    toolbar->addSeparator();

    // Hidden files toggle button with dynamic black/white style
    hiddenBtn = new QToolButton;
    hiddenBtn->setText("⊙ Hidden");
    hiddenBtn->setToolTip("Show / hide dot-files");
    hiddenBtn->setCheckable(false);
    hiddenBtn->setObjectName("hiddenBtn");
    connect(hiddenBtn, &QToolButton::clicked, this, &MainWindow::toggleHidden);
    updateHiddenButtonStyle();
    toolbar->addWidget(hiddenBtn);

    // Spacer
    QWidget *spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolbar->addWidget(spacer);

    // Search bar
	searchBox = new QLineEdit;
    searchBox->setPlaceholderText("Search...");
    searchBox->setFixedWidth(200);
    connect(searchBox, &QLineEdit::textChanged, this, &MainWindow::onSearchChanged);
    toolbar->addWidget(searchBox);
}

void MainWindow::updateHiddenButtonStyle() {
    if (showHidden) {
        hiddenBtn->setStyleSheet(
            "QToolButton {"
            "  background: #ffffff;"
            "  color: #000000;"
            "  border: 1px solid #ffffff;"
            "  border-radius: 4px;"
            "  padding: 4px 10px;"
            "}"
            "QToolButton:hover {"
            "  background: #dddddd;"
            "  color: #000000;"
            "}"
        );
    } else {
        hiddenBtn->setStyleSheet(
            "QToolButton {"
            "  background: rgba(255,255,255,13);"
            "  color: #d7d7d7;"
            "  border: 1px solid rgba(255,255,255,25);"
            "  border-radius: 4px;"
            "  padding: 4px 10px;"
            "}"
            "QToolButton:hover {"
            "  background: rgba(255,255,255,30);"
            "  color: #ffffff;"
            "  border-color: rgba(255,255,255,51);"
            "}"
        );
    }
}

// ── Directory loading ──────────────────────────────────────────────────────

void MainWindow::loadDirectory(const QString &path) {
    if (!QDir(path).exists()) return;
    currentPath = path;
    pathLabel->setText("  " + path);

    model->removeRows(0, model->rowCount());

    QDir dir(path);
    QDir::Filters filters = QDir::AllEntries | QDir::NoDotAndDotDot | QDir::System;
    if (showHidden) filters |= QDir::Hidden;

    const QFileInfoList entries = dir.entryInfoList(
        filters, QDir::DirsFirst | QDir::Name | QDir::IgnoreCase);

    QFont monoFont("Maple Mono");
    monoFont.setStyleHint(QFont::Monospace);

    for (const QFileInfo &fi : entries) {
        if (!searchFilter.isEmpty() &&
            !fi.fileName().contains(searchFilter, Qt::CaseInsensitive))
            continue;

        // Col 0 — Name
        QStandardItem *nameItem = new QStandardItem(fi.fileName());
        nameItem->setData(fi.absoluteFilePath(), Qt::UserRole);
        nameItem->setData(fi.isDir(), Qt::UserRole + 1);

        // Col 1 — Size
        QStandardItem *sizeItem = new QStandardItem(fi.isDir() ? "" : humanSize(fi.size()));
        sizeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        // Col 2 — Type
        QStandardItem *typeItem = new QStandardItem(
            fi.isDir() ? "Folder" : (fi.suffix().isEmpty() ? "File" : fi.suffix().toUpper())
        );

        // Col 3 — Date (monospace for alignment)
        QStandardItem *dateItem = new QStandardItem(formattedDate(fi));
        dateItem->setFont(monoFont);

        // Col 4 — Permissions (monospace for alignment)
        QStandardItem *permItem = new QStandardItem(permissionString(fi));
        permItem->setFont(monoFont);

        for (auto *item : {nameItem, sizeItem, typeItem, dateItem, permItem})
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        model->appendRow({nameItem, sizeItem, typeItem, dateItem, permItem});
    }

    updateStatusBar();
}

void MainWindow::updateStatusBar() {
    QDir dir(currentPath);
    QDir::Filters filters = QDir::AllEntries | QDir::NoDotAndDotDot | QDir::System;
    if (showHidden) filters |= QDir::Hidden;

    const int total = dir.entryInfoList(filters).count();
    const int dirs  = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot).count();
    const int files = total - dirs;

    const int sel = treeView->selectionModel()->selectedRows().count();
    const QString selStr = sel > 0
        ? QString("  ·  %1 selected").arg(sel)
        : "";

    statusLabel->setText(
        QString("%1 folder(s)  ·  %2 file(s)%3").arg(dirs).arg(files).arg(selStr)
    );
}

// ── Navigation slots ───────────────────────────────────────────────────────

void MainWindow::onItemActivated(const QModelIndex &index) {
    const QModelIndex col0 = model->index(index.row(), 0);
    const QString path = col0.data(Qt::UserRole).toString();
    const bool isDir   = col0.data(Qt::UserRole + 1).toBool();

    if (isDir)
        loadDirectory(path);
    else
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void MainWindow::onSidebarClicked(QListWidgetItem *item) {
    const QString path = item->data(Qt::UserRole).toString();
    if (QDir(path).exists()) loadDirectory(path);
}

void MainWindow::goUp() {
    QDir dir(currentPath);
    if (dir.cdUp()) loadDirectory(dir.absolutePath());
}

void MainWindow::toggleHidden() {
    showHidden = !showHidden;
    updateHiddenButtonStyle();
    loadDirectory(currentPath);
}

void MainWindow::onSearchChanged(const QString &text) {
    searchFilter = text;
    loadDirectory(currentPath);
}

// ── File operation slots ───────────────────────────────────────────────────

void MainWindow::createFile() {
    bool ok;
    const QString name = QInputDialog::getText(
        this, "Create file", "Name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;

    QFile f(currentPath + "/" + name);
    if (!f.open(QIODevice::WriteOnly))
        QMessageBox::critical(this, "Error", f.errorString());
    else {
        f.close();
        loadDirectory(currentPath);
    }
}

void MainWindow::createFolder() {
    bool ok;
    const QString name = QInputDialog::getText(
        this, "Create folder", "Name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;

    if (!QDir(currentPath).mkdir(name))
        QMessageBox::critical(this, "Error", "Could not create folder.");
    else
        loadDirectory(currentPath);
}

void MainWindow::renameSelected() {
    const auto indexes = treeView->selectionModel()->selectedRows();
    if (indexes.isEmpty()) return;

    const QString oldPath = indexes.first().data(Qt::UserRole).toString();
    const QString oldName = QFileInfo(oldPath).fileName();

    bool ok;
    const QString newName = QInputDialog::getText(
        this, "Rename", "New name:", QLineEdit::Normal, oldName, &ok);
    if (!ok || newName.isEmpty() || newName == oldName) return;

    const QString newPath = QFileInfo(oldPath).dir().absolutePath() + "/" + newName;
    if (!QFile::rename(oldPath, newPath))
        QMessageBox::critical(this, "Error", "Could not rename.");
    else
        loadDirectory(currentPath);
}

void MainWindow::deleteSelected() {
    const auto indexes = treeView->selectionModel()->selectedRows();
    if (indexes.isEmpty()) return;

    QStringList names;
    for (const auto &idx : indexes)
        names << QFileInfo(idx.data(Qt::UserRole).toString()).fileName();

    const QString preview = names.count() <= 5
        ? names.join("\n")
        : names.mid(0, 5).join("\n") + QString("\n... and %1 more").arg(names.count() - 5);

    const auto reply = QMessageBox::warning(
        this, "Delete",
        QString("Delete %1 item(s)?\n\n%2").arg(names.count()).arg(preview),
        QMessageBox::Ok | QMessageBox::Cancel);

    if (reply != QMessageBox::Ok) return;

    for (const auto &idx : indexes) {
        const QString path = idx.data(Qt::UserRole).toString();
        if (QFileInfo(path).isDir())
            QDir(path).removeRecursively();
        else
            QFile::remove(path);
    }
    loadDirectory(currentPath);
}

// ── Context menu ───────────────────────────────────────────────────────────

void MainWindow::showContextMenu(const QPoint &pos) {
    QMenu menu(this);

    menu.addAction("+ Create file",   this, &MainWindow::createFile);
    menu.addAction("+ Create folder", this, &MainWindow::createFolder);
    menu.addSeparator();
    menu.addAction("✎  Rename",       this, &MainWindow::renameSelected);
    menu.addAction("✕  Delete",       this, &MainWindow::deleteSelected);
    menu.addSeparator();
    menu.addAction("⎘  Copy path",    this, [this] {
        const auto indexes = treeView->selectionModel()->selectedRows();
        if (!indexes.isEmpty())
            QApplication::clipboard()->setText(indexes.first().data(Qt::UserRole).toString());
    });

    menu.exec(treeView->viewport()->mapToGlobal(pos));
}


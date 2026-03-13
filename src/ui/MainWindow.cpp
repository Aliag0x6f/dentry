/**
 * @file MainWindow.cpp
 * @brief Implementation of MainWindow.
 *
 * @author Hugo Fabresse
 */

#include "MainWindow.h"
#include <QStandardPaths>
#include <QVBoxLayout>
#include <QWidget>
#include <QDir>

namespace Dentry::Ui {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    build();
    connectSignals();
    navigateTo(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
}

void MainWindow::build() {
    setWindowTitle("Dentry");
    setMinimumSize(800, 500);
    resize(1100, 650);

    m_model = new Model::FileSystemModel(this);

    m_toolbar = new Toolbar(this);
    addToolBar(m_toolbar);

    m_statusBar = new Ui::StatusBar(this);
    setStatusBar(m_statusBar);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout *layout = new QHBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_splitter = new QSplitter(Qt::Horizontal, central);

    m_sidebar = new Sidebar(m_splitter);

    m_fileListView = new FileListView(m_splitter);
    m_fileListView->setModel(m_model);

    m_previewPanel = new PreviewPanel(m_splitter);

    m_splitter->addWidget(m_sidebar);
    m_splitter->addWidget(m_fileListView);
    m_splitter->addWidget(m_previewPanel);
    m_splitter->setStretchFactor(0, 0);
    m_splitter->setStretchFactor(1, 1);
    m_splitter->setStretchFactor(2, 0);

    layout->addWidget(m_splitter);
}

void MainWindow::connectSignals() {
    connect(m_toolbar,      &Toolbar::backRequested,        this,   &MainWindow::navigateBack);
    connect(m_toolbar,      &Toolbar::homeRequested,        this,   &MainWindow::navigateHome);
    connect(m_toolbar,      &Toolbar::searchChanged,        m_model, &Model::FileSystemModel::setFilter);
    connect(m_toolbar,      &Toolbar::hiddenToggled,        m_model, &Model::FileSystemModel::setShowHidden);

    connect(m_sidebar,      &Sidebar::placeSelected,        this,   &MainWindow::navigateTo);

    connect(m_fileListView, &FileListView::directoryRequested, this, &MainWindow::navigateTo);
    connect(m_fileListView, &FileListView::fileActivated,   m_previewPanel, &PreviewPanel::preview);
    connect(m_fileListView, &FileListView::selectionChanged, m_statusBar,   &StatusBar::setSelection);

    connect(m_model,        &Model::FileSystemModel::directoryLoaded, this, &MainWindow::onDirectoryLoaded);
}

void MainWindow::navigateTo(const QString &path) {
    QDir dir(path);
    if (!dir.exists())
        return;

    m_history.push(path);
    m_model->setDirectory(path);
    m_toolbar->setPath(path);
    m_previewPanel->clear();
}

void MainWindow::navigateBack() {
    if (m_history.size() <= 1)
        return;

    m_history.pop();
    const QString path = m_history.top();
    m_model->setDirectory(path);
    m_toolbar->setPath(path);
    m_previewPanel->clear();
}

void MainWindow::navigateHome() {
    navigateTo(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
}

void MainWindow::onDirectoryLoaded(const QString &) {
    const QList<Model::FileItem> entries = m_model->entries();

    int folders = 0;
    int files   = 0;

    for (const Model::FileItem &item : entries) {
        if (item.isDir) ++folders;
        else            ++files;
    }

    m_statusBar->setDirectoryStats(folders, files);
}

} // namespace Dentry::Ui

/**
 * @file MainWindow.cpp
 * @brief Implementation of MainWindow.
 *
 * @author Hugo Fabresse
 */

#include "MainWindow.h"

#include <QDir>
#include <QStandardPaths>

#include "../util/Logger.h"

namespace Dentry::Ui {

    MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , m_navigationController(nullptr) {
        build();
        m_navigationController->navigateTo(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    }

    void MainWindow::build() {
        setWindowTitle("Dentry");
        setupSize();

        m_model     = new Model::FileSystemModel(this);
        m_toolbar   = new Toolbar(this);
        m_statusBar = new StatusBar(this);
        m_central   = new CentralWidget(m_model, this);
        m_fileOperationController = new App::FileOperationController(m_model, this, this);
        m_navigationController    = new App::NavigationController(m_model, this);

        addToolBar(m_toolbar);
        setStatusBar(m_statusBar);
        setCentralWidget(m_central);

        setupConnections();

        LOG_INFO("Ui") << "MainWindow built";
    }

    void MainWindow::setupSize() {
        setMinimumSize(800, 500);
        resize(1100, 650);
    }

    void MainWindow::setupConnections() {
        auto *sidebar      = m_central->sidebar();
        auto *fileListView = m_central->fileListView();

        connect(m_navigationController, &App::NavigationController::pathChanged, m_toolbar, &Toolbar::setPath);
        connect(m_navigationController, &App::NavigationController::pathChanged, this, [this](const QString &) {
            m_central->previewPanel()->clear();
        });

        connect(m_toolbar, &Toolbar::backRequested, m_navigationController, &App::NavigationController::navigateBack);
        connect(m_toolbar, &Toolbar::homeRequested, m_navigationController, &App::NavigationController::navigateHome);
        connect(m_toolbar, &Toolbar::searchChanged, m_model, &Model::FileSystemModel::setFilter);
        connect(m_toolbar, &Toolbar::hiddenToggled, m_model, &Model::FileSystemModel::setShowHidden);
        connect(m_toolbar, &Toolbar::hiddenToggled, sidebar, &Sidebar::setShowHidden);

        connect(sidebar, &Sidebar::placeSelected, m_navigationController, &App::NavigationController::navigateTo);

        connect(fileListView, &FileListView::directoryRequested,    m_navigationController, &App::NavigationController::navigateTo);
        connect(fileListView, &FileListView::selectionChanged, this, [this](const QList<Model::FileItem> &selected) {
            if (selected.count() == 1 && !selected.first().isDir)
                m_central->previewPanel()->preview(selected.first());
            else
                m_central->previewPanel()->clear();
        });
        connect(fileListView, &FileListView::selectionChanged,      m_statusBar,            &StatusBar::setSelection);
        connect(fileListView, &FileListView::deleteRequested,       m_fileOperationController, &App::FileOperationController::onDeleteRequested);
        connect(fileListView, &FileListView::renameRequested,       m_fileOperationController, &App::FileOperationController::onRenameRequested);
        connect(fileListView, &FileListView::createFileRequested,   m_fileOperationController, &App::FileOperationController::onCreateFileRequested);
        connect(fileListView, &FileListView::createFolderRequested, m_fileOperationController, &App::FileOperationController::onCreateFolderRequested);
        connect(fileListView, &FileListView::copyRequested,         m_fileOperationController, &App::FileOperationController::onCopyRequested);
        connect(fileListView, &FileListView::cutRequested,          m_fileOperationController, &App::FileOperationController::onCutRequested);
        connect(fileListView, &FileListView::pasteRequested,        m_fileOperationController, &App::FileOperationController::onPasteRequested);

        connect(m_model, &Model::FileSystemModel::directoryLoaded, this, &MainWindow::onDirectoryLoaded);

        LOG_DEBUG("Ui") << "All signals connected";
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

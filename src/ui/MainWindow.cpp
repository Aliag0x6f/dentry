/**
 * @file MainWindow.cpp
 * @brief Implementation of MainWindow.
 *
 * @author Hugo Fabresse
 */

#include "MainWindow.h"
#include "../log/Logger.h"

#include <QApplication>

namespace dentry::ui {

    MainWindow::MainWindow(QWidget *parent)
        : UIComponent(parent) {
        setupWidgets();
        build();
        m_navigationController->navigateHome();
        log::info("Ui") << "MainWindow built";
    }

    void MainWindow::setupWidgets() {
        m_model     = new model::FileSystemModel(this);
        m_toolbar   = new Toolbar(this);
        m_statusBar = new StatusBar(this);
        m_central   = new CentralWidget(m_model, this);
        m_fileOperationController = new app::FileOperationController(m_model, this, this);
        m_navigationController    = new app::NavigationController(m_model, this);

        addToolBar(m_toolbar);
        setStatusBar(m_statusBar);
        setCentralWidget(m_central);
    }

    void MainWindow::setupStyle() {
        setWindowTitle(QApplication::applicationDisplayName());
    }

    void MainWindow::setupSize() {
        setMinimumSize(800, 500);
        resize(1100, 650);
    }

    void MainWindow::setupConnections() {
        auto *sidebar      = m_central->sidebar();
        auto *fileListView = m_central->fileListView();

        connect(m_navigationController, &app::NavigationController::pathChanged, m_toolbar, &Toolbar::setPath);
        connect(m_navigationController, &app::NavigationController::pathChanged, this, [this](const QString &) {
            m_central->clearPreview();
        });

        connect(m_toolbar, &Toolbar::backRequested, m_navigationController, &app::NavigationController::navigateBack);
        connect(m_toolbar, &Toolbar::homeRequested, m_navigationController, &app::NavigationController::navigateHome);
        connect(m_toolbar, &Toolbar::searchChanged, m_model, &model::FileSystemModel::setFilter);
        connect(m_toolbar, &Toolbar::hiddenToggled, m_model, &model::FileSystemModel::setShowHidden);
        connect(m_toolbar, &Toolbar::hiddenToggled, m_central, &CentralWidget::setSidebarShowHidden);

        connect(sidebar, &Sidebar::placeSelected, m_navigationController, &app::NavigationController::navigateTo);

        connect(fileListView, &FileListView::directoryRequested,    m_navigationController, &app::NavigationController::navigateTo);
        connect(fileListView, &FileListView::selectionChanged,      m_central, &CentralWidget::updatePreviewFromSelection);
        connect(fileListView, &FileListView::selectionChanged,      m_statusBar, &StatusBar::setSelection);
        connect(fileListView, &FileListView::deleteRequested,       m_fileOperationController, &app::FileOperationController::onDeleteRequested);
        connect(fileListView, &FileListView::renameRequested,       m_fileOperationController, &app::FileOperationController::onRenameRequested);
        connect(fileListView, &FileListView::createFileRequested,   m_fileOperationController, &app::FileOperationController::onCreateFileRequested);
        connect(fileListView, &FileListView::createFolderRequested, m_fileOperationController, &app::FileOperationController::onCreateFolderRequested);
        connect(fileListView, &FileListView::copyRequested,         m_fileOperationController, &app::FileOperationController::onCopyRequested);
        connect(fileListView, &FileListView::cutRequested,          m_fileOperationController, &app::FileOperationController::onCutRequested);
        connect(fileListView, &FileListView::pasteRequested,        m_fileOperationController, &app::FileOperationController::onPasteRequested);

        connect(m_model, &model::FileSystemModel::directoryLoaded, this, &MainWindow::onDirectoryLoaded);

        log::debug("Ui") << "All signals connected";
    }

    void MainWindow::onDirectoryLoaded(const QString &) {
        const QList<model::FileItem> entries = m_model->entries();

        int folders = 0;
        int files   = 0;

        for (const model::FileItem &item : entries) {
            if (item.isDir) ++folders;
            else            ++files;
        }

        m_statusBar->setDirectoryStats(folders, files);
    }


} // namespace dentry::ui

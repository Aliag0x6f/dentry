/**
 * @file MainWindow.cpp
 * @brief Implementation of MainWindow.
 *
 * @author Hugo Fabresse
 */

#include "MainWindow.h"
#include "components/ProgressDialog.h"
#include "navigation/NavigationController.h"

#include <QDir>
#include <QFileInfo>
#include <QInputDialog>
#include <QStandardPaths>

#include "../fs/operations/CopyOperation.h"
#include "../fs/operations/CreateFileOperation.h"
#include "../fs/operations/CreateFolderOperation.h"
#include "../fs/operations/DeleteOperation.h"
#include "../fs/operations/MoveOperation.h"
#include "../fs/operations/RenameOperation.h"
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
        m_navigationController = new NavigationController(m_model, this);

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

        connect(m_navigationController, &NavigationController::pathChanged, m_toolbar, &Toolbar::setPath);
        connect(m_navigationController, &NavigationController::pathChanged, this, [this](const QString &) {
            m_central->previewPanel()->clear();
        });

        connect(m_toolbar, &Toolbar::backRequested, m_navigationController,    &NavigationController::navigateBack);
        connect(m_toolbar, &Toolbar::homeRequested, m_navigationController,    &NavigationController::navigateHome);
        connect(m_toolbar, &Toolbar::searchChanged, m_model, &Model::FileSystemModel::setFilter);
        connect(m_toolbar, &Toolbar::hiddenToggled, m_model, &Model::FileSystemModel::setShowHidden);
        connect(m_toolbar, &Toolbar::hiddenToggled, sidebar, &Sidebar::setShowHidden);

        connect(sidebar, &Sidebar::placeSelected, m_navigationController, &NavigationController::navigateTo);

        connect(fileListView, &FileListView::directoryRequested,    m_navigationController,                   &NavigationController::navigateTo);
        connect(fileListView, &FileListView::selectionChanged, this, [this](const QList<Model::FileItem> &selected) {
            if (selected.count() == 1 && !selected.first().isDir)
                m_central->previewPanel()->preview(selected.first());
            else
                m_central->previewPanel()->clear();
        });
        connect(fileListView, &FileListView::selectionChanged,      m_statusBar,            &StatusBar::setSelection);
        connect(fileListView, &FileListView::deleteRequested,       this,        &MainWindow::onDeleteRequested);
        connect(fileListView, &FileListView::renameRequested,       this,        &MainWindow::onRenameRequested);
        connect(fileListView, &FileListView::createFileRequested,   this,        &MainWindow::onCreateFileRequested);
        connect(fileListView, &FileListView::createFolderRequested, this,        &MainWindow::onCreateFolderRequested);
        connect(fileListView, &FileListView::copyRequested,         this,        &MainWindow::onCopyRequested);
        connect(fileListView, &FileListView::cutRequested,          this,        &MainWindow::onCutRequested);
        connect(fileListView, &FileListView::pasteRequested,        this,        &MainWindow::onPasteRequested);

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

    // ── Operations ────────────────────────────────────────────────────────────

    void MainWindow::onCopyRequested(const QStringList &paths) {
        LOG_INFO("Ui") << "Copy requested:" << paths.count() << "item(s)";
        m_clipboard.copy(paths);
    }

    void MainWindow::onCutRequested(const QStringList &paths) {
        LOG_INFO("Ui") << "Cut requested:" << paths.count() << "item(s)";
        m_clipboard.cut(paths);
    }

    void MainWindow::onPasteRequested(const QString &destination) {
        if (m_clipboard.isEmpty()) {
            LOG_WARNING("Ui") << "Paste requested but clipboard is empty";
            return;
        }

        LOG_INFO("Ui") << "Paste requested into:" << destination
                       << "(" << (m_clipboard.isCut() ? "move" : "copy") << ")";

        Fs::AFileOperation *op = m_clipboard.isCut()
            ? static_cast<Fs::AFileOperation *>(new Fs::MoveOperation(m_clipboard.paths(), destination, nullptr))
            : static_cast<Fs::AFileOperation *>(new Fs::CopyOperation(m_clipboard.paths(), destination, nullptr));

        auto *dialog = new ProgressDialog(op, this);

        op->setParent(dialog);
        dialog->setAttribute(Qt::WA_DeleteOnClose);

        connect(op, &Fs::AFileOperation::finished, this, [this](bool success, const QString &) {
            if (success) m_model->refresh();
            if (m_clipboard.isCut() || success) m_clipboard.clear();
        });
        op->execute();
        dialog->exec();
    }

    void MainWindow::onDeleteRequested(const QStringList &paths) {
        LOG_INFO("Ui") << "Delete requested:" << paths.count() << "item(s)";

        auto *op     = new Fs::DeleteOperation(paths, nullptr);
        auto *dialog = new ProgressDialog(op, this);

        op->setParent(dialog);
        dialog->setAttribute(Qt::WA_DeleteOnClose);

        connect(op, &Fs::AFileOperation::finished, this, [this](bool success, const QString &) {
            if (success) m_model->refresh();
        });
        op->execute();
        dialog->exec();
    }

    void MainWindow::onRenameRequested(const QString &path) {
        const QString oldName = QFileInfo(path).fileName();
        bool ok;
        const QString newName = QInputDialog::getText(
            this, "Rename", "New name:", QLineEdit::Normal, oldName, &ok
        );

        if (!ok || newName.isEmpty() || newName == oldName)
            return;

        LOG_INFO("Ui") << "Rename requested:" << oldName << "->" << newName;

        auto *op     = new Fs::RenameOperation(path, newName, nullptr);
        auto *dialog = new ProgressDialog(op, this);

        op->setParent(dialog);
        dialog->setAttribute(Qt::WA_DeleteOnClose);

        connect(op, &Fs::AFileOperation::finished, this, [this](bool success, const QString &) {
            if (success) m_model->refresh();
        });
        op->execute();
        dialog->exec();
    }

    void MainWindow::onCreateFileRequested(const QString &directory) {
        bool ok;
        const QString name = QInputDialog::getText(
            this, "New File", "File name:", QLineEdit::Normal, "new_file.txt", &ok
        );

        if (!ok || name.isEmpty())
            return;

        LOG_INFO("Ui") << "Create file requested:" << name << "in" << directory;

        auto *op     = new Fs::CreateFileOperation(directory, name, nullptr);
        auto *dialog = new ProgressDialog(op, this);

        op->setParent(dialog);
        dialog->setAttribute(Qt::WA_DeleteOnClose);

        connect(op, &Fs::AFileOperation::finished, this, [this](bool success, const QString &) {
            if (success) m_model->refresh();
        });
        op->execute();
        dialog->exec();
    }

    void MainWindow::onCreateFolderRequested(const QString &directory) {
        bool ok;
        const QString name = QInputDialog::getText(
            this, "New Folder", "Folder name:", QLineEdit::Normal, "new_folder", &ok
        );

        if (!ok || name.isEmpty())
            return;

        LOG_INFO("Ui") << "Create folder requested:" << name << "in" << directory;

        auto *op     = new Fs::CreateFolderOperation(directory, name, nullptr);
        auto *dialog = new ProgressDialog(op, this);

        op->setParent(dialog);
        dialog->setAttribute(Qt::WA_DeleteOnClose);

        connect(op, &Fs::AFileOperation::finished, this, [this](bool success, const QString &) {
            if (success) m_model->refresh();
        });
        op->execute();
        dialog->exec();
    }

} // namespace Dentry::Ui

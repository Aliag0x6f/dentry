/**
 * @file FileOperationController.h
 * @brief Controller for file operations (copy, cut, paste, delete, rename, create).
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "../../app/Clipboard.h"
#include "../../model/FileSystemModel.h"

#include <QObject>
#include <QString>
#include <QStringList>
#include <QWidget>

#include <functional>

namespace Dentry::Fs {
class AFileOperation;
}

namespace Dentry::App {

/**
 * @class FileOperationController
 * @brief Handles all file operations triggered from the UI.
 *
 * Encapsulates copy/cut/paste/delete/rename/create logic and owns
 * the internal clipboard. MainWindow only needs to connect file-list
 * signals to this controller's slots.
 *
 * Example:
 * @code
 * auto *fileOps = new FileOperationController(model, parentWidget, this);
 *
 * connect(fileListView, &FileListView::copyRequested,         fileOps, &FileOperationController::onCopyRequested);
 * connect(fileListView, &FileListView::cutRequested,          fileOps, &FileOperationController::onCutRequested);
 * connect(fileListView, &FileListView::pasteRequested,        fileOps, &FileOperationController::onPasteRequested);
 * connect(fileListView, &FileListView::deleteRequested,       fileOps, &FileOperationController::onDeleteRequested);
 * connect(fileListView, &FileListView::renameRequested,       fileOps, &FileOperationController::onRenameRequested);
 * connect(fileListView, &FileListView::createFileRequested,   fileOps, &FileOperationController::onCreateFileRequested);
 * connect(fileListView, &FileListView::createFolderRequested, fileOps, &FileOperationController::onCreateFolderRequested);
 * @endcode
 */
class FileOperationController : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructs the FileOperationController.
     * @param model        The file system model to refresh after operations.
     * @param dialogParent The widget used as parent for progress/input dialogs.
     * @param parent       Optional Qt parent object.
     */
    explicit FileOperationController(Model::FileSystemModel *model,
                                     QWidget               *dialogParent,
                                     QObject               *parent = nullptr);

    ~FileOperationController() override = default;

    FileOperationController(const FileOperationController &)            = delete;
    FileOperationController &operator=(const FileOperationController &) = delete;
    FileOperationController(FileOperationController &&)                 = delete;
    FileOperationController &operator=(FileOperationController &&)      = delete;

public slots:
    /**
     * @brief Copies the given paths to the internal clipboard.
     */
    void onCopyRequested(const QStringList &paths);

    /**
     * @brief Cuts the given paths to the internal clipboard.
     */
    void onCutRequested(const QStringList &paths);

    /**
     * @brief Pastes the clipboard contents into the given directory.
     */
    void onPasteRequested(const QString &destination);

    /**
     * @brief Deletes the given paths.
     */
    void onDeleteRequested(const QStringList &paths);

    /**
     * @brief Prompts for a new name and renames the given path.
     */
    void onRenameRequested(const QString &path);

    /**
     * @brief Prompts for a name and creates a new file in the given directory.
     */
    void onCreateFileRequested(const QString &directory);

    /**
     * @brief Prompts for a name and creates a new folder in the given directory.
     */
    void onCreateFolderRequested(const QString &directory);

private:
    void runOperation(Fs::AFileOperation *operation, const std::function<void(bool)> &onFinished);

    Model::FileSystemModel *m_model;
    QWidget                *m_dialogParent;
    Clipboard               m_clipboard;
};

} // namespace Dentry::App

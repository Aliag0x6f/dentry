/**
 * @file FileOperationController.cpp
 * @brief Implementation of FileOperationController.
 *
 * @author Hugo Fabresse
 */

#include "FileOperationController.h"
#include "../../ui/components/ProgressDialog.h"

#include <QFileInfo>
#include <QInputDialog>

#include "../../fs/operations/CopyOperation.h"
#include "../../fs/operations/CreateFileOperation.h"
#include "../../fs/operations/CreateFolderOperation.h"
#include "../../fs/operations/DeleteOperation.h"
#include "../../fs/operations/MoveOperation.h"
#include "../../fs/operations/RenameOperation.h"
#include "../../util/Logger.h"

namespace Dentry::App {

    void FileOperationController::runOperation(std::unique_ptr<Fs::AFileOperation> operation,
                                               const std::function<void(bool)>     &onFinished) {
        if (!operation)
            return;

        Fs::AFileOperation *op = operation.release();
        Ui::ProgressDialog dialog(op, m_dialogParent);

        // not working for the moment
        /*connect(op, &Fs::AFileOperation::finished, op, [op](bool, const QString &) {
            op->deleteLater();
        });*/

        connect(op, &Fs::AFileOperation::finished, this, [onFinished](bool success, const QString &) {
            onFinished(success);
        });

        op->execute();
        dialog.exec();
    }

    FileOperationController::FileOperationController(Model::FileSystemModel *model,
                                                     QWidget               *dialogParent,
                                                     QObject               *parent)
        : QObject(parent)
        , m_model(model)
        , m_dialogParent(dialogParent) {
    }

    void FileOperationController::onCopyRequested(const QStringList &paths) {
        LOG_INFO("FileOps") << "Copy requested:" << paths.count() << "item(s)";
        m_clipboard.copy(paths);
    }

    void FileOperationController::onCutRequested(const QStringList &paths) {
        LOG_INFO("FileOps") << "Cut requested:" << paths.count() << "item(s)";
        m_clipboard.cut(paths);
    }

    void FileOperationController::onPasteRequested(const QString &destination) {
        if (m_clipboard.isEmpty()) {
            LOG_WARNING("FileOps") << "Paste requested but clipboard is empty";
            return;
        }

        LOG_INFO("FileOps") << "Paste requested into:" << destination
                            << "(" << (m_clipboard.isCut() ? "move" : "copy") << ")";

        std::unique_ptr<Fs::AFileOperation> op;
        if (m_clipboard.isCut()) {
            op = std::make_unique<Fs::MoveOperation>(m_clipboard.paths(), destination, nullptr);
        } else {
            op = std::make_unique<Fs::CopyOperation>(m_clipboard.paths(), destination, nullptr);
        }

        runOperation(std::move(op), [this](bool success) {
            if (success && m_model)
                m_model->refresh();
            if (m_clipboard.isCut() || success)
                m_clipboard.clear();
        });
    }

    void FileOperationController::onDeleteRequested(const QStringList &paths) {
        LOG_INFO("FileOps") << "Delete requested:" << paths.count() << "item(s)";

        auto op = std::make_unique<Fs::DeleteOperation>(paths, nullptr);
        runOperation(std::move(op), [this](bool success) {
            if (success && m_model)
                m_model->refresh();
        });
    }

    void FileOperationController::onRenameRequested(const QString &path) {
        const QString oldName = QFileInfo(path).fileName();
        bool ok;
        const QString newName = QInputDialog::getText(
            m_dialogParent, "Rename", "New name:", QLineEdit::Normal, oldName, &ok
        );

        if (!ok || newName.isEmpty() || newName == oldName)
            return;

        LOG_INFO("FileOps") << "Rename requested:" << oldName << "->" << newName;

        auto op = std::make_unique<Fs::RenameOperation>(path, newName, nullptr);
        runOperation(std::move(op), [this](bool success) {
            if (success && m_model)
                m_model->refresh();
        });
    }

    void FileOperationController::onCreateFileRequested(const QString &directory) {
        bool ok;
        const QString name = QInputDialog::getText(
            m_dialogParent, "New File", "File name:", QLineEdit::Normal, "new_file.txt", &ok
        );

        if (!ok || name.isEmpty())
            return;

        LOG_INFO("FileOps") << "Create file requested:" << name << "in" << directory;

        auto op = std::make_unique<Fs::CreateFileOperation>(directory, name, nullptr);
        runOperation(std::move(op), [this](bool success) {
            if (success && m_model)
                m_model->refresh();
        });
    }

    void FileOperationController::onCreateFolderRequested(const QString &directory) {
        bool ok;
        const QString name = QInputDialog::getText(
            m_dialogParent, "New Folder", "Folder name:", QLineEdit::Normal, "new_folder", &ok
        );

        if (!ok || name.isEmpty())
            return;

        LOG_INFO("FileOps") << "Create folder requested:" << name << "in" << directory;

        auto op = std::make_unique<Fs::CreateFolderOperation>(directory, name, nullptr);
        runOperation(std::move(op), [this](bool success) {
            if (success && m_model)
                m_model->refresh();
        });
    }

} // namespace Dentry::App

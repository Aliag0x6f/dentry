/**
 * @file FileOperationController.cpp
 * @brief Implementation of FileOperationController.
 *
 * @author Hugo Fabresse
 */

#include "FileOperationController.h"

#include <QFileInfo>
#include <QInputDialog>

#include "../../fs/operations/CopyOperation.h"
#include "../../fs/operations/CreateFileOperation.h"
#include "../../fs/operations/CreateFolderOperation.h"
#include "../../fs/operations/DeleteOperation.h"
#include "../../fs/operations/MoveOperation.h"
#include "../../fs/operations/RenameOperation.h"
#include "../../log/Logger.h"

namespace dentry::app {

    void FileOperationController::runOperation(std::unique_ptr<fs::AFileOperation> operation,
                                               const std::function<void(bool)>     &onFinished) {
        if (!operation)
            return;

        fs::AFileOperation *op = operation.release();
        op->setParent(this);

        connect(op, &fs::AFileOperation::finished, this, [onFinished, op](bool success, const QString &) {
            onFinished(success);
            op->deleteLater();
        });

        connect(op, &fs::AFileOperation::finished, this, [](bool success, const QString &error) {
            if (!success)
                log::error("FileOps") << "Operation failed:" << error;
        });

        op->execute();
    }

    FileOperationController::FileOperationController(model::FileSystemModel *model,
                                                     QWidget               *dialogParent,
                                                     QObject               *parent)
        : QObject(parent)
        , m_model(model)
        , m_dialogParent(dialogParent) {
    }

    void FileOperationController::onCopyRequested(const QStringList &paths) {
        log::info("FileOps") << "Copy requested:" << paths.count() << "item(s)";
        m_clipboard.copy(paths);
    }

    void FileOperationController::onCutRequested(const QStringList &paths) {
        log::info("FileOps") << "Cut requested:" << paths.count() << "item(s)";
        m_clipboard.cut(paths);
    }

    void FileOperationController::onPasteRequested(const QString &destination) {
        if (m_clipboard.isEmpty()) {
            log::warn("FileOps") << "Paste requested but clipboard is empty";
            return;
        }

        log::info("FileOps") << "Paste requested into:" << destination
                              << "(" << (m_clipboard.isCut() ? "move" : "copy") << ")";

        std::unique_ptr<fs::AFileOperation> op;
        if (m_clipboard.isCut()) {
            op = std::make_unique<fs::MoveOperation>(m_clipboard.paths(), destination, nullptr);
        } else {
            op = std::make_unique<fs::CopyOperation>(m_clipboard.paths(), destination, nullptr);
        }

        runOperation(std::move(op), [this](bool success) {
            if (success && m_model)
                m_model->refresh();
            if (m_clipboard.isCut() || success)
                m_clipboard.clear();
        });
    }

    void FileOperationController::onDeleteRequested(const QStringList &paths) {
        log::info("FileOps") << "Delete requested:" << paths.count() << "item(s)";

        auto op = std::make_unique<fs::DeleteOperation>(paths, nullptr);
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

        log::info("FileOps") << "Rename requested:" << oldName << "->" << newName;

        auto op = std::make_unique<fs::RenameOperation>(path, newName, nullptr);
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

        log::info("FileOps") << "Create file requested:" << name << "in" << directory;

        auto op = std::make_unique<fs::CreateFileOperation>(directory, name, nullptr);
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

        log::info("FileOps") << "Create folder requested:" << name << "in" << directory;

        auto op = std::make_unique<fs::CreateFolderOperation>(directory, name, nullptr);
        runOperation(std::move(op), [this](bool success) {
            if (success && m_model)
                m_model->refresh();
        });
    }

} // namespace dentry::app

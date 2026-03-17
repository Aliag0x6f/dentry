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

        Fs::AFileOperation *op = m_clipboard.isCut()
            ? static_cast<Fs::AFileOperation *>(new Fs::MoveOperation(m_clipboard.paths(), destination, nullptr))
            : static_cast<Fs::AFileOperation *>(new Fs::CopyOperation(m_clipboard.paths(), destination, nullptr));

        auto *dialog = new Ui::ProgressDialog(op, m_dialogParent);

        op->setParent(dialog);
        dialog->setAttribute(Qt::WA_DeleteOnClose);

        connect(op, &Fs::AFileOperation::finished, this, [this](bool success, const QString &) {
            if (success) m_model->refresh();
            if (m_clipboard.isCut() || success) m_clipboard.clear();
        });
        op->execute();
        dialog->exec();
    }

    void FileOperationController::onDeleteRequested(const QStringList &paths) {
        LOG_INFO("FileOps") << "Delete requested:" << paths.count() << "item(s)";

        auto *op     = new Fs::DeleteOperation(paths, nullptr);
        auto *dialog = new Ui::ProgressDialog(op, m_dialogParent);

        op->setParent(dialog);
        dialog->setAttribute(Qt::WA_DeleteOnClose);

        connect(op, &Fs::AFileOperation::finished, this, [this](bool success, const QString &) {
            if (success) m_model->refresh();
        });
        op->execute();
        dialog->exec();
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

        auto *op     = new Fs::RenameOperation(path, newName, nullptr);
        auto *dialog = new Ui::ProgressDialog(op, m_dialogParent);

        op->setParent(dialog);
        dialog->setAttribute(Qt::WA_DeleteOnClose);

        connect(op, &Fs::AFileOperation::finished, this, [this](bool success, const QString &) {
            if (success) m_model->refresh();
        });
        op->execute();
        dialog->exec();
    }

    void FileOperationController::onCreateFileRequested(const QString &directory) {
        bool ok;
        const QString name = QInputDialog::getText(
            m_dialogParent, "New File", "File name:", QLineEdit::Normal, "new_file.txt", &ok
        );

        if (!ok || name.isEmpty())
            return;

        LOG_INFO("FileOps") << "Create file requested:" << name << "in" << directory;

        auto *op     = new Fs::CreateFileOperation(directory, name, nullptr);
        auto *dialog = new Ui::ProgressDialog(op, m_dialogParent);

        op->setParent(dialog);
        dialog->setAttribute(Qt::WA_DeleteOnClose);

        connect(op, &Fs::AFileOperation::finished, this, [this](bool success, const QString &) {
            if (success) m_model->refresh();
        });
        op->execute();
        dialog->exec();
    }

    void FileOperationController::onCreateFolderRequested(const QString &directory) {
        bool ok;
        const QString name = QInputDialog::getText(
            m_dialogParent, "New Folder", "Folder name:", QLineEdit::Normal, "new_folder", &ok
        );

        if (!ok || name.isEmpty())
            return;

        LOG_INFO("FileOps") << "Create folder requested:" << name << "in" << directory;

        auto *op     = new Fs::CreateFolderOperation(directory, name, nullptr);
        auto *dialog = new Ui::ProgressDialog(op, m_dialogParent);

        op->setParent(dialog);
        dialog->setAttribute(Qt::WA_DeleteOnClose);

        connect(op, &Fs::AFileOperation::finished, this, [this](bool success, const QString &) {
            if (success) m_model->refresh();
        });
        op->execute();
        dialog->exec();
    }

} // namespace Dentry::App


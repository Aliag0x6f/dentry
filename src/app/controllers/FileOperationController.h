/**
 * @file FileOperationController.h
 * @brief Controller for file operations (copy, cut, paste, delete, rename, create).
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "app/clipboard/Clipboard.h"
#include "fs/AFileOperation.h"
#include "model/filesystem/FileSystemModel.h"

#include <QObject>
#include <QPointer>
#include <QString>
#include <QStringList>
#include <QWidget>
#include <functional>
#include <memory>

namespace dentry::app {

class FileOperationController : public QObject {
    Q_OBJECT

public:
    explicit FileOperationController(model::FileSystemModel *model,
                                     QWidget               *dialogParent,
                                     QObject               *parent = nullptr);

    ~FileOperationController() override = default;

    FileOperationController(const FileOperationController &)            = delete;
    FileOperationController &operator=(const FileOperationController &) = delete;
    FileOperationController(FileOperationController &&)                 = delete;
    FileOperationController &operator=(FileOperationController &&)      = delete;

    /**
     * @brief Returns the clipboard owned by this controller.
     *
     * Safe non-owning access. The clipboard is a Qt child of this controller
     * and will never outlive it.
     */
    [[nodiscard]] Clipboard *clipboard() const { return m_clipboard; }

public slots:
    void onCopyRequested(const QStringList &paths);
    void onCutRequested(const QStringList &paths);
    void onPasteRequested(const QString &destination);
    void onDeleteRequested(const QStringList &paths);
    void onRenameRequested(const QString &path);
    void onCreateFileRequested(const QString &directory);
    void onCreateFolderRequested(const QString &directory);

private:
    void runOperation(std::unique_ptr<fs::AFileOperation> operation,
                      const std::function<void(bool)>     &onFinished);

    QPointer<model::FileSystemModel> m_model;
    QPointer<QWidget>                m_dialogParent;
    QPointer<Clipboard>              m_clipboard;
};

} // namespace dentry::app

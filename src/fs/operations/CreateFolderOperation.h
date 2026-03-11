/**
 * @file CreateFolderOperation.h
 * @brief Concrete implementation of a folder creation operation.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "../AFileOperation.h"
#include <QFuture>
#include <QString>

namespace Dentry::Fs {

/**
 * @class CreateFolderOperation
 * @brief Creates a new directory asynchronously.
 *
 * Emits progress() during the operation and finished() on completion or error.
 * Supports cancellation via cancel().
 *
 * Example:
 * @code
 * auto *op = new CreateFolderOperation(directory, name, parent);
 * connect(op, &CreateFolderOperation::finished, this, &MainWindow::onOperationFinished);
 * op->execute();
 * @endcode
 */
class CreateFolderOperation final : public AFileOperation {
    Q_OBJECT

public:
    /**
     * @brief Constructs a CreateFolderOperation.
     *
     * @param directory Absolute path of the directory where the folder will be created.
     * @param name      Name of the folder to create.
     * @param parent    Optional Qt parent object.
     */
    explicit CreateFolderOperation(const QString &directory,
                                   const QString &name,
                                   QObject       *parent = nullptr);

    ~CreateFolderOperation() override = default;

    CreateFolderOperation(const CreateFolderOperation &)            = delete;
    CreateFolderOperation &operator=(const CreateFolderOperation &) = delete;
    CreateFolderOperation(CreateFolderOperation &&)                 = delete;
    CreateFolderOperation &operator=(CreateFolderOperation &&)      = delete;

    /**
     * @brief Starts the folder creation asynchronously.
     *
     * Returns immediately. Progress is reported via signals.
     */
    void execute() override;

    /**
     * @brief Returns a human-readable description of this operation.
     * @return A string such as "Creating folder projects".
     */
    [[nodiscard]] QString description() const override;

private:
    QString       m_directory;
    QString       m_name;
    QFuture<void> m_future;
};

} // namespace Dentry::Fs

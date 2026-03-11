/**
 * @file CreateFileOperation.h
 * @brief Concrete implementation of a file creation operation.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "../AFileOperation.h"
#include <QFuture>
#include <QString>

namespace Dentry::Fs {

/**
 * @class CreateFileOperation
 * @brief Creates a new empty file asynchronously.
 *
 * Emits progress() during the operation and finished() on completion or error.
 * Supports cancellation via cancel().
 *
 * Example:
 * @code
 * auto *op = new CreateFileOperation(directory, name, parent);
 * connect(op, &CreateFileOperation::finished, this, &MainWindow::onOperationFinished);
 * op->execute();
 * @endcode
 */
class CreateFileOperation final : public AFileOperation {
    Q_OBJECT

public:
    /**
     * @brief Constructs a CreateFileOperation.
     *
     * @param directory Absolute path of the directory where the file will be created.
     * @param name      Name of the file to create.
     * @param parent    Optional Qt parent object.
     */
    explicit CreateFileOperation(const QString &directory,
                                 const QString &name,
                                 QObject       *parent = nullptr);

    ~CreateFileOperation() override = default;

    CreateFileOperation(const CreateFileOperation &)            = delete;
    CreateFileOperation &operator=(const CreateFileOperation &) = delete;
    CreateFileOperation(CreateFileOperation &&)                 = delete;
    CreateFileOperation &operator=(CreateFileOperation &&)      = delete;

    /**
     * @brief Starts the file creation asynchronously.
     *
     * Returns immediately. Progress is reported via signals.
     */
    void execute() override;

    /**
     * @brief Returns a human-readable description of this operation.
     * @return A string such as "Creating file notes.txt".
     */
    [[nodiscard]] QString description() const override;

private:
    QString       m_directory;
    QString       m_name;
    QFuture<void> m_future;
};

} // namespace Dentry::Fs

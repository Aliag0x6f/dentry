/**
 * @file CreateFolderOperation.cpp
 * @brief Implementation of CreateFolderOperation.
 *
 * @author Hugo Fabresse
 */

#include "CreateFolderOperation.h"
#include <QDir>
#include <QtConcurrent>

namespace Dentry::Fs {

    CreateFolderOperation::CreateFolderOperation(const QString &directory,
                                                 const QString &name,
                                                 QObject       *parent)
        : AFileOperation(parent)
        , m_directory(directory)
        , m_name(name) {}

    void CreateFolderOperation::execute() {
        setRunning(true);

        m_future = QtConcurrent::run([this] {
            if (isCancelled()) {
                emit finished(false, "Operation cancelled");
                setRunning(false);
                return;
            }

            QDir dir(m_directory);

            if (!dir.mkdir(m_name)) {
                emit finished(false, QString("Failed to create folder: %1").arg(m_name));
                setRunning(false);
                return;
            }

            emit progress(100);
            emit finished(true, QString());
            setRunning(false);
        });
    }

    QString CreateFolderOperation::description() const {
        return QString("Creating folder %1").arg(m_name);
    }

} // namespace Dentry::Fs

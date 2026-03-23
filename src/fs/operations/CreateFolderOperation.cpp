/**
 * @file CreateFolderOperation.cpp
 * @brief Implementation of CreateFolderOperation.
 *
 * @author Hugo Fabresse
 */

#include "CreateFolderOperation.h"
#include "../../log/Logger.h"

#include <QDir>
#include <QtConcurrent>

namespace dentry::fs {

    CreateFolderOperation::CreateFolderOperation(const QString &directory,
                                                 const QString &name,
                                                 QObject       *parent)
        : AFileOperation(parent)
        , m_directory(directory)
        , m_name(name) {}

    void CreateFolderOperation::execute() {
        setRunning(true);
        log::info("Op") << "Creating folder: " << m_name << "in" << m_directory;

        m_future = QtConcurrent::run([this] {
            if (isCancelled()) {
                log::info("Op") << "Create folder cancelled";
                setRunning(false);
                emit finished(false, "Operation cancelled");
                return;
            }

            QDir dir(m_directory);

            if (!dir.mkdir(m_name)) {
                log::error("Op") << "Failed to create directory: " << m_name;
                setRunning(false);
                emit finished(false, QString("Failed to create folder: %1").arg(m_name));
                return;
            }

            log::info("Op") << "Folder created successfully:" << QDir(m_directory).filePath(m_name);
            emit progress(100);
            setRunning(false);
            emit finished(true, QString());
        });
    }

    QString CreateFolderOperation::description() const {
        return QString("Creating folder %1").arg(m_name);
    }

} // namespace dentry::fs

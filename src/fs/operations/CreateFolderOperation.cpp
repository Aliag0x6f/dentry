/**
 * @file CreateFolderOperation.cpp
 * @brief Implementation of CreateFolderOperation.
 *
 * @author Hugo Fabresse
 */

#include "CreateFolderOperation.h"
#include "../../util/Logger.h"

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
        LOG_INFO("Op") << "Creating folder: " << m_name << "in" << m_directory;

        m_future = QtConcurrent::run([this] {
            if (isCancelled()) {
                LOG_INFO("Op") << "Create folder cancelled";
                emit finished(false, "Operation cancelled");
                setRunning(false);
                return;
            }

            QDir dir(m_directory);

            if (!dir.mkdir(m_name)) {
                LOG_ERROR("Op") << "Failed to create directory: " << m_name;
                emit finished(false, QString("Failed to create folder: %1").arg(m_name));
                setRunning(false);
                return;
            }

            LOG_INFO("Op") << "Folder created successfully:" << m_directory + "/" + m_name;
            emit progress(100);
            emit finished(true, QString());
            setRunning(false);
        });
    }

    QString CreateFolderOperation::description() const {
        return QString("Creating folder %1").arg(m_name);
    }

} // namespace Dentry::Fs

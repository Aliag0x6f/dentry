/**
 * @file CreateFileOperation.cpp
 * @brief Implementation of CreateFileOperation.
 *
 * @author Hugo Fabresse
 */

#include "CreateFileOperation.h"
#include "../../util/Logger.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QtConcurrent>

namespace Dentry::Fs {

    CreateFileOperation::CreateFileOperation(const QString &directory,
                                             const QString &name,
                                             QObject       *parent)
        : AFileOperation(parent)
        , m_directory(directory)
        , m_name(name) {}

    void CreateFileOperation::execute() {
        setRunning(true);
        LOG_INFO("Op") << "Creating file:" << m_name << "in" << m_directory;

        m_future = QtConcurrent::run([this] {
            if (isCancelled()) {
                LOG_INFO("Op") << "Create file cancelled";
                setRunning(false);
                emit finished(false, "Operation cancelled");
                return;
            }

            const QString safeName = QFileInfo(m_name).fileName();
            if (safeName.isEmpty()) {
                LOG_ERROR("Op") << "Invalid file name:" << m_name;
                setRunning(false);
                emit finished(false, "Invalid file name");
                return;
            }

            const QString path = QDir(m_directory).filePath(m_name);

            QFile file(path);

            if (!file.open(QIODevice::WriteOnly)) {
                LOG_ERROR("Op") << "Failed to create file:" << m_name;
                setRunning(false);
                emit finished(false, QString("Failed to create: %1").arg(m_name));
                return;
            }

            file.close();
            LOG_INFO("Op") << "File created successfully:" << path;
            emit progress(100);
            setRunning(false);
            emit finished(true, QString());
        });
    }

    QString CreateFileOperation::description() const {
        return QString("Creating file %1").arg(m_name);
    }

} // namespace Dentry::Fs

/**
 * @file RenameOperation.cpp
 * @brief Implementation of RenameOperation.
 *
 * @author Hugo Fabresse
 */

#include "RenameOperation.h"
#include "../../util/Logger.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QtConcurrent>

namespace Dentry::Fs {

    RenameOperation::RenameOperation(const QString &source,
                                     const QString &newName,
                                     QObject       *parent)
        : AFileOperation(parent)
        , m_source(source)
        , m_newName(newName) {}

    void RenameOperation::execute() {
        setRunning(true);
        LOG_INFO("Op") << "Renaming:" << QFileInfo(m_source).fileName() << "->" << m_newName;

        m_future = QtConcurrent::run([this] {
            if (isCancelled()) {
                LOG_INFO("Op") << "Rename cancelled";
                setRunning(false);
                emit finished(false, "Operation cancelled");
                return;
            }

            if (!renameEntry(m_source)) {
                LOG_ERROR("Op") << "Failed to rename:" << QFileInfo(m_source).fileName();
                setRunning(false);
                emit finished(false, QString("Failed to rename: %1").arg(m_source));
                return;
            }

            LOG_INFO("Op") << "Rename completed successfully";
            emit progress(100);
            setRunning(false);
            emit finished(true, QString());
        });
    }

    bool RenameOperation::renameEntry(QStringView path) {
        const QFileInfo info(path.toString());
        const QFileInfo newNameInfo(m_newName);

        const QString sanitizedNewName = newNameInfo.fileName();
        if (sanitizedNewName.isEmpty()) {
            LOG_ERROR("Op") << "Invalid new name for rename operation:" << m_newName;
            return false;
        }
        const QString destination = QDir(info.dir().absolutePath()).filePath(sanitizedNewName);

        LOG_DEBUG("Op") << "Renaming:" << path << "->" << destination;
        return QFile::rename(path.toString(), destination);
    }

    QString RenameOperation::description() const {
        return QString("Renaming %1 to %2")
            .arg(m_source)
            .arg(m_newName);
    }

} // namespace Dentry::Fs

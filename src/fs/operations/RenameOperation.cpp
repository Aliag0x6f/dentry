/**
* @file RenameOperation.cpp
 * @brief Implementation of RenameOperation.
 *
 * @author Hugo Fabresse
 */

#include "RenameOperation.h"
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

        m_future = QtConcurrent::run([this] {
            if (isCancelled()) {
                emit finished(false, "Operation cancelled");
                setRunning(false);
                return;
            }

            if (!renameEntry(m_source)) {
                emit finished(false, QString("Failed to rename: %1").arg(m_source));
                setRunning(false);
                return;
            }

            emit progress(100);
            emit finished(true, QString());
            setRunning(false);
        });
    }

    bool RenameOperation::renameEntry(const QString &path) {
        const QFileInfo info(path);
        const QString destination = info.dir().absolutePath() + "/" + m_newName;

        return QFile::rename(path, destination);
    }

    QString RenameOperation::description() const {
        return QString("Renaming %1 to %2")
            .arg(m_source)
            .arg(m_newName);
    }

} // namespace Dentry::Fs

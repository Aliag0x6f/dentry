/**
 * @file DeleteOperation.cpp
 * @brief Implementation of DeleteOperation.
 *
 * @author Hugo Fabresse
 */

#include "DeleteOperation.h"
#include "log/Logger.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QtConcurrent>

namespace dentry::fs {

    DeleteOperation::DeleteOperation(const QStringList &targets,
                                     QObject           *parent)
        : AFileOperation(parent)
        , m_targets(targets) {}

    void DeleteOperation::execute() {
        setRunning(true);
        log::info("Op") << "Deleting " << m_targets.count() << " item(s)";

        m_future = QtConcurrent::run([this] {
            const int total = m_targets.count();
            int completed = 0;

            for (const QString &target : m_targets) {
                if (isCancelled()) {
                    log::info("Op") << "Delete cancelled";
                    setRunning(false);
                    emit finished(false, "Operation cancelled");
                    return;
                }

                if (!deleteEntry(target)) {
                    log::error("Op") << "Failed to delete " << target;
                    setRunning(false);
                    emit finished(false, QString("Failed to delete: %1").arg(target));
                    return;
                }

                ++completed;
                log::debug("Op") << "Deleted " << target
                                 << "(" << completed << "/" << total << ")";
                emit progress(static_cast<int>(completed * 100.0 / total));
            }

            log::info("Op") << "Delete completed successfully";
            setRunning(false);
            emit finished(true, QString());
        });
    }

    bool DeleteOperation::deleteEntry(const QString &path) {
        const QFileInfo info(path);

        if (info.isDir()) {
            log::debug("Op") << "Deleting directory:" << path;
            return QDir(path).removeRecursively();
        }

        log::debug("Op") << "Deleting file:" << path;
        return QFile::remove(path);
    }

    QString DeleteOperation::description() const {
        return QString("Deleting %1 item(s)").arg(m_targets.count());
    }

} // namespace dentry::fs

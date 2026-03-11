/**
* @file DeleteOperation.cpp
 * @brief Implementation of DeleteOperation.
 *
 * @author Hugo Fabresse
 */

#include "DeleteOperation.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QtConcurrent>

namespace Dentry::Fs {

    DeleteOperation::DeleteOperation(const QStringList &targets,
                                     QObject           *parent)
        : AFileOperation(parent)
        , m_targets(targets) {}

    void DeleteOperation::execute() {
        setRunning(true);

        m_future = QtConcurrent::run([this] {
            const int total = m_targets.count();
            int completed = 0;

            for (const QString &target : m_targets) {
                if (isCancelled()) {
                    emit finished(false, "Operation cancelled");
                    setRunning(false);
                    return;
                }

                if (!deleteEntry(target)) {
                    emit finished(false, QString("Failed to move: %1").arg(target));
                    setRunning(false);
                    return;
                }

                ++completed;
                emit progress(static_cast<int>(completed * 100.0 / total));
            }

            emit finished(true, QString());
            setRunning(false);
        });
    }

    bool DeleteOperation::deleteEntry(const QString &path) {
        const QFileInfo info(path);

        if (info.isDir()) {
            return QDir(path).removeRecursively();
        }
        QFile::remove(path);
        return false;
    }

    QString DeleteOperation::description() const {
        return QString("Deleting %1 item(s)").arg(m_targets.count());
    }

} // namespace Dentry::Fs

/**
 * @file MoveOperation.cpp
 * @brief Implementation of MoveOperation.
 *
 * @author Hugo Fabresse
 */

#include "MoveOperation.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QtConcurrent>

namespace Dentry::Fs {

    MoveOperation::MoveOperation(const QStringList &sources,
                                 const QString     &destination,
                                 QObject           *parent)
        : AFileOperation(parent)
        , m_sources(sources)
        , m_destination(destination) {}

    void MoveOperation::execute() {
        setRunning(true);

        m_future = QtConcurrent::run([this] {
            const int total = m_sources.count();
            int completed = 0;

            for (const QString &source : m_sources) {
                if (isCancelled()) {
                    emit finished(false, "Operation cancelled");
                    setRunning(false);
                    return;
                }

                const QFileInfo info(source);
                const QString dest = m_destination + "/" + info.fileName();

                if (!moveEntry(source, dest)) {
                    emit finished(false, QString("Failed to move: %1").arg(info.fileName()));
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

    bool MoveOperation::moveEntry(const QString &source, const QString &destination) {
        if (QFile::rename(source, destination))
            return true;

        return copyThenDelete(source, destination);
    }

    bool MoveOperation::copyThenDelete(const QString &source, const QString &destination) {
        const QFileInfo info(source);

        if (info.isDir()) {
            QDir srcDir(source);
            if (!QDir().mkpath(destination))
                return false;

            const QFileInfoList entries = srcDir.entryInfoList(
                QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden
            );

            for (const QFileInfo &entry : entries) {
                if (isCancelled())
                    return false;

                const QString destPath = destination + "/" + entry.fileName();

                if (!copyThenDelete(entry.absoluteFilePath(), destPath))
                    return false;
            }

            return QDir(source).removeRecursively();
        }

        if (QFile::exists(destination))
            QFile::remove(destination);

        if (!QFile::copy(source, destination))
            return false;

        return QFile::remove(source);
    }

    QString MoveOperation::description() const {
        return QString("Moving %1 item(s) to %2")
            .arg(m_sources.count())
            .arg(m_destination);
    }

} // namespace Dentry::Fs

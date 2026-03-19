/**
 * @file MoveOperation.cpp
 * @brief Implementation of MoveOperation.
 *
 * @author Hugo Fabresse
 */

#include "MoveOperation.h"
#include "../../util/Logger.h"

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
        LOG_INFO("Op") << "Moving" << m_sources.count() << "item(s) to" << m_destination;

        m_future = QtConcurrent::run([this] {
            const int total = m_sources.count();
            int completed = 0;

            for (const QString &source : m_sources) {
                if (isCancelled()) {
                    LOG_INFO("Op") << "Move cancelled";
                    setRunning(false);
                    emit finished(false, "Operation cancelled");
                    return;
                }

                const QFileInfo info(source);
                const QString dest = QDir(m_destination).filePath(info.fileName());

                if (!moveEntry(source, dest)) {
                    LOG_ERROR("Op") << "Failed to move:" << info.fileName();
                    setRunning(false);
                    emit finished(false, QString("Failed to move: %1").arg(info.fileName()));
                    return;
                }

                ++completed;
                LOG_DEBUG("Op") << "Moved:" << info.fileName()
                                << "(" << completed << "/" << total << ")";
                emit progress(static_cast<int>(completed * 100.0 / total));
            }

            LOG_INFO("Op") << "Move completed successfully";
            setRunning(false);
            emit finished(true, QString());
        });
    }

    bool MoveOperation::moveEntry(const QString &source, const QString &destination) {
        if (QFile::rename(source, destination)) {
            LOG_DEBUG("Op") << "Renamed directly:" << source;
            return true;
        }

        LOG_DEBUG("Op") << "Rename failed, falling back to copy+delete:" << source;
        return copyThenDelete(source, destination);
    }

    bool MoveOperation::copyThenDelete(const QString &source, const QString &destination) {
        const QFileInfo info(source);

        if (info.isDir()) {
            LOG_DEBUG("Op") << "Moving directory:" << source << "->" << destination;
            QDir srcDir(source);
            if (!QDir().mkpath(destination)) {
                LOG_ERROR("Op") << "Failed to create directory:" << destination;
                return false;
            }

            const QFileInfoList entries = srcDir.entryInfoList(
                QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden
            );

            for (const QFileInfo &entry : entries) {
                if (isCancelled())
                    return false;

                const QString destPath = QDir(destination).filePath(entry.fileName());

                if (!copyThenDelete(entry.absoluteFilePath(), destPath))
                    return false;
                }

                return QDir(source).removeRecursively();
            }

        if (QFile::exists(destination))
            QFile::remove(destination);

        if (!QFile::copy(source, destination)) {
            LOG_ERROR("Op") << "Failed to copy file:" << source;
            return false;
        }

        return QFile::remove(source);
    }

    QString MoveOperation::description() const {
        return QString("Moving %1 item(s) to %2")
            .arg(m_sources.count())
            .arg(m_destination);
    }

} // namespace Dentry::Fs

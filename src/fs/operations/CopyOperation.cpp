/**
 * @file CopyOperation.cpp
 * @brief Implementation of CopyOperation.
 *
 * @author Hugo Fabresse
 */

#include "CopyOperation.h"
#include "../../util/Logger.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QtConcurrent>

namespace Dentry::Fs {

    CopyOperation::CopyOperation(const QStringList &sources,
                                 const QString     &destination,
                                 QObject           *parent)
        : AFileOperation(parent)
        , m_sources(sources)
        , m_destination(destination) {}

    void CopyOperation::execute() {
        setRunning(true);
        LOG_INFO("Op") << "Copying" << m_sources.count() << "item(s) to" << m_destination;

        m_future = QtConcurrent::run([this] {
            const int total = m_sources.count();
            int completed = 0;

            for (const QString &source : m_sources) {
                if (isCancelled()) {
                    LOG_INFO("Op") << "Copy cancelled";
                    setRunning(false);
                    emit finished(false, "Operation cancelled");
                    return;
                }

                const QFileInfo info(source);
                const QString dest = QDir(m_destination).filePath(info.fileName());

                bool success = false;

                if (info.isDir()) {
                    success = copyDir(source, dest);
                } else {
                    if (QFile::exists(dest))
                        QFile::remove(dest);
                    success = QFile::copy(source, dest);
                }

                if (!success) {
                    LOG_ERROR("Op") << "Failed to copy:" << info.fileName();
                    setRunning(false);
                    emit finished(false, "Could not copy file " + dest);
                    return;
                }

                ++completed;
                LOG_DEBUG("Op") << "Copied:" << info.fileName()
                                << "(" << completed << "/" << total << ")";
                emit progress(static_cast<int>(completed * 100.0 / total));
            }

            LOG_INFO("Op") << "Copy completed successfully";
            setRunning(false);
            emit finished(true, QString());
        });
    }

    bool CopyOperation::copyDir(QStringView source, QStringView destination) {
        LOG_DEBUG("Op") << "Copying directory:" << source << "->" << destination;

        QDir srcDir(source.toString());
        QDir dstDir;

        if (!dstDir.mkpath(destination.toString())) {
            LOG_ERROR("Op") << "Failed to create directory:" << destination;
            return false;
        }

        const QFileInfoList entries = srcDir.entryInfoList(
            QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden
        );

        for (const QFileInfo &entry : entries) {
            if (isCancelled())
                return false;

            const QString destPath = QDir(destination.toString()).filePath(entry.fileName());

            if (entry.isDir()) {
                if (!copyDir(entry.absoluteFilePath(), destPath))
                    return false;
            } else {
                if (QFile::exists(destPath))
                    QFile::remove(destPath);
                if (!QFile::copy(entry.absoluteFilePath(), destPath)) {
                    LOG_ERROR("Op") << "Failed to copy file:" << entry.fileName();
                    return false;
                }
            }
        }

        return true;
    }

    QString CopyOperation::description() const {
        return QString("Copying %1 item(s) to %2")
            .arg(m_sources.count())
            .arg(m_destination);
    }

} // namespace Dentry::Fs

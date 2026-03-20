/**
 * @file CopyOperation.cpp
 * @brief Implementation of CopyOperation.
 *
 * @author Hugo Fabresse
 */

#include "CopyOperation.h"
#include "../../log/Logger.h"

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
        log::info("Op") << "Copying" << m_sources.count() << "item(s) to" << m_destination;

        m_future = QtConcurrent::run([this] {
            const int total = m_sources.count();
            int completed = 0;

            for (const QString &source : m_sources) {
                if (isCancelled()) {
                    log::info("Op") << "Copy cancelled";
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
                    log::error("Op") << "Failed to copy:" << info.fileName();
                    setRunning(false);
                    emit finished(false, "Could not copy file " + dest);
                    return;
                }

                ++completed;
                log::debug("Op") << "Copied:" << info.fileName()
                                 << "(" << completed << "/" << total << ")";
                emit progress(static_cast<int>(completed * 100.0 / total));
            }

            log::info("Op") << "Copy completed successfully";
            setRunning(false);
            emit finished(true, QString());
        });
    }

    bool CopyOperation::copyDir(const QString &source, const QString &destination) {
        log::debug("Op") << "Copying directory:" << source << "->" << destination;

        QDir srcDir(source);
        QDir dstDir;

        if (!dstDir.mkpath(destination)) {
            log::error("Op") << "Failed to create directory:" << destination;
            return false;
        }

        const QFileInfoList entries = srcDir.entryInfoList(
            QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden
        );

        for (const QFileInfo &entry : entries) {
            if (isCancelled())
                return false;

            const QString destPath = QDir(destination).filePath(entry.fileName());

            if (entry.isDir()) {
                if (!copyDir(entry.absoluteFilePath(), destPath))
                    return false;
            } else {
                if (QFile::exists(destPath))
                    QFile::remove(destPath);
                if (!QFile::copy(entry.absoluteFilePath(), destPath)) {
                    log::error("Op") << "Failed to copy file:" << entry.fileName();
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

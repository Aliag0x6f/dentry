/**
* @file CreateFileOperation.cpp
 * @brief Implementation of CreateFileOperation.
 *
 * @author Hugo Fabresse
 */

#include "CreateFileOperation.h"
#include <QFile>
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

        m_future = QtConcurrent::run([this] {
            if (isCancelled()) {
                emit finished(false, "Operation cancelled");
                setRunning(false);
                return;
            }

            const QString path = m_directory + "/" + m_name;

            QFile file(path);

            if (!file.open(QIODevice::WriteOnly)) {
                emit finished(false, QString("Failed to create: %1").arg(m_name));
                setRunning(false);
                return;
            }

            file.close();
            emit progress(100);
            emit finished(true, QString());
            setRunning(false);
        });
    }

    QString CreateFileOperation::description() const {
        return QString("Creating file %1").arg(m_name);
    }

} // namespace Dentry::Fs

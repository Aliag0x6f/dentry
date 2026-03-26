/**
 * @file MimeResolver.cpp
 * @brief Implementation of MimeResolver.
 *
 * @author Hugo Fabresse
 */

#include "MimeResolver.h"
#include "log/Logger.h"

#include <QFileInfo>
#include <QMimeDatabase>
#include <QMimeType>
#include <QString>

namespace dentry::fs {

	const QMimeDatabase &MimeResolver::database() {
		static QMimeDatabase db;
		return db;
	}

	QString MimeResolver::resolve(const QString &path) {
		return resolve(QFileInfo(path));
	}

	QString MimeResolver::resolve(const QFileInfo &info) {
		if (info.isDir())
			return QStringLiteral("inode/directory");

		const QMimeType mime = database().mimeTypeForFile(info, QMimeDatabase::MatchDefault);

		if (!mime.isValid()) {
			log::warn("Mime") << "Invalid MIME for:" << info.fileName() << "fallback to octet-stream";
			return QStringLiteral("application/octet-stream");
		}

		log::debug("Mime") << "Resolved:" << info.fileName() << "->" << mime.name();
		return mime.name();
	}

	QString MimeResolver::iconName(const QString &mimeType) {
		const QMimeType mime = database().mimeTypeForName(mimeType);

		if (!mime.isValid()) {
			log::warn("Mime") << "Invalid MIME type for icon lookup:" << mimeType;
			return QStringLiteral("text-x-generic");
		}

		return mime.genericIconName();
	}

} // namespace dentry::fs

/**
 * @file MimeResolver.cpp
 * @brief Implementation of MimeResolver.
 *
 * @author Hugo Fabresse
 */

#include "MimeResolver.h"
#include "../util/Logger.h"

#include <QMimeType>

namespace Dentry::Fs {

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
			LOG_WARNING("Mime") << "Invalid MIME for:" << info.fileName() << "fallback to octet-stream";
			return QStringLiteral("application/octet-stream");
		}

		LOG_DEBUG("Mime") << "Resolved:" << info.fileName() << "->" << mime.name();
		return mime.name();
	}

	QString MimeResolver::iconName(const QString &mimeType) {
		const QMimeType mime = database().mimeTypeForName(mimeType);

		if (!mime.isValid()) {
			LOG_WARNING("Mime") << "Invalid MIME type for icon lookup:" << mimeType;
			return QStringLiteral("text-x-generic");
		}

		return mime.genericIconName();
	}

} // namespace Dentry::Fs

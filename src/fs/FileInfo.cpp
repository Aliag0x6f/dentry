/**
 * @file FileInfo.cpp
 * @brief Implementation of FileInfo.
 *
 * @author Hugo Fabresse
 */

#include "FileInfo.h"
#include "MimeResolver.h"
#include "formatter/DateFormatter.h"
#include "formatter/PermissionFormatter.h"
#include "formatter/SizeFormatter.h"
#include "log/Logger.h"

namespace dentry::fs {

	FileInfo::FileInfo(const QString &path)
		: FileInfo(QFileInfo(path)) {}

	FileInfo::FileInfo(const QFileInfo &fileInfo)
		: m_fileInfo(fileInfo) { compute(); }

	void FileInfo::compute() {
		m_name				   = m_fileInfo.fileName();
		m_absolutePath		   = m_fileInfo.absoluteFilePath();
		m_size				   = m_fileInfo.isDir() ? 0 : m_fileInfo.size();
		m_lastModified		   = m_fileInfo.lastModified();
		m_mimeType			   = MimeResolver::resolve(m_fileInfo);
		m_formattedSize		   = m_fileInfo.isDir() ? QString() : formatter::formatSize(m_size);
		m_formattedDate		   = formatter::formatDateTime(m_lastModified);
		m_formattedPermissions = formatter::formatPermissions(m_fileInfo);

		log::debug("FileInfo") << "Computed:" << m_name << "MIME:" << m_mimeType;
	}

} // namespace dentry::fs

/**
 * @file FileInfo.cpp
 * @brief Implementation of FileInfo.
 *
 * @author Hugo Fabresse
 */

#include "FileInfo.h"
#include "MimeResolver.h"
#include "../util/DateFormatter.h"
#include "../util/PermissionFormatter.h"
#include "../util/SizeFormatter.h"
#include "../util/Logger.h"

namespace Dentry::Fs {

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
		m_formattedSize		   = m_fileInfo.isDir() ? QString() : Util::SizeFormatter::format(m_size);
		m_formattedDate		   = Util::DateFormatter::format(m_lastModified);
		m_formattedPermissions = Util::PermissionFormatter::format(m_fileInfo);

		LOG_DEBUG("FileInfo") << "Computed:" << m_name << "MIME:" << m_mimeType;
	}

} // namespace Dentry::Fs

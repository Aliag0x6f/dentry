/**
 * @file PermissionFormatter.cpp
 * @brief Implementation of PermissionFormatter.
 *
 * @author Hugo Fabresse
 */

#include "PermissionFormatter.h"

namespace Dentry::Util {

	QString PermissionFormatter::format(const QFileInfo &info) {
		QString result;

		if (!info.exists()) {
			return QString();
		}

		if (info.isDir())
			result += 'd';
		else if (info.isSymLink())
			result += 'l';
		else
			result += '-';

		result += formatRaw(info.permissions());

		return result;
	}


	QString PermissionFormatter::formatRaw(QFile::Permissions permissions) {
		QString result;

		// Owner
		result += (permissions & QFileDevice::ReadOwner)  ? 'r' : '-';
		result += (permissions & QFileDevice::WriteOwner) ? 'w' : '-';
		result += (permissions & QFileDevice::ExeOwner)   ? 'x' : '-';

		// Group
		result += (permissions & QFileDevice::ReadGroup)  ? 'r' : '-';
		result += (permissions & QFileDevice::WriteGroup) ? 'w' : '-';
		result += (permissions & QFileDevice::ExeGroup)   ? 'x' : '-';

		// Others
		result += (permissions & QFileDevice::ReadOther)  ? 'r' : '-';
		result += (permissions & QFileDevice::WriteOther) ? 'w' : '-';
		result += (permissions & QFileDevice::ExeOther)   ? 'x' : '-';

		return result;
	}

} // namespace Dentry::Util

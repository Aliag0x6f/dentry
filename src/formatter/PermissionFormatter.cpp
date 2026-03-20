/**
 * @file PermissionFormatter.cpp
 * @brief Implementation of permission formatting free functions.
 *
 * @author Hugo Fabresse
 */

#include "PermissionFormatter.h"

namespace Dentry::formatter {

	QString formatPermissions(const QFileInfo &info) {
		QString result;

		if (!info.exists()) {
			return QString();
		}

		if (info.isSymLink())
			result += 'l';
		else if (info.isDir())
			result += 'd';
		else
			result += '-';

		result += formatPermissionsRaw(info.permissions());

		return result;
	}


	QString formatPermissionsRaw(QFile::Permissions permissions) {
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

} // namespace Dentry::formatter

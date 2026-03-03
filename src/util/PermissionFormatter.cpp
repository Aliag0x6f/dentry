/**
* @file PermissionFormatter.cpp
 * @brief Implementation of PermissionFormatter.
 *
 * @author Hugo Fabresse
 */

#include "PermissionFormatter.h"

namespace Dentry::Util {

	QString PermissionFormatter::format(const QFileInfo &info) {
		// TODO: implement 10-character Unix permission string (e.g. "-rwxr-xr-x")
		Q_UNUSED(info)
		return QStringLiteral("----------");
	}

	QString PermissionFormatter::formatRaw(QFile::Permissions permissions) {
		// TODO: implement 9-character permission string without type prefix
		Q_UNUSED(permissions)
		return QStringLiteral("---------");
	}

} // namespace Dentry::Util

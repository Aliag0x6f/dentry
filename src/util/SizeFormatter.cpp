/**
* @file SizeFormatter.cpp
 * @brief Implementation of SizeFormatter.
 *
 * @author Hugo Fabresse
 */

#include "SizeFormatter.h"

namespace Dentry::Util {

	QString SizeFormatter::format(qint64 bytes) {
		// TODO: implement binary unit formatting (B, K, M, G, T)
		Q_UNUSED(bytes)
		return QStringLiteral("-- B");
	}

	QString SizeFormatter::formatLong(qint64 bytes) {
		// TODO: implement binary unit formatting with full unit name (MB, GB...)
		Q_UNUSED(bytes)
		return QStringLiteral("-- B");
	}

} // namespace Dentry::Util
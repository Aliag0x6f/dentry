/**
 * @file SizeFormatter.cpp
 * @brief Implementation of SizeFormatter.
 *
 * @author Hugo Fabresse
 */

#include "SizeFormatter.h"

namespace Dentry::Util {

	QString SizeFormatter::format(qint64 bytes) {
		return QLocale().formattedDataSize(
			bytes,
			2,
			QLocale::DataSizeTraditionalFormat
		);
	}

	QString SizeFormatter::formatLong(qint64 bytes) {
		return QLocale().formattedDataSize(
			bytes,
			2,
			QLocale::DataSizeIecFormat
		);
	}

} // namespace Dentry::Util

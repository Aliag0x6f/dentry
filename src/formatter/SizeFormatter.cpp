/**
 * @file SizeFormatter.cpp
 * @brief Implementation of size formatting free functions.
 *
 * @author Hugo Fabresse
 */

#include "SizeFormatter.h"

namespace dentry::formatter {

	QString formatSize(qint64 bytes) {
		return QLocale().formattedDataSize(bytes, 2, QLocale::DataSizeTraditionalFormat);
	}

	QString formatSizeLong(qint64 bytes) {
		return QLocale().formattedDataSize(bytes, 2, QLocale::DataSizeIecFormat);
	}

} // namespace dentry::formatter

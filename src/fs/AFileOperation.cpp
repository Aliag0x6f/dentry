/**
 * @file AFileOperation.cpp
 * @brief Implementation of AFileOperation.
 *
 * @author Hugo Fabresse
 */

#include "AFileOperation.h"

namespace dentry::fs {

	AFileOperation::AFileOperation(QObject *parent)
		: QObject(parent) {}

	void AFileOperation::cancel() {
		m_cancelled = true;
	}

	bool AFileOperation::isRunning() const {
		return m_running;
	}

	bool AFileOperation::isCancelled() const {
		return m_cancelled;
	}

	void AFileOperation::setRunning(bool running) {
		m_running = running;

		if (!running)
			m_cancelled = false;
	}

} // namespace dentry::fs

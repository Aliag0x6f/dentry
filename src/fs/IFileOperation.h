/**
 * @file IFileOperation.h
 * @brief Interface for all file operations.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QString>

namespace Dentry::Fs {

	/**
	 * @class IFileOperation
	 * @brief Pure interface for all file operations.
	 *
	 * Defines the contract that every file operation must fulfill.
	 * All operations are asynchronous and report progress and completion
	 * through signals defined in the concrete Qt implementation.
	 *
	 * @note This interface does not inherit QObject — signal/slot
	 *       infrastructure is added in AFileOperation.
	 */
	class IFileOperation {
	public:
		virtual ~IFileOperation() = default;

		IFileOperation(const IFileOperation &)            = delete;
		IFileOperation &operator=(const IFileOperation &) = delete;
		IFileOperation(IFileOperation &&)                 = delete;
		IFileOperation &operator=(IFileOperation &&)      = delete;

		/**
		 * @brief Starts the operation asynchronously.
		 *
		 * Must return immediately. Progress is reported via signals.
		 */
		virtual void execute() = 0;

		/**
		 * @brief Cancels the operation if it is running.
		 *
		 * Implementations must handle cancellation gracefully
		 * and leave the filesystem in a consistent state.
		 */
		virtual void cancel() = 0;

		/**
		 * @brief Returns whether the operation is currently running.
		 */
		[[nodiscard]] virtual bool isRunning() const = 0;

		/**
		 * @brief Returns a human-readable description of the operation.
		 *
		 * Used to display in the progress dialog.
		 * Example: "Copying document.pdf to /home/user/Downloads"
		 *
		 * @return A descriptive string.
		 */
		[[nodiscard]] virtual QString description() const = 0;

	protected:
		IFileOperation() = default;
	};

} // namespace Dentry::Fs

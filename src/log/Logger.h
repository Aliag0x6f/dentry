/**
 * @file Logger.h
 * @brief Application-wide logger for Dentry.
 *
 * Wraps Qt's message handler system with colored terminal output,
 * timestamps, and log levels. Installed once at startup via log::install().
 *
 * Usage:
 * @code
 * log::debug("FileInfo") << "Resolved MIME type:" << mimeType;
 * log::info("Model") << "Directory loaded:" << path;
 * log::warn("Fs") << "File not readable:" << path;
 * log::error("Operation") << "Copy failed:" << error;
 * @endcode
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QDebug>
#include <QLoggingCategory>
#include <QString>

namespace dentry::log {

/**
 * @brief Installs the custom message handler.
 *
 * Must be called once before any dentry::log::* function is used.
 * Typically called in Application::initialize().
 */
void install();

/**
 * @brief Uninstalls the custom message handler.
 *
 * Removes the logger-installed message handler. This does not restore
 * Qt's global logging filter rules; any filter changes made in
 * install() remain in effect.
 */
void uninstall();

/**
 * @brief Starts a debug-level log stream for the given category.
 */
QDebug debug(const char *category);

/**
 * @brief Starts an info-level log stream for the given category.
 */
QDebug info(const char *category);

/**
 * @brief Starts a warning-level log stream for the given category.
 */
QDebug warn(const char *category);

/**
 * @brief Starts an error-level log stream for the given category.
 */
QDebug error(const char *category);

} // namespace dentry::log

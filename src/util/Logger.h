/**
 * @file Logger.h
 * @brief Application-wide logger for Dentry.
 *
 * Wraps Qt's message handler system with colored terminal output,
 * timestamps, and log levels. Installed once at startup via Logger::install().
 *
 * Usage:
 * @code
 * LOG_DEBUG("FileInfo")   << "Resolved MIME type:" << mimeType;
 * LOG_INFO("Model")       << "Directory loaded:" << path;
 * LOG_WARNING("Fs")       << "File not readable:" << path;
 * LOG_ERROR("Operation")  << "Copy failed:" << error;
 * @endcode
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QDebug>
#include <QLoggingCategory>
#include <QString>

namespace Dentry::Util {

/**
 * @class Logger
 * @brief Installs and configures the application-wide message handler.
 *
 * Non-instantiable utility class. Call Logger::install() once in
 * Application::initialize() before any logging occurs.
 */
class Logger {
public:
    Logger()  = delete;
    ~Logger() = delete;

    Logger(const Logger &)            = delete;
    Logger &operator=(const Logger &) = delete;
    Logger(Logger &&)                 = delete;
    Logger &operator=(Logger &&)      = delete;

    /**
     * @brief Installs the custom message handler.
     *
     * Must be called once before any LOG_* macro is used.
     * Typically called in Application::initialize().
     */
    static void install();

    /**
     * @brief Uninstalls the custom message handler and restores Qt default.
     */
    static void uninstall();
};

} // namespace Dentry::Util

// ── Logging macros ────────────────────────────────────────────────────────────

/**
 * @brief Logs a debug message with a category tag.
 *
 * Only visible in debug builds or when QT_LOGGING_RULES enables the category.
 *
 * @param category A short string identifying the source (e.g. "Model", "Fs").
 *
 * Example:
 * @code
 * LOG_DEBUG("FileInfo") << "Resolved MIME:" << mime;
 * @endcode
 */
#define LOG_DEBUG(category)   qCDebug(   QLoggingCategory(category) )

/**
 * @brief Logs an informational message with a category tag.
 *
 * @param category A short string identifying the source.
 */
#define LOG_INFO(category)    qCInfo(    QLoggingCategory(category) )

/**
 * @brief Logs a warning message with a category tag.
 *
 * @param category A short string identifying the source.
 */
#define LOG_WARNING(category) qCWarning( QLoggingCategory(category) )

/**
 * @brief Logs an error message with a category tag.
 *
 * @param category A short string identifying the source.
 */
#define LOG_ERROR(category)   qCCritical(QLoggingCategory(category) )

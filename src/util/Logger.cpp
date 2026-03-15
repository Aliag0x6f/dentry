/**
 * @file Logger.cpp
 * @brief Implementation of Logger.
 *
 * @author Hugo Fabresse
 */

#include "Logger.h"

#include <QDateTime>
#include <QMessageLogContext>
#include <QString>

#include <cstdio>

namespace Dentry::Util {

// ── ANSI color codes ──────────────────────────────────────────────────────────

static constexpr const char *RESET   = "\033[0m";
static constexpr const char *DIM     = "\033[2m";
static constexpr const char *BLUE    = "\033[34m";
static constexpr const char *GREEN   = "\033[32m";
static constexpr const char *YELLOW  = "\033[33m";
static constexpr const char *RED     = "\033[31m";
static constexpr const char *BOLD_RED = "\033[1;31m";

// ── Message handler ───────────────────────────────────────────────────────────

static void messageHandler(QtMsgType type,
                            const QMessageLogContext &context,
                            const QString &message) {

    const QString timestamp = QDateTime::currentDateTime()
        .toString("HH:mm:ss.zzz");

    const char *levelColor = RESET;
    const char *levelLabel = "???";

    switch (type) {
        case QtDebugMsg:
            levelColor = BLUE;
            levelLabel = "DBG";
            break;
        case QtInfoMsg:
            levelColor = GREEN;
            levelLabel = "INF";
            break;
        case QtWarningMsg:
            levelColor = YELLOW;
            levelLabel = "WRN";
            break;
        case QtCriticalMsg:
            levelColor = RED;
            levelLabel = "ERR";
            break;
        case QtFatalMsg:
            levelColor = BOLD_RED;
            levelLabel = "FTL";
            break;
    }

    const QString category = context.category
        ? QString(context.category)
        : QStringLiteral("default");

    fprintf(stderr,
        "%s%s%s %s[%s]%s %s[%s]%s %s\n",
        DIM,    qPrintable(timestamp), RESET,
        levelColor, levelLabel,        RESET,
        DIM,    qPrintable(category),  RESET,
        qPrintable(message)
    );

    if (type == QtFatalMsg)
        abort();
}

// ── Logger ────────────────────────────────────────────────────────────────────

void Logger::install() {
    if (qgetenv("DENTRY_DEBUG").isEmpty())
        QLoggingCategory::setFilterRules("*.debug=false");

    qInstallMessageHandler(messageHandler);
}

void Logger::uninstall() {
    qInstallMessageHandler(nullptr);
}

} // namespace Dentry::Util

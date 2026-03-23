/**
 * @file Logger.cpp
 * @brief Implementation of Logger.
 *
 * @author Hugo Fabresse
 */

#include "Logger.h"

#include <QByteArray>
#include <QDateTime>
#include <QHash>
#include <QMessageLogContext>
#include <QMutex>
#include <QMutexLocker>
#include <QSharedPointer>
#include <QString>
#include <cstdio>
#include <cstdlib>

namespace {

QLoggingCategory &categoryFor(const char *categoryName) {
    static QMutex                                              mutex;
    static QHash<QString, QByteArray>                          categoryNames;
    static QHash<QString, QSharedPointer<QLoggingCategory>>   categories;

    const QString key = QString::fromUtf8(categoryName);
    QMutexLocker   lock(&mutex);

    auto it = categories.find(key);
    if (it == categories.end()) {
        auto nameIt = categoryNames.insert(key, key.toUtf8());
        it = categories.insert(key, QSharedPointer<QLoggingCategory>::create(nameIt.value().constData()));
    }

    return *it.value();
}

} // namespace

namespace dentry::log {

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

void install() {
    if (qgetenv("DENTRY_DEBUG").isEmpty())
        QLoggingCategory::setFilterRules("*.debug=false");

    qInstallMessageHandler(messageHandler);
}

void uninstall() {
    qInstallMessageHandler(nullptr);
}


QDebug debug(const char *category) {
    return QMessageLogger().debug(categoryFor(category));
}

QDebug info(const char *category) {
    return QMessageLogger().info(categoryFor(category));
}

QDebug warn(const char *category) {
    return QMessageLogger().warning(categoryFor(category));
}

QDebug error(const char *category) {
    return QMessageLogger().critical(categoryFor(category));
}

} // namespace dentry::log

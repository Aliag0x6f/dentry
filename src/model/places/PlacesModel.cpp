/**
* @file PlacesModel.cpp
 * @brief Implementation of PlacesModel.
 *
 * @author Hugo Fabresse
 */

#include "PlacesModel.h"
#include "log/Logger.h"

#include <QDir>
#include <QStandardItem>
#include <QStandardPaths>

namespace dentry::model {

    PlacesModel::PlacesModel(QObject *parent)
        : QStandardItemModel(parent) {
        populate();
    }

    void PlacesModel::refresh() {
        populate();
    }

    void PlacesModel::populate() {
        clear();

        const QString homePath =
            QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

        // ── Home ──────────────────────────────────────────────────────────
        auto *homeItem = new QStandardItem("Home");
        homeItem->setData(homePath, Qt::UserRole);
        homeItem->setEditable(false);
        appendRow(homeItem);

        // ── Home subdirectories ───────────────────────────────────────────
        QDir::Filters filters = QDir::Dirs | QDir::NoDotAndDotDot;
        /*if (m_showHidden)
            filters |= QDir::Hidden;*/

        const QFileInfoList entries =
            QDir(homePath).entryInfoList(filters, QDir::Name);

        for (const QFileInfo &entry : entries) {
            auto *item = new QStandardItem(entry.fileName());
            item->setData(entry.absoluteFilePath(), Qt::UserRole);
            item->setEditable(false);
            appendRow(item);
        }

        // ── Root ──────────────────────────────────────────────────────────
        auto *rootItem = new QStandardItem("Root");
        rootItem->setData(QStringLiteral("/"), Qt::UserRole);
        rootItem->setEditable(false);
        appendRow(rootItem);

        log::debug("Model") << "PlacesModel populated:" << rowCount() << "entries";
    }

    QModelIndex PlacesModel::defaultIndex() const {
        return rowCount() > 0 ? index(0, 0) : QModelIndex();
    }

} // namespace dentry::model

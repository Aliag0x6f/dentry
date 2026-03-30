/**
* @file PlacesModel.cpp
 * @brief Implementation of PlacesModel.
 *
 * @author Hugo Fabresse
 */

#include "PlacesModel.h"
#include "log/Logger.h"

#include <QApplication>
#include <QDir>
#include <QStandardItem>
#include <QStandardPaths>
#include <QWidget>

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

    void PlacesModel::manageSelectionFocus(QItemSelectionModel* selectionModel, QWidget* view) {
        connect(qApp, &QApplication::focusChanged, this, [selectionModel, view, this](QWidget*, QWidget* now) {
            if (!view->isAncestorOf(now)) {
                selectionModel->clearSelection();
            } else if (now == view) {
                if (selectionModel->selectedIndexes().isEmpty() && rowCount() > 0) {
                    QModelIndex first = index(0, 0);
                    selectionModel->select(first, QItemSelectionModel::Select | QItemSelectionModel::Rows);
                    selectionModel->setCurrentIndex(first, QItemSelectionModel::NoUpdate);
                }
            }
        });
    }

} // namespace dentry::model

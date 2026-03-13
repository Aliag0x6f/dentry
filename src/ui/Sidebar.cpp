/**
 * @file Sidebar.cpp
 * @brief Implementation of Sidebar.
 *
 * @author Hugo Fabresse
 */

#include "Sidebar.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QStandardPaths>
#include <QDir>

namespace Dentry::Ui {

    Sidebar::Sidebar(QWidget *parent)
        : QWidget(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        QLabel *title = new QLabel("PLACES", this);
        title->setObjectName("sidebarTitle");
        layout->addWidget(title);

        m_list = new QListWidget(this);
        m_list->setMinimumWidth(160);
        m_list->setMaximumWidth(220);
        layout->addWidget(m_list);

        buildPlaces();

        connect(m_list, &QListWidget::itemClicked, this, &Sidebar::onItemClicked);

    }

    void Sidebar::buildPlaces() {
        const QString homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        auto *homeItem = new QListWidgetItem("Home", m_list);
        homeItem->setData(Qt::UserRole, homePath);

        QDir homeDir(homePath);
        const QFileInfoList entries = homeDir.entryInfoList(
            QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden,
            QDir::Name
        );

        for (const QFileInfo &entry : entries) {
            auto *item = new QListWidgetItem(entry.fileName(), m_list);
            item->setData(Qt::UserRole, entry.absoluteFilePath());
        }

        auto *rootItem = new QListWidgetItem("Root", m_list);
        rootItem->setData(Qt::UserRole, QStringLiteral("/"));
    }

    void Sidebar::onItemClicked(QListWidgetItem *item) {
        const QString path = item->data(Qt::UserRole).toString();

        if (!path.isEmpty())
            emit placeSelected(path);
    }

} // namespace Dentry::Ui

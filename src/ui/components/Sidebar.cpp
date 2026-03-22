/**
 * @file Sidebar.cpp
 * @brief Implementation of Sidebar.
 *
 * @author Hugo Fabresse
 */

#include "Sidebar.h"

#include <QDir>
#include <QLabel>
#include <QStandardPaths>

namespace dentry::ui {

    Sidebar::Sidebar(QWidget *parent)
        : UIComponent(parent) {
        build();
    }

    void Sidebar::setupLayout(VLayout &layout) {
        layout.setContentsMargins(0, 0, 0, 0);
        layout.setSpacing(0);

        QLabel *title = new QLabel("PLACES", this);
        title->setObjectName("sidebarTitle");
        layout.addWidget(title);

        QFrame *separator = new QFrame(this);
        separator->setFrameShape(QFrame::HLine);
        separator->setObjectName("sidebarSeparator");
        layout.addWidget(separator);

        m_list = new QListWidget(this);
        layout.addWidget(m_list);

        buildPlaces();
    }

    void Sidebar::setupSize() {
        m_list->setMinimumWidth(160);
        m_list->setMaximumWidth(220);
    }

    void Sidebar::setupStyle() {
        setObjectName("sidebar");
    }

    void Sidebar::setupConnections() {
        connect(m_list, &QListWidget::itemClicked, this, &Sidebar::onItemClicked);
    }

    void Sidebar::buildPlaces() {
        const QString homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

        auto *homeItem = new QListWidgetItem("Home", m_list);
        homeItem->setData(Qt::UserRole, homePath);

        QDir homeDir(homePath);
        QDir::Filters filters = QDir::Dirs | QDir::NoDotAndDotDot;
        if (m_showHidden)
            filters |= QDir::Hidden;

        const QFileInfoList entries = homeDir.entryInfoList(filters, QDir::Name);
        for (const QFileInfo &entry : entries) {
            auto *item = new QListWidgetItem(entry.fileName(), m_list);
            item->setData(Qt::UserRole, entry.absoluteFilePath());
        }

        auto *rootItem = new QListWidgetItem("Root", m_list);
        rootItem->setData(Qt::UserRole, QStringLiteral("/"));
    }

    void Sidebar::setShowHidden(bool show) {
        m_showHidden = show;
        m_list->clear();
        buildPlaces();
    }

    void Sidebar::onItemClicked(QListWidgetItem *item) {
        const QString path = item->data(Qt::UserRole).toString();
        if (!path.isEmpty())
            emit placeSelected(path);
    }

} // namespace dentry::ui

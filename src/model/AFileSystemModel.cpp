/**
 * @file AFileSystemModel.cpp
 * @brief Implementation of AFileSystemModel.
 *
 * @author Hugo Fabresse
 */

#include "AFileSystemModel.h"
#include "../log/Logger.h"

namespace dentry::model {

    AFileSystemModel::AFileSystemModel(QObject *parent)
        : QAbstractItemModel(parent) {}

    void AFileSystemModel::setShowHidden(bool show) {
        log::debug("Model") << "ShowHidden changed to:" << show;
        m_showHidden = show;
        refresh();
    }

    void AFileSystemModel::setFilter(const QString &filter) {
        log::debug("Model") << "Filter changed to:" << filter;
        m_filter = filter;
        refresh();
    }

    QModelIndex AFileSystemModel::index(int row, int column, const QModelIndex &parent) const {
        if (parent.isValid())
            return {};

        if (row < 0 || row >= m_entries.count())
            return {};

        if (column < 0 || column >= Column::ColumnCount)
            return {};

        return createIndex(row, column);
    }

    QModelIndex AFileSystemModel::parent(const QModelIndex &child) const {
        Q_UNUSED(child)
        return {};
    }

    int AFileSystemModel::rowCount(const QModelIndex &parent) const {
        if (parent.isValid())
            return 0;

        return m_entries.count();
    }

    int AFileSystemModel::columnCount(const QModelIndex &parent) const {
        if (parent.isValid())
            return 0;

        return Column::ColumnCount;
    }

    QVariant AFileSystemModel::data(const QModelIndex &index, int role) const {
        if (!index.isValid())
            return {};

        if (index.row() < 0 || index.row() >= m_entries.count())
            return {};

        const FileItem &item = m_entries.at(index.row());

        if (role == Qt::DisplayRole) {
            switch (index.column()) {
                case Column::Name:         return item.name;
                case Column::Size:         return item.formattedSize;
                case Column::Type:         return item.mimeType;
                case Column::DateModified: return item.formattedDate;
                case Column::Permissions:  return item.formattedPermissions;
                default:                   return {};
            }
        }

        if (role == Qt::TextAlignmentRole && index.column() == Column::Size)
            return QVariant(Qt::AlignRight | Qt::AlignVCenter);

        return {};
    }

    QVariant AFileSystemModel::headerData(int section, Qt::Orientation orientation, int role) const {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
            return {};

        switch (section) {
            case Column::Name:         return "Name";
            case Column::Size:         return "Size";
            case Column::Type:         return "Type";
            case Column::DateModified: return "Date Modified";
            case Column::Permissions:  return "Permissions";
            default:                   return {};
        }
    }

} // namespace dentry::model

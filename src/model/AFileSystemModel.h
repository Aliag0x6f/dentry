/**
 * @file AFileSystemModel.h
 * @brief Abstract base class for filesystem models.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "IFileSystemModel.h"
#include <QAbstractItemModel>
#include <QList>
#include <QString>

namespace Dentry::Model {

/**
 * @class AFileSystemModel
 * @brief Abstract Qt-based implementation of IFileSystemModel.
 *
 * Adds Qt signal/slot infrastructure and QAbstractItemModel integration
 * on top of IFileSystemModel. Manages shared state (current path, filter,
 * hidden flag) and defines signals used by all concrete implementations.
 *
 * Concrete models inherit from this class and implement:
 * - setDirectory() — load and populate entries for a given path
 * - refresh()      — reload the current directory
 *
 * @note QAbstractItemModel must come first in the inheritance list.
 */
class AFileSystemModel : public QAbstractItemModel, public IFileSystemModel {
    Q_OBJECT

public:
    explicit AFileSystemModel(QObject *parent = nullptr);

    ~AFileSystemModel() override = default;

    AFileSystemModel(const AFileSystemModel &)            = delete;
    AFileSystemModel &operator=(const AFileSystemModel &) = delete;
    AFileSystemModel(AFileSystemModel &&)                 = delete;
    AFileSystemModel &operator=(AFileSystemModel &&)      = delete;

    // ── IFileSystemModel ──────────────────────────────────────────────────

    [[nodiscard]] QString         currentPath() const override { return m_currentPath; }
    [[nodiscard]] bool            showHidden()  const override { return m_showHidden; }
    [[nodiscard]] QString         filter()      const override { return m_filter; }
    [[nodiscard]] QList<FileItem> entries()     const override { return m_entries; }

    void setShowHidden(bool show) override;
    void setFilter(const QString &filter) override;

    // ── QAbstractItemModel ────────────────────────────────────────────────

    [[nodiscard]] QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]] QModelIndex parent(const QModelIndex &child) const override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

signals:
    /**
     * @brief Emitted when the directory has been loaded.
     * @param path The path that was loaded.
     */
    void directoryLoaded(const QString &path);

    /**
     * @brief Emitted when an error occurs during directory loading.
     * @param error Human-readable error message.
     */
    void errorOccurred(const QString &error);

protected:
    /** @brief List of entries in the current directory. */
    QList<FileItem> m_entries;

    /** @brief Absolute path of the currently displayed directory. */
    QString m_currentPath;

    /** @brief Whether hidden dot-files are shown. */
    bool m_showHidden = false;

    /** @brief Current search filter string. */
    QString m_filter;

    /**
     * @brief Column definitions used by headerData() and data().
     */
    enum Column {
        Name         = 0,
        Size         = 1,
        Type         = 2,
        DateModified = 3,
        Permissions  = 4,
        ColumnCount  = 5
    };
};

} // namespace Dentry::Model

/**
 * @file FileSystemModel.h
 * @brief Model for local directory browsing.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "FileItem.h"

#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QList>
#include <QString>

namespace dentry::model {

/**
 * @class FileSystemModel
 * @brief Reads and exposes the contents of a local directory.
 *
 * Uses QDir and dentry::fs::FileInfo to populate entries.
 * Supports hidden file filtering, search filtering, and sorting
 * with directories always listed before files.
 *
 * Example:
 * @code
 * auto *model = new FileSystemModel(this);
 * view->setModel(model);
 * model->setDirectory(QDir::homePath());
 * @endcode
 */
class FileSystemModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit FileSystemModel(QObject *parent = nullptr);

    ~FileSystemModel() override = default;

    FileSystemModel(const FileSystemModel &)            = delete;
    FileSystemModel &operator=(const FileSystemModel &) = delete;
    FileSystemModel(FileSystemModel &&)                 = delete;
    FileSystemModel &operator=(FileSystemModel &&)      = delete;

    // ── Domain API ────────────────────────────────────────────────────

    /**
     * @brief Loads the contents of the given directory.
     *
     * Emits directoryLoaded() on success, errorOccurred() on failure.
     */
    void setDirectory(const QString &path);

    /** @brief Reloads the current directory. */
    void refresh();

    void setShowHidden(bool show);
    void setFilter(const QString &filter);

    [[nodiscard]] QString         currentPath() const { return m_currentPath; }
    [[nodiscard]] bool            showHidden()  const { return m_showHidden; }
    [[nodiscard]] QString         filter()      const { return m_filter; }
    [[nodiscard]] QList<FileItem> entries()     const { return m_entries; }

    // ── QAbstractItemModel ────────────────────────────────────────────

    [[nodiscard]] QModelIndex index(int row, int column, const QModelIndex &parent = {}) const override;
    [[nodiscard]] QModelIndex parent(const QModelIndex &child) const override;
    [[nodiscard]] int         rowCount(const QModelIndex &parent = {}) const override;
    [[nodiscard]] int         columnCount(const QModelIndex &parent = {}) const override;
    [[nodiscard]] QVariant    data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] QVariant    headerData(int section, Qt::Orientation, int role = Qt::DisplayRole) const override;

    void manageSelectionFocus(QItemSelectionModel* selectionModel, QWidget* view);

signals:
    void directoryLoaded(const QString &path);
    void errorOccurred(const QString &error);

private:
    void populate(const QString &path);

    enum Column {
        Name         = 0,
        Size         = 1,
        Type         = 2,
        DateModified = 3,
        Permissions  = 4,
        ColumnCount  = 5
    };

    QList<FileItem> m_entries;
    QString         m_currentPath;
    bool            m_showHidden = false;
    QString         m_filter;
};

} // namespace dentry::model

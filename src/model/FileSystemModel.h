/**
 * @file FileSystemModel.h
 * @brief Concrete filesystem model for local directory browsing.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "AFileSystemModel.h"

#include <QString>

namespace dentry::model {

    /**
     * @class FileSystemModel
     * @brief Concrete model that reads the local filesystem.
     *
     * Uses QDir and dentry::fs::FileInfo to populate entries.
     * Supports hidden file filtering, search filtering, and sorting
     * with directories always listed before files.
     *
     * Example:
     * @code
     * auto model = std::make_unique<FileSystemModel>(parent);
     * treeView->setModel(model.get());
     * model->setDirectory(QDir::homePath());
     * model.release(); // Qt parent now owns the model.
     * @endcode
     */
    class FileSystemModel final : public AFileSystemModel {
        Q_OBJECT

    public:
        explicit FileSystemModel(QObject *parent = nullptr);

        ~FileSystemModel() override = default;

        FileSystemModel(const FileSystemModel &)            = delete;
        FileSystemModel &operator=(const FileSystemModel &) = delete;
        FileSystemModel(FileSystemModel &&)                 = delete;
        FileSystemModel &operator=(FileSystemModel &&)      = delete;

        /**
         * @brief Loads and displays the contents of the given directory.
         *
         * Populates m_entries with FileItem instances sorted directories first,
         * then alphabetically. Applies hidden and filter settings.
         *
         * Emits directoryLoaded() on success, errorOccurred() on failure.
         *
         * @param path Absolute path of the directory to load.
         */
        void setDirectory(const QString &path) override;

        /**
         * @brief Reloads the current directory.
         *
         * Equivalent to calling setDirectory(currentPath()).
         */
        void refresh() override;
    };

} // namespace dentry::model

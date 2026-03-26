/**
 * @file IFileSystemModel.h
 * @brief Interface for all filesystem models.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "FileItem.h"

#include <QList>
#include <QString>

namespace dentry::model {

/**
 * @class IFileSystemModel
 * @brief Pure interface for filesystem models.
 *
 * Defines the contract that every filesystem model must fulfill.
 * Decouples the UI layer from any specific model implementation,
 * allowing future backends (network, archive, etc.) without UI changes.
 */
class IFileSystemModel {
public:
    virtual ~IFileSystemModel() = default;

    IFileSystemModel(const IFileSystemModel &)            = delete;
    IFileSystemModel &operator=(const IFileSystemModel &) = delete;
    IFileSystemModel(IFileSystemModel &&)                 = delete;
    IFileSystemModel &operator=(IFileSystemModel &&)      = delete;

    /**
     * @brief Sets the directory to display.
     * @param path Absolute path of the directory.
     */
    virtual void setDirectory(const QString &path) = 0;

    /**
     * @brief Returns the current directory path.
     */
    [[nodiscard]] virtual QString currentPath() const = 0;

    /**
     * @brief Returns the list of entries in the current directory.
     */
    [[nodiscard]] virtual QList<FileItem> entries() const = 0;

    /**
     * @brief Refreshes the current directory contents.
     */
    virtual void refresh() = 0;

    /**
     * @brief Sets whether hidden dot-files are shown.
     * @param show True to show hidden files.
     */
    virtual void setShowHidden(bool show) = 0;

    /**
     * @brief Returns whether hidden files are currently shown.
     */
    [[nodiscard]] virtual bool showHidden() const = 0;

    /**
     * @brief Sets the search filter string.
     *
     * Only entries whose name contains the filter string are shown.
     * Pass an empty string to clear the filter.
     *
     * @param filter The search string.
     */
    virtual void setFilter(const QString &filter) = 0;

    /**
     * @brief Returns the current search filter string.
     */
    [[nodiscard]] virtual QString filter() const = 0;

protected:
    IFileSystemModel() = default;
};

} // namespace dentry::model

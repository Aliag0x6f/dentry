/**
 * @file StatusBar.h
 * @brief Status bar widget for the Dentry file manager.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "../AUIComponent.h"
#include "../../model/FileItem.h"

#include <QLabel>
#include <QList>
#include <QPointer>
#include <QStatusBar>
#include <QString>

namespace dentry::ui {

    /**
     * @class StatusBar
     * @brief Displays directory statistics and selection info at the bottom of the window.
     *
     * Shows the number of folders and files in the current directory,
     * and updates when the selection changes.
     *
     * Example:
     * @code
     * auto statusBar = std::make_unique<StatusBar>(this);
     * setStatusBar(statusBar.get());
     * statusBar->setDirectoryStats(4, 12);
     * statusBar->setSelection({ item1, item2 });
     * statusBar.release(); // QMainWindow now owns the status bar.
     * @endcode
     */
    class StatusBar : public QStatusBar, public AUIComponent {
        Q_OBJECT

    public:
        /**
         * @brief Constructs the StatusBar.
         * @param parent Optional Qt parent widget.
         */
        explicit StatusBar(QWidget *parent = nullptr);

        ~StatusBar() override = default;

        StatusBar(const StatusBar &)            = delete;
        StatusBar &operator=(const StatusBar &) = delete;
        StatusBar(StatusBar &&)                 = delete;
        StatusBar &operator=(StatusBar &&)      = delete;

        void build() override;

    public slots:
        /**
         * @brief Updates the directory statistics.
         * @param folderCount Number of directories in the current directory.
         * @param fileCount   Number of files in the current directory.
         */
        void setDirectoryStats(int folderCount, int fileCount);

        /**
         * @brief Updates the selection information.
         * @param selected List of currently selected FileItems.
         */
        void setSelection(const QList<model::FileItem> &selected);

        /**
         * @brief Clears all displayed information.
         */
        void clear();

    protected:
        void setupSize() override;

    private:
        void updateDisplay();

        /** @brief Label displaying folder and file counts. */
        QPointer<QLabel> m_statsLabel;

        /** @brief Label displaying selection count. */
        QPointer<QLabel> m_selectionLabel;

        int m_folderCount    = 0;
        int m_fileCount      = 0;
        int m_selectionCount = 0;
    };

} // namespace dentry::ui

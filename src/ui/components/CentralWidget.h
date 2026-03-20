/**
 * @file CentralWidget.h
 * @brief Central widget of the Dentry main window.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "../AUIComponent.h"
#include "FileListView.h"
#include "PreviewPanel.h"
#include "Sidebar.h"
#include "../../model/FileSystemModel.h"

#include <QHBoxLayout>
#include <QPointer>
#include <QSplitter>
#include <QWidget>

namespace dentry::ui {

/**
 * @class CentralWidget
 * @brief Owns and lays out the Sidebar, FileListView and PreviewPanel inside a QSplitter.
 *
 * MainWindow sets this as its central widget and interacts
 * with its children only through the provided accessors.
 *
 * Example:
 * @code
 * auto central = std::make_unique<CentralWidget>(model, this);
 * setCentralWidget(central.get());
 * connect(central->sidebar(),      &Sidebar::placeSelected,          this, &MainWindow::navigateTo);
 * connect(central->fileListView(), &FileListView::directoryRequested, this, &MainWindow::navigateTo);
 * central.release(); // QMainWindow now owns the central widget.
 * @endcode
 */
class CentralWidget : public QWidget, public AUIComponent {
    Q_OBJECT

public:
    /**
     * @brief Constructs the CentralWidget.
     * @param model  The file system model shared with FileListView.
     * @param parent Optional Qt parent.
     */
    explicit CentralWidget(model::FileSystemModel *model, QWidget *parent = nullptr);

    ~CentralWidget() override = default;

    CentralWidget(const CentralWidget &)            = delete;
    CentralWidget &operator=(const CentralWidget &) = delete;
    CentralWidget(CentralWidget &&)                 = delete;
    CentralWidget &operator=(CentralWidget &&)      = delete;

    /** @brief Returns the sidebar. */
    Sidebar      *sidebar()       const { return m_sidebar; }

    /** @brief Returns the file list view. */
    FileListView *fileListView()  const { return m_fileListView; }

    /** @brief Returns the preview panel. */
    PreviewPanel *previewPanel()  const { return m_previewPanel; }

    void build() override;

protected:
    void setupSize()  override;
    void setupStyle() override;

private:
    QPointer<model::FileSystemModel> m_model;
    QPointer<QHBoxLayout>            m_layout;
    QPointer<QSplitter>              m_splitter;
    QPointer<Sidebar>                m_sidebar;
    QPointer<FileListView>           m_fileListView;
    QPointer<PreviewPanel>           m_previewPanel;
};

} // namespace dentry::ui

/**
 * @file CentralWidget.h
 * @brief Central widget of the Dentry main window.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "../UIComponent.h"
#include "FileListView.h"
#include "PreviewPanel.h"
#include "Sidebar.h"
#include "../../model/FileSystemModel.h"

#include <QHBoxLayout>
#include <QList>
#include <QPointer>
#include <QSplitter>
#include <QWidget>

namespace dentry::ui {

/**
 * @class CentralWidget
 * @brief Owns and lays out the Sidebar, FileListView and PreviewPanel inside a QSplitter.
 *
 * MainWindow sets this as its central widget and interacts through
 * behavior methods plus a minimal set of child accessors when needed.
 * Example:
 * @code
 * auto central = std::make_unique<CentralWidget>(model, this);
 * setCentralWidget(central.get());
 * connect(central->sidebar(), &Sidebar::placeSelected, this, &MainWindow::navigateTo);
 * connect(central->fileListView(), &FileListView::directoryRequested, this, &MainWindow::navigateTo);
 * central.release(); // QMainWindow now owns the central widget.
 * @endcode
 */
class CentralWidget : public UIComponent<QWidget, QHBoxLayout> {
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

    /** @brief Exposes sidebar for integration wiring from MainWindow. */
    Sidebar *sidebar() const { return m_sidebar; }

    /** @brief Exposes file list view for integration wiring from MainWindow. */
    FileListView *fileListView() const { return m_fileListView; }

    /** @brief Updates sidebar visibility policy. */
    void setSidebarShowHidden(bool show);

public slots:
    /** @brief Clears preview content. */
    void clearPreview();

    /** @brief Updates preview based on the current selection policy. */
    void updatePreviewFromSelection(const QList<model::FileItem> &selected);

protected:
    /** @brief Creates the splitter structure and adds child panels to the layout. */
    void setupLayout(QHBoxLayout &layout) override;

    /** @brief Applies margins and spacing for the central area layout. */
    void setupSize()  override;

    /** @brief Applies object names used by styling rules. */
    void setupStyle() override;

private:
    QPointer<model::FileSystemModel> m_model;
    QPointer<QSplitter>              m_splitter;
    QPointer<Sidebar>                m_sidebar;
    QPointer<FileListView>           m_fileListView;
    QPointer<PreviewPanel>           m_previewPanel;
};

} // namespace dentry::ui

/**
 * @file MainWindow.cpp
 * @brief Implementation of MainWindow.
 *
 * @author Hugo Fabresse
 */

#include "app/controllers/KeyboardController.h"
#include "app/controllers/NavigationController.h"
#include "app/controllers/FileOperationController.h"
#include "model/context_menu/MenuItemDefinition.h"
#include "ui/MainWindow.h"
#include "log/Logger.h"
#include "app/bindings/FileListBindings.h"
#include "app/bindings/SidebarBindings.h"

#include <QApplication>
#include <QDir>

namespace dentry::ui {

    static QList<model::MenuItemDefinition>
    buildMenuDefinitions(app::FileOperationController *ctrl) {
        using Ctx = model::ContextMenu;
        using Def = model::MenuItemDefinition;

        return {
            Def{
                "Copy", {}, false,
                [](const Ctx &c) { return !c.selectedPaths.isEmpty(); },
                [ctrl](const Ctx &c) { ctrl->onCopyRequested(c.selectedPaths); }
            },
            Def{
                "Cut", {}, false,
                [](const Ctx &c) { return !c.selectedPaths.isEmpty(); },
                [ctrl](const Ctx &c) { ctrl->onCutRequested(c.selectedPaths); }
            },
            Def{
                "Paste", {}, false,
                [](const Ctx &c) { return !c.clipboardEmpty; },
                [ctrl](const Ctx &c) { ctrl->onPasteRequested(c.currentDirectory); }
            },

            Def::separator(),

            Def{
                "Delete", {}, false,
                [](const Ctx &c) { return !c.selectedPaths.isEmpty(); },
                [ctrl](const Ctx &c) { ctrl->onDeleteRequested(c.selectedPaths); }
            },
            Def{
                "Rename", {}, false,
                [](const Ctx &c) { return c.selectedPaths.size() == 1; },
                [ctrl](const Ctx &c) { ctrl->onRenameRequested(c.selectedPaths.first()); }
            },

            Def::separator(),

            Def{
                "New File", {}, false,
                [](const Ctx &) { return true; },
                [ctrl](const Ctx &c) { ctrl->onCreateFileRequested(c.currentDirectory); }
            },
            Def{
                "New Folder", {}, false,
                [](const Ctx &) { return true; },
                [ctrl](const Ctx &c) { ctrl->onCreateFolderRequested(c.currentDirectory); }
            },
        };
    }

    MainWindow::MainWindow(QWidget *parent)
        : UIComponent(parent) {
        setupWidgets();
        build();

        log::info("Ui") << "MainWindow built";
    }

    void MainWindow::setupWidgets() {

        m_statusbar = new StatusBar(this);
        m_statusbar->setObjectName("StatusBar");
        setStatusBar(m_statusbar);

        m_centralWidget = new CentralWidget(this);
        m_centralWidget->setObjectName("CentralWidget");
        setCentralWidget(m_centralWidget);

        m_toolbar = new ToolBar(this);
        m_toolbar->setObjectName("ToolBar");
        addToolBar(m_toolbar);

        if (m_centralWidget && m_centralWidget->fileListView()) {
            m_centralWidget->fileListView()->setFocus();
        }

        log::debug("Ui") << "MainWindow widgets is set up";
    }

    void MainWindow::setupStyle() {
        setWindowTitle(QApplication::applicationDisplayName());

        log::debug("Ui") << "MainWindow style is set up";
    }

    void MainWindow::setupSize() {
        setMinimumSize(800, 500);
        resize(1100, 650);

        log::debug("Ui") << "MainWindow size is set up";
    }

    void MainWindow::setupConnections() {
        m_inputRegistry = new app::InputRegistry(this);
        m_inputRegistry->installAll({
            app::bindings::fileList(m_centralWidget->fileListView(), m_toolbar),
            app::bindings::sidebar(m_centralWidget->sidebar(), m_toolbar)
        });

        auto *view    = m_centralWidget->fileListView();
        auto *fsModel = qobject_cast<model::FileSystemModel *>(view->model());

        // Show current directory immediately on startup.
        if (m_toolbar && fsModel)
            m_toolbar->setPath(fsModel->currentPath());

        // ── Navigation ────────────────────────────────────────────────────
        auto *navController = new app::NavigationController(view, fsModel, this);

        connect(m_toolbar, &ToolBar::backRequested,
                navController, &app::NavigationController::navigateBack);

        connect(m_toolbar, &ToolBar::homeRequested,
                navController, &app::NavigationController::navigateHome);

        connect(m_centralWidget->sidebar(), &SideBar::placeActivated,
                navController, &app::NavigationController::navigateTo);

        connect(navController, &app::NavigationController::pathChanged,
                m_toolbar, &ToolBar::setPath);

        if (m_toolbar && fsModel) {
            connect(m_toolbar, &ToolBar::toggleHiddenRequested,
                    fsModel, &model::FileSystemModel::setShowHidden);

            connect(m_toolbar, &ToolBar::searchChanged,
                    fsModel, &model::FileSystemModel::setFilter);
        }

        // ── File operations ───────────────────────────────────────────────
        m_fileOpController = new app::FileOperationController(fsModel, this, this);

        m_contextMenu = new FileListContextMenu(
            new model::ContextMenuModel(buildMenuDefinitions(m_fileOpController), this),
            m_fileOpController->clipboard(),
            this
        );

        view->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(view, &QWidget::customContextMenuRequested,
                this, [this](const QPoint &pos) {
            auto *v = m_centralWidget->fileListView();
            auto *m = qobject_cast<model::FileSystemModel *>(v->model());
            if (!v || !m) return;

            m_contextMenu->popup(
                v->viewport()->mapToGlobal(pos),
                v->selectedPaths(),
                m->currentPath()
            );
        });
    }

} // namespace dentry::ui

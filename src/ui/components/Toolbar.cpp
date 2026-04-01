// src/ui/components/Toolbar.cpp
#include "log/Logger.h"
#include "ui/components/Toolbar.h"

#include <QToolButton>

namespace dentry::ui {

    ToolBar::ToolBar(QWidget *parent)
        : UIComponent(parent) {
        setupWidgets();
        build();

        setObjectName("ToolBar");

        log::info("Ui") << "ToolBar built";
    }

    void ToolBar::setupWidgets() {
        m_backAction = addAction("←");
        m_backAction->setToolTip("Back");
        m_backAction->setStatusTip("Go to previous directory");
        connect(m_backAction, &QAction::triggered, this, &ToolBar::backRequested);

        m_homeAction = addAction("~");
        m_homeAction->setToolTip("Home");
        m_homeAction->setStatusTip("Go to home directory");
        connect(m_homeAction, &QAction::triggered, this, &ToolBar::homeRequested);

        addSeparator();

        m_pathBar = new QLabel(this);
        m_pathBar->setObjectName("pathLabel");
        m_pathBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        addWidget(m_pathBar);

        m_hiddenAction = addAction("·");
        m_hiddenAction->setCheckable(true);
        m_hiddenAction->setToolTip("Show hidden files");
        m_hiddenAction->setStatusTip("Toggle the visibility of hidden files");
        connect(m_hiddenAction, &QAction::triggered, this, [this](bool checked) {
            emit toggleHiddenRequested(checked);
        });

        if (QToolButton *btn = qobject_cast<QToolButton *>(widgetForAction(m_hiddenAction)))
            btn->setObjectName("hiddenButton");

        addSeparator();

        m_searchBar = new QLineEdit(this);
        m_searchBar->setPlaceholderText("⌕ Search");
        m_searchBar->setClearButtonEnabled(true);
        connect(m_searchBar, &QLineEdit::textChanged,
                this, &ToolBar::searchChanged);
        addWidget(m_searchBar);
    }

    void ToolBar::setupStyle() {
        setMovable(false);
    }

    void ToolBar::setupSize() {
        m_searchBar->setFixedWidth(200);
    }

    void ToolBar::setPath(const QString &path) {
        m_pathBar->setText(path);
    }

    void ToolBar::triggerHome() {
        if (m_homeAction)
            m_homeAction->trigger();
    }

    void ToolBar::toggleHidden() {
        if (m_hiddenAction)
            m_hiddenAction->trigger();
    }

    void ToolBar::focusSearch() {
        if (!m_searchBar)
            return;

        m_searchBar->setFocus();
        m_searchBar->selectAll();
    }

} // namespace dentry::ui

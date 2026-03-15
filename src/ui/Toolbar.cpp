/**
* @file Toolbar.cpp
 * @brief Implementation of Toolbar.
 *
 * @author Hugo Fabresse
 */

#include "Toolbar.h"
#include <QLabel>

namespace Dentry::Ui {

    Toolbar::Toolbar(QWidget *parent)
        : QToolBar(parent) {
        build();
    }

    void Toolbar::build() {
        setMovable(false);

        m_backAction = addAction("←");
        m_backAction->setToolTip("Back");
        m_backAction->setStatusTip("Go to previous directory");

        m_homeAction = addAction("~");
        m_homeAction->setToolTip("Home");
        m_homeAction->setStatusTip("Go to home directory");

        addSeparator();

        m_pathBar = new QLabel(this);
        m_pathBar->setObjectName("pathLabel");
        m_pathBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        // addSeparator();

        m_hiddenAction = addAction("·");
        m_hiddenAction->setCheckable(true);
        m_hiddenAction->setToolTip("Show hidden files");
        m_hiddenAction->setStatusTip("Toggle the visibility of hidden files");

        addSeparator();

        m_searchBar = new QLineEdit(this);
        m_searchBar->setPlaceholderText("⌕ Search");
        m_searchBar->setFixedWidth(200);
        m_searchBar->setClearButtonEnabled(true);
        addWidget(m_searchBar);

        connect(m_backAction,   &QAction::triggered,     this, &Toolbar::backRequested);
        connect(m_homeAction,   &QAction::triggered,     this, &Toolbar::homeRequested);
        connect(m_hiddenAction, &QAction::toggled,       this, &Toolbar::hiddenToggled);
        connect(m_searchBar,    &QLineEdit::textChanged, this, &Toolbar::searchChanged);
    }

    void Toolbar::setPath(const QString &path) {
        m_pathBar->setText(path);
    }

} // namespace Dentry::Ui

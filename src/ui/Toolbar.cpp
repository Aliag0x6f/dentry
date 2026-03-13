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
        m_homeAction = addAction("~");

        addSeparator();

        m_pathBar = new QLineEdit(this);
        m_pathBar->setObjectName("pathLabel");
        m_pathBar->setReadOnly(true);
        m_pathBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        addWidget(m_pathBar);

        addSeparator();

        m_hiddenAction = addAction("·");
        m_hiddenAction->setCheckable(true);

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

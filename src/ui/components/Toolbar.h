// src/ui/components/Toolbar.h
#pragma once

#include "ui/UIComponent.h"

#include <QToolBar>
#include <QLineEdit>
#include <QPointer>
#include <QLabel>

namespace dentry::ui {

class ToolBar : public UIComponent<QToolBar, void> {
    Q_OBJECT

public:
    explicit ToolBar(QWidget *parent = nullptr);
    ~ToolBar() override = default;

    ToolBar(const ToolBar &)            = delete;
    ToolBar &operator=(const ToolBar &) = delete;
    ToolBar(ToolBar &&)                 = delete;
    ToolBar &operator=(ToolBar &&)      = delete;

    void setPath(const QString &path);

    /** @brief Triggers the same behavior as clicking the Home toolbar button. */
    void triggerHome();

    /** @brief Toggles hidden-files state exactly like clicking hiddenButton. */
    void toggleHidden();

    /** @brief Focuses search input and selects all text for immediate typing. */
    void focusSearch();

signals:
    /** @brief Emitted when the back button is clicked. */
    void backRequested();

    /** @brief Emitted when the home button is clicked. */
    void homeRequested();

    /** @brief Emitted when the hidden files toggle button is clicked. */
    void toggleHiddenRequested(bool show);

    /** @brief Emitted when the search text changes. */
    void searchChanged(const QString &text);

protected:
    void setupStyle()       override;
    void setupSize()        override;

private:
    void setupWidgets();

    QPointer<QAction>   m_backAction;
    QPointer<QAction>   m_homeAction;
    QPointer<QAction>   m_hiddenAction;
    QPointer<QLineEdit> m_searchBar;
    QPointer<QLabel>    m_pathBar;
};

} // namespace dentry::ui

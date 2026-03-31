/**
* @file ContextMenuModel.h
 * @brief Materializes a definition table into display-ready menu items.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include "model/context_menu/ContextMenu.h"
#include "model/context_menu/MenuItemDefinition.h"
#include "model/context_menu/MenuItem.h"

#include <QList>
#include <QObject>

namespace dentry::model {

    /**
     * @class ContextMenuModel
     * @brief Holds a static definition table and materializes it on demand.
     *
     * The definition table is injected once at construction. Adding a menu
     * entry only requires appending one MenuItemDefinition to the table
     * passed to the constructor — no other class needs to change.
     *
     * Example:
     * @code
     * auto *model = new ContextMenuModel(buildMenuDefinitions(controller), this);
     * model->update(context);
     * // model->items() is now ready for FileListContextMenu
     * @endcode
     */
    class ContextMenuModel : public QObject {
        Q_OBJECT

    public:
        explicit ContextMenuModel(QList<MenuItemDefinition> definitions,
                                  QObject                  *parent = nullptr);

        /**
         * @brief Evaluates every definition against @p context and updates items().
         *
         * Emits itemsChanged() after rebuilding.
         */
        void update(const ContextMenu &context);

        [[nodiscard]] QList<MenuItem> items()   const { return m_items; }
        [[nodiscard]] ContextMenu     context() const { return m_context; }

        signals:
            void itemsChanged();

    private:
        QList<MenuItemDefinition> m_definitions;
        QList<MenuItem>           m_items;
        ContextMenu               m_context;
    };

} // namespace dentry::model

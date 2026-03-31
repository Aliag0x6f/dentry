/**
* @file ContextMenuModel.cpp
 * @brief Implementation of ContextMenuModel.
 *
 * @author Hugo Fabresse
 */

#include "model/context_menu/ContextMenuModel.h"

namespace dentry::model {

    ContextMenuModel::ContextMenuModel(QList<MenuItemDefinition> definitions,
                                       QObject                  *parent)
        : QObject(parent)
        , m_definitions(std::move(definitions)) {}

    void ContextMenuModel::update(const ContextMenu &context) {
        m_context = context;
        m_items.clear();
        m_items.reserve(m_definitions.size());

        for (const MenuItemDefinition &def : m_definitions) {
            if (def.isSeparator) {
                m_items << MenuItem::separator();
                continue;
            }

            MenuItem item;
            item.label   = def.labelFn ? def.labelFn(context) : def.label;
            item.enabled = def.isEnabled && def.isEnabled(context);

            if (def.execute) {
                item.execute = [exec = def.execute, context] { exec(context); };
            }

            m_items << item;
        }

        emit itemsChanged();
    }

} // namespace dentry::model

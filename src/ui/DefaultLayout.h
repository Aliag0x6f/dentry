/**
 * @file DefaultLayout.h
 * @brief Typed layout wrappers with shared default spacing/margins.
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

namespace dentry::ui {

    /**
     * @brief Thin wrapper applying default margin/spacing policy to Qt box layouts.
     *
     * The defaults can be overridden in component setup methods when needed.
     */
    template<typename TLayout>
    class DefaultLayout : public TLayout {
    public:
        explicit DefaultLayout(QWidget *parent = nullptr)
            : TLayout(parent) {
            configure();
        }

    protected:
        /** @brief Applies default layout policy shared by UI components. */
        virtual void configure() {
            TLayout::setContentsMargins(6, 6, 6, 6);
            TLayout::setSpacing(6);
        }
    };

    using HLayout = DefaultLayout<QHBoxLayout>;
    using VLayout = DefaultLayout<QVBoxLayout>;

} // namespace dentry::ui

/**
 * @file UIComponent.h
 * @brief Typed base template for buildable UI components.
 *
 * Provides an ordered setup pipeline:
 * - with layout: setupLayout(layout) -> setupSize() -> setupStyle() -> setupConnections()
 * - without layout: setupSize() -> setupStyle() -> setupConnections()
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QLayout>
#include <QWidget>
#include <QtGlobal>

#include <type_traits>

namespace dentry::ui {

    /**
     * @brief Base template for UI components that own a concrete layout type.
     *
     * TWidget must derive from QWidget. TLayout must derive from QLayout and be
     * constructible with a QWidget parent.
     */
    template<typename TWidget, typename TLayout>
    class UIComponent : public TWidget {
    public:
        explicit UIComponent(QWidget *parent = nullptr)
            : TWidget(parent) {
            static_assert(std::is_base_of_v<QWidget, TWidget>, "TWidget must derive from QWidget");
            static_assert(std::is_base_of_v<QLayout, TLayout>, "TLayout must derive from QLayout");
            static_assert(std::is_constructible_v<TLayout, QWidget *>, "TLayout must be constructible with QWidget*");
        }

        virtual ~UIComponent() = default;

        /**
         * @brief Executes the ordered setup pipeline for layout-based components.
         *
         * This method is intended to be called exactly once from the concrete
         * component constructor.
         */
        void build() {
            if (m_built) {
                Q_ASSERT_X(false, "UIComponent::build", "build() must only be called once");
                return;
            }

            if (this->layout() != nullptr || m_layout != nullptr) {
                Q_ASSERT_X(false, "UIComponent::build", "layout already exists before build()");
                return;
            }

            m_built = true;
            m_layout = new TLayout(static_cast<QWidget *>(this));
            setupLayout(*m_layout);
            setupSize();
            setupStyle();
            setupConnections();
        }

    protected:
        /** @brief Configures the owned layout structure and child placement. */
        virtual void setupLayout(TLayout &) {}
        /** @brief Applies widget size constraints. */
        virtual void setupSize() {}
        /** @brief Applies style-related metadata (object name, flags, style hints). */
        virtual void setupStyle() {}
        /** @brief Connects internal signals and slots. */
        virtual void setupConnections() {}

        /** @brief Owned layout instance created during build(). */
        TLayout *m_layout = nullptr;

        /** @brief Tracks that build() has already been executed once. */
        bool m_built = false;
    };

    /**
     * @brief Specialization for widgets that should not create/own a custom layout.
     */
    template<typename TWidget>
    class UIComponent<TWidget, void> : public TWidget {
    public:
        explicit UIComponent(QWidget *parent = nullptr)
            : TWidget(parent) {
            static_assert(std::is_base_of_v<QWidget, TWidget>, "TWidget must derive from QWidget");
        }

        virtual ~UIComponent() = default;

        /**
         * @brief Executes the ordered setup pipeline for components without owned layout.
         *
         * This method is intended to be called exactly once from the concrete
         * component constructor.
         */
        void build() {
            if (m_built) {
                Q_ASSERT_X(false, "UIComponent::build", "build() must only be called once");
                return;
            }

            m_built = true;
            setupSize();
            setupStyle();
            setupConnections();
        }

    protected:
        /** @brief Applies widget size constraints. */
        virtual void setupSize() {}
        /** @brief Applies style-related metadata (object name, flags, style hints). */
        virtual void setupStyle() {}
        /** @brief Connects internal signals and slots. */
        virtual void setupConnections() {}

        /** @brief Tracks that build() has already been executed once. */
        bool m_built = false;
    };

} // namespace dentry::ui

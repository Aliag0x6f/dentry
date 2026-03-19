/**
 * @file AppMetadata.h
 * @brief Application metadata constants.
 *
 * These values are injected by CMake at compile time via target_compile_definitions.
 *
 * @author Hugo Fabresse
 */

#pragma once

namespace Dentry::Metadata {

    /** @brief Application name (internal identifier). */
    constexpr inline auto AppName = DENTRY_APP_NAME;

    /** @brief Application display name (user-facing). */
    constexpr inline auto AppDisplayName = DENTRY_APP_NAME;

    /** @brief Application version from CMake PROJECT_VERSION. */
    constexpr inline auto AppVersion = DENTRY_APP_VERSION;

} // namespace Dentry::Metadata

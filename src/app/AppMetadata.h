/**
 * @file AppMetadata.h
 * @brief Application metadata constants.
 *
 * These values are injected by CMake at compile time via target_compile_definitions.
 *
 * @author Hugo Fabresse
 */

#pragma once

#ifndef DENTRY_APP_NAME
    #error "DENTRY_APP_NAME not defined by CMake. Ensure target_compile_definitions includes DENTRY_APP_NAME."
#endif

#ifndef DENTRY_APP_VERSION
    #error "DENTRY_APP_VERSION not defined by CMake. Ensure target_compile_definitions includes DENTRY_APP_VERSION."
#endif

namespace dentry::metadata {

    /** @brief Application name (internal identifier). */
    constexpr inline auto AppName = DENTRY_APP_NAME;

    /** @brief Application display name (user-facing). */
    constexpr inline auto AppDisplayName = AppName;

    /** @brief Application version from CMake PROJECT_VERSION. */
    constexpr inline auto AppVersion = DENTRY_APP_VERSION;

} // namespace dentry::metadata

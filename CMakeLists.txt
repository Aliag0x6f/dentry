cmake_minimum_required(VERSION 3.16)

project(dentry VERSION 1.1.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(ProcessorCount)

option(DENTRY_ENABLE_WARNINGS "Enable extra compiler warnings" ON)
option(DENTRY_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(DENTRY_ENABLE_UNITY "Enable unity build for faster clean builds" OFF)
option(DENTRY_ENABLE_PCH "Enable precompiled headers" OFF)
ProcessorCount(DENTRY_DEFAULT_BUILD_JOBS)
if(NOT DENTRY_DEFAULT_BUILD_JOBS OR DENTRY_DEFAULT_BUILD_JOBS EQUAL 0)
    set(DENTRY_DEFAULT_BUILD_JOBS 4)
endif()
set(DENTRY_BUILD_JOBS "${DENTRY_DEFAULT_BUILD_JOBS}" CACHE STRING "Maximum parallel compile jobs for this target (Ninja only)")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Concurrent)

file(GLOB_RECURSE DENTRY_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.qrc"
)

qt_add_executable(dentry
    ${DENTRY_SOURCES}
)

if(CMAKE_GENERATOR MATCHES "Ninja")
    # Limit compile/link parallelism without passing -j on each build command.
    set_property(GLOBAL PROPERTY JOB_POOLS compile_pool=${DENTRY_BUILD_JOBS};link_pool=1)
    set_property(TARGET dentry PROPERTY JOB_POOL_COMPILE compile_pool)
    set_property(TARGET dentry PROPERTY JOB_POOL_LINK link_pool)
endif()

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${DENTRY_SOURCES})

target_include_directories(dentry
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(dentry
    PRIVATE
        Qt6::Widgets
        Qt6::Concurrent
)

if(DENTRY_ENABLE_WARNINGS)
    if(MSVC)
        target_compile_options(dentry PRIVATE /W4)
        if(DENTRY_WARNINGS_AS_ERRORS)
            target_compile_options(dentry PRIVATE /WX)
        endif()
    else()
        target_compile_options(dentry PRIVATE -Wall -Wextra -Wpedantic)
        if(DENTRY_WARNINGS_AS_ERRORS)
            target_compile_options(dentry PRIVATE -Werror)
        endif()
    endif()
endif()

set_target_properties(dentry PROPERTIES
    UNITY_BUILD ${DENTRY_ENABLE_UNITY}
)

if(DENTRY_ENABLE_PCH)
    target_precompile_headers(dentry PRIVATE
        <QApplication>
        <QDebug>
        <QDir>
        <QFileInfo>
        <QString>
        <QWidget>
    )
endif()

install(TARGETS dentry
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES dentry.desktop
    DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
)

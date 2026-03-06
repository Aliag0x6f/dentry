cmake_minimum_required(VERSION 3.16)
project(dentry)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(dentry
        src/main.cpp
        src/util/DateFormatter.cpp
        src/util/PermissionFormatter.cpp
        src/util/SizeFormatter.cpp
        src/fs/MimeResolver.cpp
        src/fs/FileInfo.cpp
        src/fs/AFileOperation.cpp
)

target_link_libraries(dentry Qt6::Widgets)

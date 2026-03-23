# Dentry

A minimal, keyboard-driven file manager for Linux built with Qt6.

Dentry — named after the Linux kernel's directory entry cache structure — is a lightweight graphical file manager designed for tiling window managers and minimal desktop environments. It focuses on clarity, speed, and a consistent dark aesthetic without unnecessary UI chrome.

## Features

- Sidebar with quick-access places populated dynamically from home directory, toggleable hidden folders, Home always first, Root always last
- File list view with sortable columns: Name, Size, Type, Date Modified, Permissions
- Async file operations: Copy, Cut, Paste, Move, Delete, Rename, Create File, Create Folder
- Progress dialog with cancellation support for all file operations
- Right-click context menu with full operation support
- Toggle for hidden dot-files with visual indicator (black = off, white = on)
- Inline search with real-time filtering
- File preview panel for text and images
- Dark theme — black/white only, zone-based layout with border-radius, no external theme dependency
- Colored terminal logger with category filtering via `DENTRY_DEBUG` environment variable
- Full keyboard and mouse support

## Technical Specifications

| Property | Value |
|---|---|
| Language | C++17 |
| UI Framework | Qt6 Widgets + Qt6 Concurrent |
| Build System | CMake 3.16+ |
| License | GPLv3 |
| Tested on | Arch Linux |

## Ownership and RAII Policy

To keep lifetimes explicit and safe, Dentry follows these ownership rules:

- `QObject` lifetimes are managed by Qt parent-child ownership.
- Non-Qt ownership uses `std::unique_ptr`.
- Raw pointers are non-owning observers only.
- Business code must not rely on unmanaged `new`/`delete` ownership pairs.

Practical examples:

- File operations in controllers are created via `std::unique_ptr`; dialogs observe them through non-owning pointers during execution.
- UI members that reference widgets/components use `QPointer` for safe, non-owning access to parent-owned objects.

References:

- RAII: https://en.cppreference.com/w/cpp/language/raii
- `std::unique_ptr`: https://en.cppreference.com/w/cpp/memory/unique_ptr
- Qt object trees: https://doc.qt.io/qt-6/objecttrees.html

## Requirements

- Qt6 Base (`qt6-base`)
- Qt6 Concurrent (`qt6-base`)
- CMake 3.16 or later
- A C++17-compatible compiler (GCC 9+, Clang 10+)

## Namespace Conventions

To avoid catch-all namespaces and improve navigation, Dentry follows this namespace taxonomy:

- `dentry::log`: logging API (`install`, `debug`, `info`, `warn`, `error`)
- `dentry::formatter`: stateless formatting helpers (`formatDateTime`, `formatSize`, `formatPermissions`, ...)
- `dentry::app`, `dentry::fs`, `dentry::model`, `dentry::ui`: core application domains

Conventions:

- All namespaces use lower-case domain names.
- New helper modules must use focused, lower-case namespaces.
- Avoid reintroducing broad catch-all namespaces like `Util`.

## Coding Standards

### Encapsulation and Behavior-First APIs

To preserve encapsulation and avoid Law-of-Demeter violations, Dentry favors behavior methods on owning classes over exposing internal widget state through getter chains.

Guidelines:

- Prefer intent-level methods/signals on owning objects (example: `CentralWidget::setSidebarShowHidden`, `CentralWidget::updatePreviewFromSelection`).
- Avoid caller-side orchestration through nested getters (example to avoid: `main->central()->previewPanel()->...`).
- Keep simple accessors only when they are stable, necessary, and do not leak orchestration responsibilities.

Issue #19 refactor scope (target classes):

- `src/ui/components/CentralWidget.*`: own child wiring and expose behavior-first API.
- `src/ui/MainWindow.cpp`: consume intent-level API instead of child getters.

### Const-Correctness and `constexpr`

Dentry applies `constexpr` systematically to make immutability explicit, enable compile-time evaluation, and prevent accidental runtime mutability.

**Guidelines:**

- Mark all compile-time constants with `constexpr` (not just `const`).
- Apply `constexpr` to pure, side-effect-free functions where meaningful and correct.
- Use `constexpr inline` for header-only constants to avoid ODR violations.
- Avoid forcing `constexpr` on runtime-dependent logic (e.g., functions using Qt types like `QString`, `QDateTime`, `QLocale`).

**Examples:**

```cpp
// ✓ Correct: compile-time constant
constexpr inline auto AppName = DENTRY_APP_NAME;
static constexpr const char *RED = "\033[31m";

// ✗ Avoid: Qt runtime types cannot be constexpr
[[nodiscard]] QString formatSize(qint64 bytes);  // Keep as non-constexpr
```

**References:**

- https://en.cppreference.com/w/cpp/language/constexpr
- https://en.cppreference.com/w/cpp/language/constant_initialization

## Installation

### Arch Linux (AUR)
```bash
yay -S dentry
# or
paru -S dentry
```

### Arch Linux / Manjaro (manual)
```bash
sudo pacman -S qt6-base cmake base-devel
```

### Ubuntu 22.04+ / Debian Bookworm+
```bash
sudo apt install qt6-base-dev cmake build-essential
```

### Fedora
```bash
sudo dnf install qt6-qtbase-devel cmake gcc-c++
```

## Building
```bash
git clone https://github.com/Hugo-Fabresse/dentry.git
cd dentry
cmake -B build
cmake --build build
```

The compiled binary will be located at `build/dentry`.

### Install system-wide
```bash
sudo cmake --install build
```

## Running
```bash
./build/dentry
```

Or if installed system-wide:
```bash
dentry
```

### Debug logging
```bash
# Enable all logs
DENTRY_DEBUG=1 dentry

# Filter by category
DENTRY_DEBUG=1 QT_LOGGING_RULES="*.debug=true;Mime.debug=false;FileInfo.debug=false" dentry
```

## Usage

### Navigation

| Action | Method |
|---|---|
| Open folder | Double-click or Enter |
| Go back | Click "←" or Backspace |
| Go to Home | Click "~" |
| Jump to a place | Click an entry in the sidebar |
| Sort by column | Click a column header |

### File Operations

| Action | Method |
|---|---|
| Copy | Right-click → Copy |
| Cut | Right-click → Cut |
| Paste | Right-click → Paste |
| Delete | Right-click → Delete |
| Rename | Right-click → Rename |
| New file | Right-click → New File |
| New folder | Right-click → New Folder |
| Open file | Double-click (uses default app) |

### Search

Type in the search bar to filter the current directory in real time. The filter applies only to the current directory and clears on navigation.

### Hidden Files

Click the "·" button in the toolbar to toggle visibility of dot-files. Black background = hidden files off. White background = hidden files visible. The sidebar updates accordingly.

## Project Structure
```
dentry/
├── CMakeLists.txt
├── Doxyfile
├── LICENSE
├── README.md
├── dentry.desktop
├── doc/
└── src/
    ├── main.cpp
    ├── app/
    │   ├── Application.h/.cpp
    │   └── Clipboard.h/.cpp
    ├── fs/
    │   ├── IFileOperation.h
    │   ├── AFileOperation.h/.cpp
    │   ├── FileInfo.h/.cpp
    │   ├── MimeResolver.h/.cpp
    │   └── operations/
    │       ├── CopyOperation.h/.cpp
    │       ├── MoveOperation.h/.cpp
    │       ├── DeleteOperation.h/.cpp
    │       ├── RenameOperation.h/.cpp
    │       ├── CreateFileOperation.h/.cpp
    │       └── CreateFolderOperation.h/.cpp
    ├── model/
    │   ├── IFileSystemModel.h
    │   ├── AFileSystemModel.h/.cpp
    │   ├── FileSystemModel.h/.cpp
    │   └── FileItem.h
    ├── ui/
    │   ├── MainWindow.h/.cpp
    │   ├── Sidebar.h/.cpp
    │   ├── FileListView.h/.cpp
    │   ├── PreviewPanel.h/.cpp
    │   ├── Toolbar.h/.cpp
    │   ├── StatusBar.h/.cpp
    │   ├── Style.h
    │   ├── style.qss
    │   └── resources.qrc
    ├── log/
    │   └── Logger.h/.cpp
    └── formatter/
        ├── SizeFormatter.h/.cpp
        ├── DateFormatter.h/.cpp
        └── PermissionFormatter.h/.cpp
```

## License

GPLv3 — see [LICENSE](LICENSE).
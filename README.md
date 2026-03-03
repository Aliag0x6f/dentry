# Dentry

A minimal, keyboard-driven file manager for Linux built with Qt6.

Dentry — named after the Linux kernel's directory entry cache structure — is a lightweight
graphical file manager designed for tiling window managers and minimal desktop environments.
It focuses on clarity, speed, and a consistent dark aesthetic without unnecessary UI chrome.

---

## Features

- Sidebar with quick-access places (Home, Desktop, Documents, Downloads, Pictures, Videos, Music, .config, /)
- File list view with sortable columns: Name, Size, Type, Date Modified, Permissions
- Toolbar with file operations: create file, create folder, rename, delete
- Toggle for hidden dot-files with visual indicator
- Inline search with real-time filtering
- Right-click context menu
- Full keyboard and mouse support
- Dark theme with no external theme dependency

---

## Technical Specifications

| Property       | Value                        |
|----------------|------------------------------|
| Language       | C++17                        |
| UI Framework   | Qt6 (Widgets)                |
| Build System   | CMake 3.16+                  |
| License        | MIT                          |
| Tested on      | Arch Linux, Ubuntu 22.04+    |

---

## Requirements

- Qt6 Base (`qt6-base`)
- CMake 3.16 or later
- A C++17-compatible compiler (GCC 9+, Clang 10+)

---

## Installation

### Arch Linux / Manjaro

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

---

## Building

Clone the repository and build with CMake:

```bash
git clone https://github.com/Hugo-Fabresse/dentry.git
cd dentry
cmake -B build
cmake --build build
```

The compiled binary will be located at `build/dentry`.

### Optional: install system-wide

```bash
sudo cmake --install build
```

This installs the binary to `/usr/local/bin/dentry`.

---

## Running

```bash
./build/dentry
```

Or if installed system-wide:

```bash
dentry
```

---

## Usage

### Navigation

| Action                        | Method                              |
|-------------------------------|-------------------------------------|
| Open folder                   | Double-click or Enter               |
| Go to parent directory        | Click "Back" or Backspace           |
| Go to Home                    | Click "Home"                        |
| Jump to a place               | Click an entry in the sidebar       |
| Sort by column                | Click a column header               |

### File Operations

| Action                        | Method                              |
|-------------------------------|-------------------------------------|
| Create file                   | Toolbar "File" button               |
| Create folder                 | Toolbar "Folder" button             |
| Rename                        | Toolbar "Rename" or right-click     |
| Delete                        | Toolbar "Delete" or right-click     |
| Copy path to clipboard        | Right-click "Copy path"             |
| Open file                     | Double-click (uses default app)     |

### Search

Type in the search bar to filter the current directory in real time.
The filter applies only to the current directory and clears on navigation.

### Hidden Files

Click the "Hidden" button in the toolbar to toggle visibility of dot-files.
The button displays a white background when hidden files are visible.

---

## Project Structure

```
dentry/
├── CMakeLists.txt
└── src/
    ├── main.cpp          # Entry point
    ├── MainWindow.h      # Window declaration with Doxygen documentation
    ├── MainWindow.cpp    # UI logic and file operations
    └── Style.h           # QSS stylesheet (fully separated from logic)
```


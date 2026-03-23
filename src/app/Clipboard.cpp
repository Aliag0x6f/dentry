/**
 * @file Clipboard.cpp
 * @brief Implementation of Clipboard.
 *
 * @author Hugo Fabresse
 */

#include "Clipboard.h"
#include "../log/Logger.h"

namespace dentry::app {

    void Clipboard::copy(const QStringList &paths) {
        m_paths = paths;
        m_isCut = false;
        log::debug("Clipboard") << "Copy:" << paths.count() << "item(s)";
    }

    void Clipboard::cut(const QStringList &paths) {
        m_paths = paths;
        m_isCut = true;
        log::debug("Clipboard") << "Cut:" << paths.count() << "item(s)";
    }

    void Clipboard::clear() {
        m_paths.clear();
        m_isCut = false;
        log::debug("Clipboard") << "Cleared";
    }

    const QStringList &Clipboard::paths() const {
        return m_paths;
    }

    bool Clipboard::isCut() const {
        return m_isCut;
    }

    bool Clipboard::isEmpty() const {
        return m_paths.isEmpty();
    }

} // namespace dentry::app

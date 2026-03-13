/**
 * @file Style.h
 * @brief Loader for the Dentry QSS stylesheet.
 *
 * The stylesheet is defined in style.qss and embedded at compile time
 * via the Qt Resource System (resources.qrc).
 * Applied once at startup via QApplication::setStyleSheet().
 *
 * Dark theme designed for minimal desktop environments.
 * Fully self-contained — no external theme dependency.
 *
 * Color palette:
 * - Background:  #0a0a0a
 * - Surface:     #080808 / #111111
 * - Text:        #d7d7d7 / #eeeeee
 * - Selection:   #ffffff (bg) / #000000 (fg)
 * - Border:      rgba(255, 255, 255, 20)
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QFile>
#include <QString>

namespace Dentry::Ui {

    /**
     * @class Style
     * @brief Loads and provides the QSS stylesheet from embedded resources.
     *
     * All methods are static. This class is not meant to be instantiated.
     */
    class Style {
    public:
        Style()  = delete;
        ~Style() = delete;

        Style(const Style &)            = delete;
        Style &operator=(const Style &) = delete;
        Style(Style &&)                 = delete;
        Style &operator=(Style &&)      = delete;

        /**
         * @brief Loads and returns the QSS stylesheet from Qt resources.
         *
         * Reads the stylesheet from the embedded resource at :/ui/style.qss.
         * Returns an empty string if the resource cannot be opened.
         *
         * @return A QString containing the complete application stylesheet.
         */
        [[nodiscard]] static inline QString sheet() {
            QFile file(":/ui/style.qss");
            file.open(QIODevice::ReadOnly);
            return QString::fromUtf8(file.readAll());
        }
    };

} // namespace Dentry::Ui

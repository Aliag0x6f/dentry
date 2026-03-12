/**
* @file Style.h
 * @brief Qt stylesheet (QSS) for the Dentry application.
 *
 * Dark theme designed for minimal desktop environments.
 * Fully self-contained — no external theme dependency.
 *
 * Color palette:
 * - Background:  #0a0a0a
 * - Surface:     #080808 / #111111
 * - Text:        #d7d7d7 / #eeeeee
 * - Selection:   #ffffff (bg) / #000000 (fg)
 * - Border:      rgba(255,255,255,0.08-0.15)
 *
 * @author Hugo Fabresse
 */

#pragma once

#include <QString>

namespace Dentry::Ui {

    /**
     * @class Style
     * @brief Provides the QSS stylesheet for the Dentry application.
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
         * @brief Returns the full QSS stylesheet string.
         * @return A QString containing the complete application stylesheet.
         */
        [[nodiscard]] static QString sheet();
    };

} // namespace Dentry::Ui

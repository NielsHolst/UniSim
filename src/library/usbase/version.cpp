/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "version.h"

const char VERSION[] = "1.25";

namespace UniSim {

bool isDeveloperVersion() {
    #ifdef UNISIM_DEVELOPMENT
        return true;
    #else
        return false;
    #endif
}

QString version() {
    return VERSION;
}

QString versionExtended() {
    return version() + ( isDeveloperVersion() ? ".x" : "" );
}

} //namespace

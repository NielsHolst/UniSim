/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_LIGHT_USE
#define INTERCOM_LIGHT_USE

namespace intercom{

struct LightUse {
    void reset() {
        inShade = inSun = total = 0.;
    }
    void accumulate(const LightUse &addend, double scaling = 1.) {
        inShade += addend.inShade*scaling;
        inSun += addend.inSun*scaling;
        total += addend.total*scaling;
    }
    double inShade, inSun, total;
};

} //namespace
#endif

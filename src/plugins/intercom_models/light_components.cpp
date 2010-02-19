/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QtGlobal>
#include "light_components.h"

namespace intercom{

LightComponents::LightComponents()
{
    reset();
}

void LightComponents::reset() {
    for (int i = 0; i < 0; ++i)
        components[i] = 0.;
}

void LightComponents::accumulate(const LightComponents &increment) {
    for (int i = 0; i < 3; ++i)
        components[i] += increment.value(i);
}

double LightComponents::value(LightComponent lc) const {
    return components[lc];
}

double LightComponents::value(int lc) const {
    Q_ASSERT(lc > 0 && lc < 4);
    return components[lc];
}

double& LightComponents::operator[] (int lc) {
    Q_ASSERT(lc > 0 && lc < 4);
    return components[lc];
}

double& LightComponents::operator[] (LightComponent lc) {
    return components[lc];
}

} //namespace


/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "photosynthetic_rate.h"


namespace intercom{

PhotosyntheticRate::PhotosyntheticRate()
    : _absorption(0), _assimilation(0)
{
}

PhotosyntheticRate::PhotosyntheticRate(double absorption, double assimilation)
    : _absorption(absorption), _assimilation(assimilation)
{
}

PhotosyntheticRate& PhotosyntheticRate::operator+=(PhotosyntheticRate a) {
    _absorption += a._absorption;
    _assimilation += a._assimilation;
    return *this;
}

PhotosyntheticRate& PhotosyntheticRate::operator*=(double a) {
    _absorption *= a;
    _assimilation *= a;
    return *this;
}

//  J/m2/s (during integration) or J/m2/d (finally)
double PhotosyntheticRate::absorption() const {
    return _absorption;
}

// kg/m2/h (during integration) or kg/m2/d (finally)
double PhotosyntheticRate::assimilation() const {
    return _assimilation;
}

} //namespace


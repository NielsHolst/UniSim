/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_PHOTOSYNTHETIC_RATE
#define INTERCOM_PHOTOSYNTHETIC_RATE

namespace intercom{

class PhotosyntheticRate
{
public: 
    PhotosyntheticRate();
    PhotosyntheticRate(double absorption, double assimilation);
    double absorption() const;
    double assimilation() const;
    PhotosyntheticRate& operator+=(PhotosyntheticRate a);
    PhotosyntheticRate& operator*=(double a);
private:
    double _absorption, _assimilation;
};

} //namespace
#endif

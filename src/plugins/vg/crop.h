/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_CROP_H
#define VG_CROP_H

#include <usbase/model.h>

namespace vg {

class Crop : public UniSim::Model
{
public:
    Crop(UniSim::Identifier name, QObject *parent);

private:
    double density;
};
} //namespace


#endif

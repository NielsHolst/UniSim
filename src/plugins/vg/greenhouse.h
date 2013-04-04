/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_GREENHOUSE_H
#define VG_GREENHOUSE_H

#include <usbase/model.h>

namespace vg {

class Greenhouse : public UniSim::Model
{
public:
    Greenhouse(UniSim::Identifier name, QObject *parent);
    void reset();

private:
    // Parameters
    double width, length, height, latcov, Kcover, trDif;

    // Variables
    double volume, groundArea, coverArea;
};
} //namespace


#endif

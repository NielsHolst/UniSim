/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SCREENS_H
#define VG_SCREENS_H

#include <usbase/model.h>
#include <variable_collection.h>

namespace vg {

class Screens : public UniSim::Model
{
public:
    Screens(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();

private:
    // Variables
    double temperature, airTransmission, lightTransmission, dew;
    // Data
    VariableCollection t, at, lt, d;
};
} //namespace


#endif

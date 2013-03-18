/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_ENVIRONMENT_H
#define VG_ENVIRONMENT_H

#include <usbase/model.h>

namespace vg {

class Environment : public UniSim::Model
{
public:
    Environment(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();

private:
    // Parameters
    double co2;

    // Variables
    double directRadiation, Tsky, Esky, Tcover, Tscreen, Tsoil;

    // Links
    UniSim::Model *records;
    const double *TSkyRecords, *Tair, *windspeed;
};
} //namespace


#endif

/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_MACROPOREFLOW_H
#define INSECTICIDE_MACROPOREFLOW_H

#include <usbase/model.h>

namespace insecticide {

class macroporeflow : public UniSim::Model
{
public:
    macroporeflow(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double fsand, fsilt, fclay, fw, fa, fmacropore, Flowrate, frr,
    tpevent, P, frsw, Tf, Doseaflm;

    // Output
    double fporemobile, Vporeimmobile, C, Cused, Cfree,
    Pcapacitymm, Preqh, Preqday, Pi,
    Dmacropore, Vporemobile;
};

} //namespace


#endif

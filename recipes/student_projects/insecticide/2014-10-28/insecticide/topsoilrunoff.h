/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_TOPSOILRUNOFF_H
#define INSECTICIDE_TOPSOILRUNOFF_H

#include <usbase/model.h>

namespace insecticide {

class topsoilrunoff : public UniSim::Model
{
public:
    topsoilrunoff(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double S, wbz, Doseaflrm, Doseldlobr, P, KocpH, fom, Q;

    // Output
    double Fslope, Fbuffer, F, Dosesr, Kd;
};

} //namespace


#endif

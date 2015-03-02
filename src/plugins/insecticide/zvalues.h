/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_ZVALUES_H
#define INSECTICIDE_ZVALUES_H

#include <usbase/model.h>

namespace insecticide {

class zvalues : public UniSim::Model
{
public:
    zvalues(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double MW, Pd,
    Psol, VP, foc, pb, fa, fw, fs, farea,
    KocpH, TrefVP, Tsa, R, Sp;

    // Output
    double Za, Zw, Zs, VPTadap, Vs, Vw, Va, VZ;
};

} //namespace


#endif

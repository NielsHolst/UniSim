/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "primarydistributionleaf.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (primarydistributionleaf)
	
primarydistributionleaf::primarydistributionleaf(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, Doseappl, 1.);         //applied dosage kg/ha
    Input (double, Dosedrift, 1.);
    Input(double, stage1, 0.);
    Input(double, stage2, 0.);
    Input(double, stage3, 0.);
    Input(double, stage4, 0.);

    Output (double, Doserl);
    Output (double, fi);            //fraction intercepted

}

void primarydistributionleaf::reset() {

    Doserl = 0.;
    fi = 0.;

}

void primarydistributionleaf::update() {

    //stage 1 = leaf development, stage 2 = tillering
    //stage 3 = stem elongation, and stage 4 = senescent
    fi = 0.25*stage1 + 0.5*stage2 + 0.7*stage3 + 0.9*stage4;
    Doserl = (Doseappl - Dosedrift) * fi;

}

} //namespace


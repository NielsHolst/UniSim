/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "WindDriftTOT.h"
#include "publish.h"
#include "iostream"

using namespace UniSim;

namespace insecticide {

PUBLISH (WindDriftTOT)
	
WindDriftTOT::WindDriftTOT(Identifier name, QObject *parent)
    : WindDrift(name, parent)
{
    Output (double, totaldrift);
}

void WindDriftTOT::reset() {
    totaldrift = 0.;
}

void WindDriftTOT::update() {
    totaldrift = 0;
    int npass = int((110 - P)/Wboom);
    for(int i = 1; i <= npass; ++i){
        totaldrift += calcAmountdrifted(i);
    }
}

} //namespace


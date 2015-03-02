/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "winddrifttotal.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (winddrifttotal)
	
winddrifttotal::winddrifttotal(Identifier name, QObject *parent)
    : winddrift(name, parent)
    //: Model(name, parent) ...notice here "Model(name, parent)" is replaced by "WindDrift(name, parent)"
{

    Output (double, totaldrift);

}

void winddrifttotal::reset() {

    totaldrift = 0.;

}

void winddrifttotal::update() {

    totaldrift = 0;
    int npass = int((110. - P)/Wboom);
    for(int i = 1; i <= npass; ++i){
    totaldrift += calcAmountdrifted(i); //totaldrift = sum of the total drift

}

}

} //namespace


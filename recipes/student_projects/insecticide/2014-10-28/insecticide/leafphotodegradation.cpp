/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
//#include <usbase/exception.h>
#include "leafphotodegradation.h"
#include <cmath>
#include "publish.h"


using namespace UniSim;

namespace insecticide {

PUBLISH (leafphotodegradation)
	
leafphotodegradation::leafphotodegradation(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, I, 10.);             //Wh m-2 day-1
    Input (double, kOH, 0.0003);        //cm3/(molecules*s)
    Input (double, dl, 12);

    Output (double, log_OH);            //log10(OH), log (molecules/cm3)
    Output (double, kld);
    Output (double, concentration);
    Output (double, outflow);           //fraction removed by degradation
}

void leafphotodegradation::reset() {
    concentration = 0.;
    outflow = 0.;
    log_OH = 0.;
    kld = 0.;
}

void leafphotodegradation::update() {

    log_OH = (0.0003 * I) + 4.7783;
    kld = 3600*kOH* std::pow(10, (log_OH * dl/24.));

}

} //namespace


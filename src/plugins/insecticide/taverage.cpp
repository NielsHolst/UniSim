/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "taverage.h"
#include "publish.h"

using namespace std;

using namespace UniSim;

namespace insecticide {
	
PUBLISH(taverage)

taverage::taverage(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Output (double, value);
    Output (double, Tav);
}

void taverage::initialize() {
    Model *records = seekOneChild<Model*>("records");
    Variable<double> *pullTav = records->peekOneChild<Variable<double>*>("Tav");

    temperatureIsPresent = pullTav;
    if (temperatureIsPresent) {
        ptrTav = pullTav->valuePtr();
    }
}
void taverage::reset() {
    value = 0.;

}
void taverage::update() {
    Tav = temperatureIsPresent ? *ptrTav : 0.;
    value = Tav;
    if (value < 0.)
        value = 0.;

}
} //namespace


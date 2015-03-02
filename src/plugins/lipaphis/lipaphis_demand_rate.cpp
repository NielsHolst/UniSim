/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "lipaphis_demand_rate.h"
#include "publish.h"

using namespace UniSim;

namespace lipaphis {

PUBLISH(LipaphisDemandRate)

LipaphisDemandRate::LipaphisDemandRate(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, preyQuality, 3.);
    Output(double, rate);
}

void LipaphisDemandRate::reset() {
    update();
}

void LipaphisDemandRate::update() {
    double slope= -0.0173,
           intercept= 0.1754;
    rate= slope*preyQuality + intercept;
}

} //namespace


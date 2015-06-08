/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "a.h"
#include "publish.h"

using namespace UniSim;

namespace lipaphis {

PUBLISH(a)

a::a(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, nitrogen, 100);
    Input(double, slope, 1);
    Input(double, intercept, 0);
    Output(double, lifeTimeFecundity);
}

void a::reset() {
    update();
}

void a::update() {
    lifeTimeFecundity = slope*nitrogen + intercept;
}

} //namespace


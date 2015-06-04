/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "aphid_fecundity.h"
#include "publish.h"

using namespace UniSim;

namespace lipaphis {

PUBLISH(AphidFecundity)

AphidFecundity::AphidFecundity(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, nitrogen, 100);
    Input(double, slope, 1);
    Input(double, intercept, 0);
    Output(double, lifeTimeFecundity);
}

void AphidFecundity::reset() {
    update();
}

void AphidFecundity::update() {
    lifeTimeFecundity = slope*nitrogen + intercept;
}

} //namespace


/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "aphid_fecundity1.h"
#include "publish.h"

using namespace UniSim;

namespace lipaphis {

PUBLISH(AphidFecundity1)

AphidFecundity1::AphidFecundity1(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, Ncontent , 4.);
    Output(double, fecundity);
}

void AphidFecundity1::reset() {
    fecundity = 40.;
}

void AphidFecundity1::update() {
    fecundity = 25.841+21.696*Ncontent;
}

} //namespace


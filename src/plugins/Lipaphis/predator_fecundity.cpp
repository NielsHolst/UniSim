/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "predator_fecundity.h"
#include "publish.h"

using namespace UniSim;

namespace lipaphis {

PUBLISH(PredatorFecundity)

PredatorFecundity::PredatorFecundity(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, sdRatio , 4.);
    Output(double, fecundity);
}

void PredatorFecundity::reset() {
    fecundity = 40.;
}

void PredatorFecundity::update() {

        fecundity = (sdRatio >= 0.2) ? 81.25*sdRatio-16.25 : 0.05;
}

} //namespace


/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "sex_ratio.h"
#include "publish.h"

using namespace UniSim;

namespace lipaphis {

PUBLISH(SexRatio)

SexRatio::SexRatio(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, totalNumber, 100);
    Output(double, FemaleNumber);
    Output(double, MaleNumber);
}

void SexRatio::reset() {
    update();
}

void SexRatio::update() {
    FemaleNumber = totalNumber * 0.6,
    MaleNumber = totalNumber * 0.4;
}

} //namespace


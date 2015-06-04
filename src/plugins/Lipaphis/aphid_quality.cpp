/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "aphid_quality.h"
#include "publish.h"

using namespace UniSim;

namespace lipaphis {

PUBLISH(AphidQuality)

AphidQuality::AphidQuality(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, Ncontent1 , 4.);
    Output(double, protein);
}

void AphidQuality::reset() {
    protein = 5.;
}

void AphidQuality::update() {
    double intercept = 0.1043,
           slope = 1.7768,
           maxleafN = 3.;

    protein = (Ncontent1 < maxleafN) ? intercept +slope*Ncontent1 : intercept +slope*maxleafN;
}

} //namespace


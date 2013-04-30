/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "general.h"
#include "proportional_control.h"

using namespace UniSim;

namespace vg {
	
ProportionalControl::ProportionalControl(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("actualValue", &actualValue, 20., this, "");
    new Parameter<double>("targetValue", &targetValue, 20., this, "");
    new Parameter<double>("gapMultiplier", &gapMultiplier, 1., this, "");
    new Parameter<double>("pBand", &pBand, 5., this, "");
    new Parameter<double>("maxResponse", &maxResponse, 100., this, "");
    new Parameter<QString>("targetType", &targetTypeString, QString("ceiling"), this, "");
    new Variable<double>("response", &response, this, "");
}

void ProportionalControl::reset() {
    decodeTargetType();
    update();
}

void ProportionalControl::decodeTargetType() {
    QString s = targetTypeString.toLower();
    if (s == "floor")
        targetType = Floor;
    else if (s == "ceiling")
        targetType = Ceiling;
    else {
        QString msg = "Target type must be 'floor' or 'ceiling', not '%1'";
        throw Exception(msg.arg(targetTypeString), this);
    }
}

void ProportionalControl::update() {
    double gap = gapMultiplier*(actualValue - targetValue);
    if (targetType == Ceiling)
        gap = -gap;
    response = propControl(gap, pBand, maxResponse);
}


} //namespace


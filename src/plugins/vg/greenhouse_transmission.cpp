/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>
#include "greenhouse_transmission.h"

using namespace UniSim;

namespace vg {
	
GreenhouseTransmission::GreenhouseTransmission(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(sinb), "calendar[sinb]");
    addParameter<QString>("glassType", &glassTypeStr, QString("Single"), "Glass type: Single, Double or Hortiplus");
    addParameter<double>(Name(diffuse), 0.79, "Transmission of diffuse light through greenhouse construction [0;1]");
    addVariable<double>(Name(direct), "Transmission of direct light through greenhouse construction [0;1]");
}

void GreenhouseTransmission::initialize() {
    defineConstants();
}

void GreenhouseTransmission::defineConstants() {
    int i = int(Single);
    a[i] = 0.844;
    b[i] = 7.39;
    c[i] = -1.66;
    i = int(Double);
    a[i] = 0.818;
    b[i] = 9.77;
    c[i] = -1.73;
    i = int(Hortiplus);
    a[i] = 0.758;
    b[i] = 7.39;
    c[i] = -1.66;
}

void GreenhouseTransmission::reset() {
    decodeGlassType();
    update();
}

void GreenhouseTransmission::decodeGlassType() {
    QString s = glassTypeStr.toLower();
    if (s=="single")
        glassType = Single;
    else if (s=="double")
        glassType = Double;
    else if (s=="hortiplus")
        glassType = Hortiplus;
    else {
        QString msg = "Unknown glass type: '%1'";
        throw Exception(msg.arg(glassTypeStr));
    }
}

void GreenhouseTransmission::update() {
    double angle = asin(sinb)*180./PI;
    int i = int(glassType);
    direct = a[i]/(1. + pow(angle/b[i], c[i]));
    Q_ASSERT(direct>=0. && direct<=1.);
}


} //namespace


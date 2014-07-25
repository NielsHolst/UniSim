/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "publish.h"
#include "leaf_temperature.h"

using namespace UniSim;

namespace vg {

PUBLISH(LeafTemperature)

LeafTemperature::LeafTemperature(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, Tgh, "indoors/temperature[value]");
    InputRef(double, RHgh, "indoors/humidity[rh]");
    InputRef(double, rsH2O, "../rs[rsH2O]");
    InputRef(double, rbH2O, "../rb[rbH2O]");
    InputRef(double, Rna, "../Rna[Rna]");
    Output(double, value);
}

void LeafTemperature::reset() {
}

void LeafTemperature::update() {
    double s = svpSlope(Tgh);
    double psatu = svp(Tgh);
    double pgh = vpFromRh(Tgh, RHgh);
    double Tgh3 = pow((Tgh+T0), 3);
    value = (1/RhoAir/CpAir*(rsH2O+rbH2O)*Rna - 1/Psychr*(psatu-pgh))
            /
            (1+(s/Psychr+ rsH2O/rbH2O+ 1/(RhoAir*CpAir/4/Sigma*Tgh3)*(rsH2O+rbH2O)))
            + Tgh;
}


} //namespace


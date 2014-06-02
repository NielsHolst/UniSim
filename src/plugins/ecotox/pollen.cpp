/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "pollen.h"
#include "publish.h"

using namespace UniSim;


namespace ecotox {

PUBLISH(Pollen)

/*! \class Pollen
 * \brief Pollen deposition
This model must be composed of two models, _depositionRate_ and _lossRate_,
and possibly a third model, _depositionFlush_. Its parameters determine the total deposition
and the distance from the source.

The LogLogistic dose-response model can pull the current dose from _Pollen_ as
_pollenDensity_, _toxinDensity_ or _stdPollenDensity_.
The exchange rate between these different units of density is determined by the parameters
_pollenMass_ and _toxinConc_, which set the mass and toxin concentration for the crop at hand,
and _stdPollenMass_ and _stdToxinConc_, which set the corresponding values for a standard
reference crop. The transformations are
_toxinDensity_ = _pollenDensity_*_pollenMass_*_toxinConc_
and _stdPollenDensity_ = _pollenDensity_*_pollenMass_/_stdPollenMass_*_toxinConc_/_stdToxinConc_
*/

Pollen::Pollen(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<double>(Name(distance), 0.,
                          "Distance to maize field (m)");
    addParameter<double>(Name(distanceExp), -0.548,
                          "Pollen deposition declines with the distance from the maize field. "
                          "This is described as a power function with the exponent @F {distanceExp}");
    addParameter<double>(Name(distanceMin), 0.3,
                          "Minimum distance of pollen dispersion model (m)");
    addParameter<double>(Name(Npeak), 1000.,
                          "Peak average density of pollen deposited on food plant (pollen per cm @Sup {2}) at field edge, i.e. "
                          "at @F {distance <= distanceMin}");
    addParameter<int>(Name(duration), 14.,
                       "Duration of pollen shedding period (whole number of days)");
    addParameter<double>(Name(lossRate), 0.,
                         "Proportional loss of pollen from leaf surface (per day)");
    addParameter<QDate>(Name(onsetDate), QDate(),
                         "Date of onset of pollen shedding");
    addParameter<QDate>(Name(currentDate), QDate(),
                         "Current date of simulation");

    addVariable<double>(Name(depositionRate),
                             "Current pollen deposition rate (per day per cm @Sup {2})");
    addVariable<double>(Name(depositionTotal),
                             "Total pollen deposition (per cm @Sup {2})");
    addVariable<double>(Name(density),
                             "Current pollen density (per cm @Sup {2})");
}

void Pollen::initialize() {
}

void Pollen::reset() {
    depositionRate =
    depositionTotal =
    density = 0.;
    setScaleToPeakAtOne();
}

void Pollen::update() {
    depositionRate = Npeak*scaleToPeakAtOne*parabola(daysIntoShedding())*distanceEffect();
    depositionTotal += depositionRate;
    density *= 1. - lossRate;
    density +=  depositionRate;
}

void Pollen::setScaleToPeakAtOne() {
    double peak = 0.;
    double density = 0.;
    for (int x = 1; x < duration; ++x) {
        density *= 1. - lossRate;
        density += parabola(x);
        if (density > peak) peak = density;
    }
    scaleToPeakAtOne = 1./peak;
}

double Pollen::parabola(int x) {
    return x*(duration - x)*4./duration/duration;
}

int Pollen::daysIntoShedding() {
    int days = currentDate.dayOfYear() - onsetDate.dayOfYear();
    return (days <= 0 || days >= duration) ? 0 : days;
}

double Pollen::distanceEffect() {
    return (distance <= distanceMin) ? 1. : pow(distance, distanceExp)*pow(distanceMin, -distanceExp);
}


} //namespace


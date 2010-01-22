/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <usbase/utilities.h>
#include "../standard_models/stage.h"
#include "pt_reproduction.h"

using namespace UniSim;

namespace grainstore{

PtReproduction::PtReproduction(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    setState("fecundity", &fecundity);
    setState("densityDependence", &_densityDependence);
}

void PtReproduction::initialize()
{
    setParameter("delay", &delay, 5);
    setParameter("sexRatio", &sexRatio, 0.5);
    setParameter("tempMax", &tempMax, 30.);
    setParameter("c1", &c1, 0.005494);
    setParameter("c2", &c2, 0.2359);
    setParameter("ageOpt", &ageOpt, 34.92);
    setParameter("ageDisp", &sexRatio, 26.67);
    setParameter("applyDensityDependence", &applyDensityDependence, false);
    setParameter("rm_a", &rm_a, 0.01261);
    setParameter("rm_b", &rm_b, 0.02001);
    setParameter("rm_opt", &rm_opt, 0.0875);
    setParameter("scale_a", &scale_a, 0.01851);
    setParameter("scale_b", &scale_b, -0.1116);
    setParameter("scale_c", &scale_c, 0.202);

    weather = findOne<Model*>("weather");
    Model *development = findSibling<Model*>("development");
    egg = UniSim::findChild<Model*>("egg", development);
    adult = UniSim::findChild<Stage*>("adult", development);

    // Solve: scale_a*log(x)*log(x) + scale_b*log(x) + scale_c == rm_opt
    double D = scale_b*scale_b - 4.*scale_a*(scale_c - rm_opt);
    if (D < 0)  throw Exception("PtReproduction cannot solve parabolic eq.");
    lowCut = pow(10., -(scale_b + sqrt(D))/2./scale_a);
    // Solve for parabolic low point
    highCut = pow(10., -scale_b/2./scale_a);
    // Value at low point
    scaleHigh = (scale_a*log10(highCut) + scale_b)*log10(highCut) + scale_c;
}

void PtReproduction::reset()
{
    fecundity = 0;
}

void PtReproduction::update() {
    int k = adult->parameter<int>("k");
    double duration = adult->parameter<double>("duration");
    const double *ageClasses = adult->ageClasses();

    temp = weather->state("Tavg");
    if (temp > tempMax)  temp = tempMax;

    double a, b;
    fecundity = 0.;
    for (int i = 0; i < k; i++) {
        a = ageClasses[i];
        b = fecundityAtAge((i + 0.5)*duration/k);
        fecundity += ageClasses[i]*fecundityAtAge((i + 0.5)*duration/k);
    }

    if (applyDensityDependence) {
        _densityDependence = densityDependence(adult->state("number"));
        fecundity *= _densityDependence;
    }
    else {
        _densityDependence = 1.;
    }

    egg->setInput("inflow", fecundity);
}

double PtReproduction::fecundityAtAge(double age) {
    if (age < delay)  return 0.;

    // Multiply by 2 because parameters in P.t. paper is off by that factor
    // (sex ratio corrected for twice)
    return sexRatio*2*extrValue(c1*exp(c2*temp), ageOpt, ageDisp, age);
}

double PtReproduction::extrValue(double a, double b, double c, double x) const
{
    Q_ASSERT(c != 0);
    double k = (x - b)/c;
    return a*exp(-exp(-k) - k + 1.);
}

double PtReproduction::densityDependence(double density) const
{
  double rm;
  if (density < lowCut)
    return 1.;
  else if (density > highCut)
    rm = scaleHigh;
  else
    rm = (scale_a*log10(density) + scale_b)*log10(density) + scale_c;
  return rm_a*exp(rm/rm_b);
}

} //namespace


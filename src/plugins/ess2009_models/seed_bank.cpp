/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include <cmath>
#include <usbase/exception.h>
#include <usbase/utilities.h>
#include "seed_bank.h"

using namespace UniSim;

namespace ess2009 {

SeedBank::SeedBank(UniSim::Identifier name, QObject *parent)
	: Model(name,parent) 
{ 
    setState("number", &_total);
    setState("dormant", &_dormant);
    setState("nonDormant", &_density);
    setState("dailyEmergenceRatio", &_dailyEmergenceRatio);
    setState("totalEmergenceRatio", &_totalEmergenceRatio);
    setState("dailyEmergenceDensity", &_dailyEmergenceDensity);
    setState("totalEmergenceDensity", &_totalEmergenceDensity);
    setState("cropEffectOnEmergence", &_cropEffectOnEmergence);
    setState("dailyEmergenceRatioPotential", &_dailyEmergenceRatioPotantial);
    QString s;
}

void SeedBank::initialize()
{
    setParameter("initialDensity", &_initialDensity, 1000.);
    setParameter("emergenceCalendar", &_emergenceString, QString("(0 0 5 10 20 40 10 5 0 0 0 0)"));
    setParameter("yearlyEmergenceRate", &_yearlyEmergenceRate, 0.20);
    setParameter("yearlyMortalityRate", &_yearlyMortalityRate, 0.10);
    setParameter("cropLaiExp", &_cropLaiExp, 0.04282);

    _calendar = seekOne<Model*>("calendar");
    _rotation = seekOne<Model*>("rotation");

    _dailySurvivalRate = pow(1. - _yearlyMortalityRate, 1./365.);
    decodeEmergence();
    fitEmergence();
}

void SeedBank::reset()
{
    _density = _initialDensity;
    _dailyEmergenceRatio = _totalEmergenceRatio =
    _dailyEmergenceDensity = _totalEmergenceDensity = 0;
    setInput("dormantInflow", 0);
    setInput("instantMortality", 0);
}

void SeedBank::update()
{
    applyInstantMortality();
    addInflow();
    int dayOfYear = int(_calendar->state("dayOfYear"));
    _dailyEmergenceRatioPotantial = lookupEmergence(dayOfYear);
    _cropEffectOnEmergence = cropEffectOnEmergence();
    _dailyEmergenceRatio = _dailyEmergenceRatioPotantial*_cropEffectOnEmergence;
    _dailyEmergenceDensity = _dailyEmergenceRatio*_density;
    _density -= _dailyEmergenceDensity;
    _density *= _dailySurvivalRate;

    if (dayOfYear == 1) {
        _totalEmergenceRatio = _dailyEmergenceRatio;
        _totalEmergenceDensity = _dailyEmergenceDensity;
    }
    else {
        _totalEmergenceRatio += _dailyEmergenceRatio;
        _totalEmergenceDensity += _dailyEmergenceDensity;
    }
    _total = _density + _dormant;
}

void SeedBank::applyInstantMortality() {
    double survival = 1. - input("instantMortality")/100.;
    if (survival < 1.) {
        _density *= survival;
        _dormant *= survival;
        setInput("instantMortality", 0.);
    }
}

void SeedBank::addInflow() {
    _dormant += input("dormantInflow");
    setInput("dormantInflow", 0);

    int dayOfYear = int(_calendar->state("dayOfYear"));
    if (dayOfYear == 1) {
        _density += _dormant;
        _dormant = 0.;
    }
}

double SeedBank::cropEffectOnEmergence() const {
    return exp(-_cropLaiExp*UniSim::pow0(_rotation->state("lai"),2.5));
}

void SeedBank::decodeEmergence() {
    const int m[14] = {-16,15,45,74,105,135,166,196,227,258,288,319,349,380};
    QStringList valList = UniSim::decodeSimpleList(_emergenceString, "Emergence calendar");
    if (valList.size() != 12) throw Exception("Emergence calendar must have space-separated 12 values: " +
                                              _emergenceString);
    bool ok = true;
    for (int i = 0; i < 12 && ok; ++i) {
        _emergenceCalendar[m[i+1]] = valList[i].toDouble(&ok);
    }
    if (!ok) throw Exception("Emergence calendar must hold space-separated numbers only: " +
                             _emergenceString);
    _emergenceCalendar[m[0]] = _emergenceCalendar[m[12]];
    _emergenceCalendar[m[13]] = _emergenceCalendar[m[1]];
}

double SeedBank::lookupEmergence(int dayOfYear) const {
    double emergenceAtDay = interpolate(_emergenceCalendar, dayOfYear);
    return _emergenceScaling*emergenceAtDay;
}

void SeedBank::fitEmergence() {
    double sum = 0.;
    for (QMap<int,double>::iterator em = _emergenceCalendar.begin();
                                    em != _emergenceCalendar.end(); ++em) {
        sum += em.value();
    }
    if (sum == 0.) return;

    _emergenceScaling = 1.;
    scaleEmergence(1./sum);

    double minScale = 1., maxScale = 1.;

    do {
        minScale /= 2.;
        _emergenceScaling = minScale;
    } while (totalEmergenceRatio() > _yearlyEmergenceRate);
    do {
        maxScale *= 2.;
        _emergenceScaling = maxScale;
    } while (totalEmergenceRatio() < _yearlyEmergenceRate);

    int it = 0;

    /*QFile file("C:/data/qdev/UniSim_0_2_2/applications/UniSim_sim/temp/fittest.txt");
    bool fileOk = file.open(QIODevice::WriteOnly | QIODevice::Text);
    Q_ASSERT(fileOk);*/

    double seekValue = _yearlyEmergenceRate;
    const double EPS = 1e-6;
    do {
        double midScale = (minScale + maxScale) / 2.;
        _emergenceScaling = midScale;
        double midValue = totalEmergenceRatio();
        if (fabs(midValue - seekValue) < EPS) break;

        /*_emergenceScaling = minScale;
        double minValue = totalEmergenceRatio();

        _emergenceScaling = maxScale;
        double maxValue = totalEmergenceRatio();

        QString s;
        QTextStream text(&s);
        text << "(" << minValue << " " << midValue << " " << maxValue << ")  "
             << "(" << minScale << " " << midScale << " " << maxScale<< ") \n";*/

        if (seekValue < midValue)
            maxScale = midScale;
        else
            minScale = midScale;

        // file.write(qPrintable(s));
    } while (it++ < 20);
}

void SeedBank::scaleEmergence(double factor) {
    for (QMap<int,double>::iterator em = _emergenceCalendar.begin();
                                    em != _emergenceCalendar.end(); ++em) {
        em.value() *= factor;
    }
}

double SeedBank::totalEmergenceRatio() const {
    double density= 1, total = 0;
    for (int day = 1; day <= 365; ++day) {
        double dailyEmergenceRatio = lookupEmergence(day);
        total += dailyEmergenceRatio;
        density -= dailyEmergenceRatio;
        density *= _dailySurvivalRate;
    }
    return total;
}


}

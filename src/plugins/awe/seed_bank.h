/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef AWE_SEED_BANK_H
#define AWE_SEED_BANK_H

#include <QObject>
#include <usbase/model.h>

namespace awe {

    class SeedBank : public UniSim::Model
{
	//Q_OBJECT
public:
    SeedBank(UniSim::Identifier name, QObject *parent=0);
	
    void initialize();
    void reset();
	void update();
	
private:
    // parameters
    double initialDensity, yearlyEmergenceRate, yearlyMortalityRate, cropLaiExp;
    QString emergenceString;

    // pull variables
    double density, dormant, total,
        dailyEmergenceRatio, totalEmergenceRatio,
        dailyEmergenceDensity, totalEmergenceDensity,
        dailyEmergenceRatioPotential, cropEffectOnEmergence;

    // push variables
    double dormantInflow, instantMortality;


    // decoded parameters
    double dailySurvivalRate, emergenceScaling;
    QMap<int,double> emergenceCalendar;    // indexed by mid-month day of the year from Dec to Jan

    // links
    UniSim::Model *calendar, *rotation;

    // methods
    void decodeEmergence();
    double lookupEmergence(int dayOfYear) const;
    void fitEmergence();
    void scaleEmergence(double factor);
    double calcTotalEmergenceRatio() const;
    double calcCropEffectOnEmergence() const;
    void addInflow();
    void applyInstantMortality();
};


}
#endif

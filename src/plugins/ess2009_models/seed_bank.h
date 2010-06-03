/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ESS2009_SEED_BANK_H
#define ESS2009_SEED_BANK_H

#include <QObject>
#include <usbase/model.h>

namespace ess2009 {

    class SeedBank : public UniSim::Model
{
	Q_OBJECT
public:
    SeedBank(UniSim::Identifier name, QObject *parent=0);
	
    void initialize();
    void reset();
	void update();
	
private:
    // parameters
    double _initialDensity, _yearlyEmergenceRate, _yearlyMortalityRate, _cropLaiExp;
    QString _emergenceString;

    // pull variables
    double _density, _dormant, _total,
        _dailyEmergenceRatio, _totalEmergenceRatio,
        _dailyEmergenceDensity, _totalEmergenceDensity,
        _dailyEmergenceRatioPotential, _cropEffectOnEmergence;

    // push variables
    double dormantInflow, instantMortality;


    // decoded parameters
    double _dailySurvivalRate, _emergenceScaling;
    QMap<int,double> _emergenceCalendar;    // indexed by mid-month day of the year from Dec to Jan

    // links
    UniSim::Model *_calendar, *_rotation;

    // methods
    void decodeEmergence();
    double lookupEmergence(int dayOfYear) const;
    void fitEmergence();
    void scaleEmergence(double factor);
    double totalEmergenceRatio() const;
    double cropEffectOnEmergence() const;
    void addInflow();
    void applyInstantMortality();
};


}
#endif

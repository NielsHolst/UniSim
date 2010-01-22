/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ESS2009_WEED_H
#define ESS2009_WEED_H

#include <QObject>
#include <usbase/model.h>

using UniSim::Model;

namespace ess2009 {

class Rotation;

class Weed : public Model
{
    Q_OBJECT
public:
    Weed(UniSim::Identifier name, QObject *parent=0);
	void initialize();
	void reset();
	void update();

private:
	// parameters
    double _cropLaiExp1, _cropLaiExp2, _seedProdSlope, _seedProdExp, _seedSpillAtHarvestPct;

	// state
    double _prevOutflowTotal, _projectedDeqs, _projectedYieldLossPct, _projectedMass, _seedsDropping;
double _g;
	// links
	Model *_seedBank, *_seedling, *_juvenile, *_mature, *_seedsOnPlant,
		  *_calendar, *_weather;
	Rotation *_rotation;

	// methods
	double cropEffectOnSeedlings() const;
	void projectCompetitionOutcome();
	double projectedDeqs();
    double yieldLossPct(double N) const;
	double proportionDeqsEnteringMaturity() const;
    void kill(Model *stage, double mortalityPct);
    void killAndSpillSeedsOnPlant(double mortalityPct);

private slots:
    void handleEvent(QObject *sender, QString eventName);

};

}
#endif

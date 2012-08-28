/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef AWE_WEED_H
#define AWE_WEED_H

#include <QObject>
#include <usbase/model.h>

using UniSim::Model;

namespace awe {

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
    double c1, c2, seedProdSlope, seedProdExp;

	// state
    double prevOutflowTotal,
           projectedDeqs, projectedYieldLossPct, projectedMass,
           seedsDropping, g;

	// links
    Model *seedBank, *seedling, *juvenile, *mature, *seedsOnPlant,
          *calendar, *weather;
    Rotation *rotation;

	// methods
	double cropEffectOnSeedlings() const;
	void projectCompetitionOutcome();
    double calcProjectedDeqs();
    double yieldLossPct(double N) const;
	double proportionDeqsEnteringMaturity() const;
    void kill(Model *stage, double mortalityPct);

private slots:
    void handleEvent(QObject *sender, QString eventName);

};

}
#endif

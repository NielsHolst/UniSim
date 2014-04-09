/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef SUPPLYDEMAND_LIFE_TABLE
#define SUPPLYDEMAND_LIFE_TABLE
#include <QObject>
#include <usbase/model.h>

namespace SupplyDemand {

class LifeTable : public UniSim::Model
{
	//Q_OBJECT
public: 
    LifeTable(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void initialize();
    void reset();
    void update();

private:
    // methods
    double inflow(UniSim::Models preceding);
    void seekSplitModel();

    // parameters
    QString strPreceding;

    // pull variables
    double individualMass;

    // links
    const double *mass, *number, *split[2];
    UniSim::Model *massModel, *numberModel;
    UniSim::Models precedingMass, precedingNumber;
    bool doSplit;
    int myPosition;
};

} //namespace
#endif

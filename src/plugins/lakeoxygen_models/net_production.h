/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef LAKEOXYGEN_NET_PRODUCTION
#define LAKEOXYGEN_NET_PRODUCTION
#include <QList>
#include <QObject>
#include <usbase/model.h>
#include <usbase/pull_variable.h>
#include <math.h>

namespace lakeoxygen {

class LakeEnvironment;

class NetProduction : public UniSim::Model
{
	Q_OBJECT
public: 
    NetProduction(UniSim::Identifier name, QObject *parent=0);
	// standard methods
    void ammend();
    void initialize();
	void reset();
	void update();

private:

    // parameters
    double K, unit, Ksys, maxDepth;
    int layers;

    // pull variables
    double doObsSatPct, netProd, netProdWind,
        Kwind, Ku10,
        sumStochastic, sumWind,
        layerHeight, light;

    // links
    UniSim::Model *weather, *calendar, *oxygenConcentration;
    QList<LakeEnvironment*> lakeEnvironments;

    // for Layers
    void CreateLayers();
    void addOutput(int layer, UniSim::PullVariable<double> *light);
};

} //namespace
#endif

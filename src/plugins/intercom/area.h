/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_AREA
#define INTERCOM_AREA
#include <QFile>
#include <QList>
#include <QMap>
#include <QObject>
#include <usbase/model.h>

namespace intercom{

class AreaDensity;

class Area : public UniSim::Model
{
	//Q_OBJECT
public: 
    enum KindsOfLight {Diffuse, DirectDirect, DirectTotal};
    enum Rates {Absorption, Assimilation};

    Area(UniSim::Identifier name, QObject *parent=0);

	//standard methods
	void initialize();
	void reset();
    void update();
    void cleanup();

    // special methods
    void setPoint(int hourPoint, int heightPoint);
    const double * calcELAI();
    void updatePhotosynthesis(const double *sumELAI);


private:
    // parameters
    double scatteringCoeff, kDiffuse;
    bool writeTestOutput;

	// state
    int hourPoint, heightPoint;
    double dayLength, sinb, plantHeight,
        height, k[3], reflection[3];
    QFile test;

    // pull variables
    double value, lai, photosynthesisPerDay[2], grossProduction;

    // push variables
    double allocation;

    // links
    UniSim::Model *calendar, *weather, *plant, *plantHeightModel,
        *area, *specificLeafArea, *lightUseEfficiency, *assimilationMax;
    AreaDensity *density;

    // methods
    void updateLai();
    void updateExtensionCoeff();
    void updateReflection();
    double assimilation(double absorption) const;

};

} //namespace
#endif

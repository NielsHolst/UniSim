/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_AREA
#define INTERCOM_AREA
#include <QList>
#include <QMap>
#include <QObject>
#include <usbase/model.h>
#include "light_components.h"
#include "photosynthetic_rate.h"

namespace intercom{

class Area : public UniSim::Model
{
	Q_OBJECT
public: 
    typedef enum {Symmetric, Even, Tapering, TopHeavy} Distribution;

    Area(UniSim::Identifier name, QObject *parent=0);

	//standard methods
	void initialize();
	void reset();

    // special methods
    LightComponents calcEffectiveAreaAbove(double height);
    PhotosyntheticRate calcPhotosynthesis(LightComponents eaa);
    PhotosyntheticRate calcPhotosynthesisInShade(LightComponents eaa);
    PhotosyntheticRate calcPhotosynthesisInSun(PhotosyntheticRate psInShade);
    PhotosyntheticRate calcPhotosynthesisTotal(LightComponents eaa, PhotosyntheticRate psInShade, PhotosyntheticRate psInSun);

    LightComponents calcAbsorptionInShade(LightComponents eaa);

    LightComponents calc_k() const;

    Distribution distribution() const;
    double atHeight(double height) const;
    double aboveHeight(double height) const;

private:
    // input management
    QMap<UniSim::Identifier, Distribution> lookupDist;

    // parameters
    QString distText;
    double initial, scatteringCoeff, kDiffuse;

	// state
    double lai;

    // links
    UniSim::Model *calendar, *weather, *plant, *plantHeight, *lightUseEfficiency, *assimilationMax;

    // methods
    double netAbsorption(const LightComponents &absorbed) const;
    double assimilationRate(double absorption) const;
};

} //namespace
#endif

/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef GRAINSTORE_MAIZE
#define GRAINSTORE_MAIZE
#include <QObject>
#include <usbase/model.h>

namespace grainstore{

class Maize : public UniSim::Model
{
	Q_OBJECT
public: 
	Maize(UniSim::Identifier name, QObject *parent=0);
    // standard methods
	void initialize();
	void reset();
	void update();

private:
    typedef enum{LossPct, DamagePct, NumInjuryTypes} InjuryType;
    QString paramText[NumInjuryTypes];

    // parameters
    double backgroundRate[NumInjuryTypes];

	// state
    double injury[NumInjuryTypes], ptSD[NumInjuryTypes];

    // models
    UniSim::Model *ptAdult;

    // methods
    void updateInjury(InjuryType type);
};

} //namespace
#endif

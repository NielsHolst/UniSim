/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ANIMALSTOCKING_GRASSPRODUCTIVITY
#define ANIMALSTOCKING_GRASSPRODUCTIVITY
#include <QObject>
#include <usbase/model.h>

namespace AnimalStocking {

class GrassProductivity : public UniSim::Model
{
	Q_OBJECT
public: 
	GrassProductivity(UniSim::Identifier name, QObject *parent=0);
    // standard methods
    void initialize();
    void reset();
    void update();

    // special methods
    virtual double calculateDryMatter() = 0;

protected:
    // links
    UniSim::Model *environment;

private:
    // parameter
    double a,b;

	// pull variables
    double dryMatter, energy;
};

} //namespace
#endif

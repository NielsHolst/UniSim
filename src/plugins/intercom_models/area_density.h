/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_AREA_DENSITY
#define INTERCOM_AREA_DENSITY
#include <QList>
#include <QMap>
#include <QObject>
#include <usbase/model.h>

namespace intercom{

class AreaDensity : public UniSim::Model
{
	Q_OBJECT
public: 

    AreaDensity(UniSim::Identifier name, QObject *parent=0);

	//standard methods
	void initialize();
	void reset();
    void update();

    // special methods
    virtual double at(double height) const = 0;
    virtual double above(double height) const = 0;

protected:
    // state
    double plantHeight;

private:
    // links
    UniSim::Model *plantHeightModel;

};

} //namespace
#endif

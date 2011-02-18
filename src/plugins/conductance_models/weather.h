/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef CONDUCTANCE_WEATHER
#define CONDUCTANCE_WEATHER
#include <QObject>
#include <usbase/model.h>

namespace conductance {

class Weather : public UniSim::Model
{
	Q_OBJECT
public: 
	Weather(UniSim::Identifier name, QObject *parent=0);
    // standard methods
    void reset();
    void update();
private:
    // parameter / pull variable
    double irradiation;
};

} //namespace
#endif

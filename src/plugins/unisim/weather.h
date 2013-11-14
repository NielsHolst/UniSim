/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_WEATHER_H
#define UNISIM_WEATHER_H
#include <usbase/model.h>

namespace UniSim {

    class Weather : public Model
{
	//Q_OBJECT
public: 
    Weather(Identifier name, QObject *parent=0);
    // standard methods
    void initialize();
    void reset();
    void update();

private:
    // pull variables (in addition to columns from records file)
    double Tavg;

    // links
    const double *ptrAvg, *ptrMin, *ptrMax;
    bool avgIsPresent, minMaxIsPresent;

};

} //namespace
#endif

/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_SCHEDULED
#define UNISIM_SCHEDULED
#include <QMap>
#include <usbase/model.h>

namespace UniSim{

class Scheduled : public Model
{
	//Q_OBJECT
public:
    Scheduled(Identifier name, QObject *parent=0);
    //standard methods
    void initialize();
    void reset();
    void update();
private:
    // parameters
    QString scheduleAsString;
    QMap<double,double> schedule;
    // state
    double value, increment;
    // links
    UniSim::Model *time;
    // methods
    double interpolateValue(double t);
};

} //namespace


#endif

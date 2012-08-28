/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_INSECT_H
#define UNISIM_INSECT_H

#include <QObject>
#include <QMap>
#include <usbase/model.h>

namespace UniSim{

class InsectLifeCycle;
class Stage;

class Inachis : public Model
{
	Q_OBJECT
public:
    Inachis(Identifier name, QObject *parent=0);
    // standard methods
    void initialize();
    void reset();
    void update();
private:
    // parameters
    double sexRatio, eggProduction;
    // pull variables
    double newAdults, lostAdults;
    // links
    Stage *hibernatingAdult;
    QList<InsectLifeCycle*> generations;
    // methods
    double reproduction(double adults);
};

} //namespace


#endif

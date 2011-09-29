/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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

class Stage;

class Insect : public Model
{
	Q_OBJECT
public:
    Insect(Identifier name, QObject *parent=0);
    // standard methods
    void initialize();
    void reset();
    void update();
private:
    // parameters
    int generations;
    double sexRatio, eggProduction;
    // pull variables
    double newAdults, lostAdults;
    // links
    Model *eggLayingPhase, *hibernatingAdult;
    QList<Stage*> stages;
    // methods
    void reproduce();
};

} //namespace


#endif

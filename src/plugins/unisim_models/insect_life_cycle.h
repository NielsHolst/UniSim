/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_INSECT_LIFE_CYCLE_H
#define UNISIM_INSECT_LIFE_CYCLE_H

#include <QObject>
#include <QMap>
#include <usbase/model.h>

namespace UniSim{

class Stage;

class InsectLifeCycle : public Model
{
	Q_OBJECT
public:
    InsectLifeCycle(Identifier name, QObject *parent=0);
    // standard methods
    void initialize();
    void reset();
    void update();
    // special methods
    QList<Stage*>& stages();
private:
    // pull variables
    double eclosedReproductiveAdults, adultsToHibernation, deadAdults;
    // links
    QList<Stage*> _stages;
//    const double *pollen;
    // methods
    double updateStage(Stage *stage);
};

} //namespace


#endif

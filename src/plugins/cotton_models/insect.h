/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef COTTON_INSECT
#define COTTON_INSECT
#include <QDate>
#include <QObject>
#include <usbase/model.h>

namespace UniSim {
    class Stage;
}

namespace cotton{

class Insect : public UniSim::Model
{
	Q_OBJECT
public: 
    Insect(UniSim::Identifier name, QObject *parent=0);
	// standard methods
	void initialize();
    void reset();
	void update();

private:
    // methods
    bool isEgglayingDate();

	// parameters
    double initEggs;
    QDate initEgglayingDate;

    // pull variables
    bool eggsLaid;

    // links
    Model *runIterator, *calendar, *egg;
    QList<UniSim::Stage*> stages;
};

} //namespace
#endif

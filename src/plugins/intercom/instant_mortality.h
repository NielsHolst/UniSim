/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_INSTANT_MORTALITY
#define INTERCOM_INSTANT_MORTALITY
#include <QMap>
#include <QObject>
#include <usbase/model.h>

namespace intercom{

class InstantMortality : public UniSim::Model
{
	//Q_OBJECT
public: 
    InstantMortality(UniSim::Identifier name, QObject *parent=0);
	//standard methods
	void initialize();
	void update();

private:
	// parameters
    int day, month;
    QString survivalAsString;

    // models
    UniSim::Model *calendar;
    class Victim {
    public:
        Victim(UniSim::Model *model, double mortality);
        void push();
    private:
        UniSim::Model *model;
        double mortality;
    };
    QList<Victim> victims;

};

} //namespace
#endif

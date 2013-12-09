/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef IPM_G1_LOSS_RATE_H
#define IPM_G1_LOSS_RATE_H

#include <QObject>
#include <usbase/model.h>

namespace ipm {

class G1LossRate : public UniSim::Model
{
	//Q_OBJECT
public:
    G1LossRate(UniSim::Identifier name, QObject *parent=0);
    void initialize();
	void reset();
    void update();
	
private:
    //parameters
    int day, month, year;
	double initialValue, growthRate, value, scale, scaledValue;

    // decoded parameters;
    int dayOfYear;

    // links
    UniSim::Model *calendar;
};


}
#endif

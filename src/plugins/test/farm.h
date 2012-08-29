/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_TEST_FARM
#define UNISIM_TEST_FARM
#include <QObject>
#include <usbase/model.h>

namespace test{

class Farm : public UniSim::Model
{
	//Q_OBJECT
public: 
    Farm(UniSim::Identifier name, QObject *parent=0);
	//standard methods

private:
    // parameters
    int realId, leakageRate;
    QString farmType, soilType, economicType;
};

} //namespace
#endif

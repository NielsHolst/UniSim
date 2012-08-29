/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_TEST_OPERATION
#define UNISIM_TEST_OPERATION
#include <QDate>
#include <QObject>
#include <usbase/model.h>

namespace test{

class Operation : public UniSim::Model
{
	//Q_OBJECT
public: 
    Operation(UniSim::Identifier name, QObject *parent=0);
	//standard methods
    void reset();
    void update();

private:
    // parameters
    QDate date;
    int cost;
    bool isOrganic;
    // pull variables
    int energy, labour, CO2;
};

} //namespace
#endif

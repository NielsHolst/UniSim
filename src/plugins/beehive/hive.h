/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef BEEHIVE_HIVE
#define BEEHIVE_HIVE
#include <QObject>
#include <usbase/model.h>

namespace beehive {

class Hive : public UniSim::Model
{
	//Q_OBJECT
public: 
    Hive(UniSim::Identifier name, QObject *parent=0);
	// standard methods

private:
	// parameters
    double x, y;
};

} //namespace
#endif

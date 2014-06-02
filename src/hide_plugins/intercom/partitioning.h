/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INTERCOM_PARTITIONING
#define INTERCOM_PARTITIONING
#include <QObject>
#include <usbase/model.h>

namespace intercom{

class Partitioning : public UniSim::Model
{
	//Q_OBJECT
public: 
    Partitioning(UniSim::Identifier name, QObject *parent=0);

protected:
    // pull variables
    double value;
};

} //namespace
#endif

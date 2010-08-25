/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_RANDOM_BASE
#define UNISIM_RANDOM_BASE
#include <QObject>
#include <usbase/model.h>

namespace UniSim{

class RandomBase : public Model
{
	Q_OBJECT
public: 
    RandomBase(Identifier name, QObject *parent=0);
protected:
    // parameters
    double minValue, maxValue;
    // pull variables
    double value;
};

} //namespace
#endif

/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_COUNTER_H
#define UNISIM_COUNTER_H
#include <QObject>
#include <usbase/model.h>

namespace UniSim{

class Counter : public Model
{
	Q_OBJECT
public: 
    Counter(UniSim::Identifier name, QObject *parent=0);
    // standard methods
    void reset();
    void update();
private:
    // parameters
    int initValue;
    // pull variables
    int value;
};

} //namespace
#endif

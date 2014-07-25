/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_AVERAGE_H
#define UNISIM_AVERAGE_H

#include <usbase/model.h>

namespace UniSim{

class Average : public UniSim::Model
{
public: 
    Average(UniSim::Identifier name, QObject *parent=0);
    // standard methods
    void initialize();
    void reset();
    void update();
private:
    QString inputs;
    double value;
    QVector<const VariableBase*> values;
};

} //namespace
#endif

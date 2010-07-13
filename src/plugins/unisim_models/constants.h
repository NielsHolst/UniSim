/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_CONSTANTS
#define UNISIM_CONSTANTS
#include <QObject>
#include <usbase/model.h>

namespace UniSim{

class Constants : public UniSim::Model
{
	Q_OBJECT
public: 
    Constants(UniSim::Identifier name, QObject *parent=0);
    ~Constants();
    // standard methods
    void initialize();
private:
    // data
    QList<double*> valuePtrs;
    // methods
    void addPullVariable(Identifier id, double value);
};

} //namespace
#endif

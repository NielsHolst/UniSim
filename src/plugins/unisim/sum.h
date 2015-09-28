/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_SUM_S
#define UNISIM_SUM_S
#include <QObject>
#include <usbase/model.h>


namespace UniSim{

class Sum : public UniSim::Model
{
	//Q_OBJECT
public: 
    Sum(UniSim::Identifier name, QObject *parent=0);
    // standard methods
    void initialize();
    void reset();
    void update();
private:
    // Parameters
    QString toAdd;

    // Variables
    double value;

    // Links
    QList<VariableBase*> values;
};

} //namespace
#endif

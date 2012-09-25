/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_STAGE_DEMAND_H
#define UNISIM_STAGE_DEMAND_H

#include <QObject>
#include <QMap>
#include <usbase/model.h>

namespace UniSim{

class Stage;

class StageDemand : public Model
{
	//Q_OBJECT
public:
    StageDemand(Identifier name, QObject *parent=0);
    // standard methods
    void initialize();
    void reset();
    void update();
private:
    // parameters
    QString stageName;
    // pull variables
    double value;
    //links
    Stage *stage;
};

} //namespace

#endif

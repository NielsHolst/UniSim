/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef AWE_INSTANT_MORTALITY_H
#define AWE_INSTANT_MORTALITY_H

#include <QObject>
#include <usbase/model.h>

namespace awe {

class PlantGrowthStage;

class InstantMortality : public UniSim::Model
{
	Q_OBJECT
public:
    InstantMortality(UniSim::Identifier name, QObject *parent=0);
    void initialize();
    void update();
	
private:
    //parameters
    int day, month;
    QString mortalityString;

    // decoded parameters;
    int dayOfYear;

    // links
    struct TargetMortality {
        QList<PlantGrowthStage*> targets;
        double value;
    };
    QList<TargetMortality> targetMortalities;
    UniSim::Model *calendar;

    //methods
    void decodeMortalities();
    void chopRightParenthesis(QString &s) const;

};


}
#endif

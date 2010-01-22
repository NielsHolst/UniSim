/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef ESS2009_INSTANT_MORTALITY_H
#define ESS2009_INSTANT_MORTALITY_H

#include <QObject>
#include <usbase/model.h>

namespace ess2009 {

class InstantMortality : public UniSim::Model
{
	Q_OBJECT
public:
    InstantMortality(UniSim::Identifier name, QObject *parent=0);
    void initialize();
    void update();
	
private:
    //parameters
    int _day, _month;
    QString _mortalityString;

    // decoded parameters;
    int _dayOfYear;

    // links
    struct TargetMortality {
        UniSim::Model *target;
        double value;
    };
    QList<TargetMortality> _targetMortalities;
    UniSim::Model *_calendar;

    //methods
    void decodeMortalities();
    void chopRightParenthesis(QString &s) const;

};


}
#endif

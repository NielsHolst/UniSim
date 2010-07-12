/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef AWE_CROP_H
#define AWE_CROP_H

#include <QMap>
#include <QObject>
#include <usbase/model.h>

namespace awe {

class Crop : public UniSim::Model
{
    Q_OBJECT
public:
    Crop(UniSim::Identifier name, QObject *parent=0);

    void initialize();
    void reset();
    void update();

private:
    // parameters
    int sowingDay, sowingMonth, harvestDay, harvestMonth;
    double weedExchangeRate, maxYield,
            maxYieldLossPct, slopeYieldLossPct;
    QString laiCalendar;

    // state
    bool isGrowing;
    double lai, Tsum;

    // decoded parameters
    int sowingDayOfYear, harvestDayOfYear;
    QMap<int, double> laiByTSum;

    // links
    UniSim::Model *weather, *calendar;

    // methods
    void decodeLai();
    double lookupLai() const;
    void chopRightParenthesis(QString &s) const;
};

}
#endif

/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef CROP_H
#define CROP_H

#include <QMap>
#include <QObject>
#include <usbase/model.h>

namespace ess2009 {

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
    int _sowingDay, _sowingMonth, _harvestDay, _harvestMonth;
    double _weedExchangeRate, _maxYield,
            _maxYieldLossPct, _slopeYieldLossPct;
    QString _laiCalendar;

    // state
    bool _isGrowing;
    double _lai, _Tsum;

    // decoded parameters
    int _sowingDayOfYear, _harvestDayOfYear;
    QMap<int, double> _laiByTSum;

    // links
    UniSim::Model *_weather, *_calendar;

    // methods
    void decodeLai();
    double lookupLai() const;
    void chopRightParenthesis(QString &s) const;
};

}
#endif

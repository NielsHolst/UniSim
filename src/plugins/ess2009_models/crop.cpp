/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <usbase/exception.h>
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "crop.h"
#include "weather.h"

using namespace UniSim;

namespace ess2009 {

Crop::Crop(UniSim::Identifier name, QObject *parent)
    : Model(name,parent)
{
    new PullVariable("lai", &_lai, this);
    new PullVariable("Tsum", &_Tsum, this);
}



void Crop::initialize() {
    setParameter("sowingDay", &_sowingDay, 1);
    setParameter("sowingMonth", &_sowingMonth, 4);
    setParameter("harvestDay", &_harvestDay, 1);
    setParameter("harvestMonth", &_harvestMonth, 8);
    setParameter("weedExchangeRate", &_weedExchangeRate, 0.6);
    setParameter("maxYield", &_maxYield, 800.);
    setParameter("maxYieldLossPct", &_maxYieldLossPct, 60.);
    setParameter("slopeYieldLossPct", &_slopeYieldLossPct, 0.5);
    setParameter("laiCalendar", &_laiCalendar,
                 QString("((0 0)(110 0)(210 0.3)(310 0.8)(410 1.6)"
                         "(510 2.9)(610 5)(1000 5)(1200 5)(1650 2))"));

    _sowingDayOfYear = UniSim::toDayOfYear(_sowingDay, _sowingMonth);
    _harvestDayOfYear = UniSim::toDayOfYear(_harvestDay, _harvestMonth);

    decodeLai();
    //_weather = findOneModelByType<Weather*>(this);
    //_calendar = findOneModelByType<Calendar*>(this);

    _weather = seekOne<Model*>("weather");
    _calendar = seekOne<Model*>("calendar");
}

void Crop::reset() {
    _isGrowing = false;
    _lai = _Tsum = 0.;
}

void Crop::update() {
    int dayOfYear = int(_calendar->pullVariable("dayOfYear"));

    if (_isGrowing) {
        _Tsum += _weather->pullVariable("T");
        _lai = lookupLai();
        if (dayOfYear == _harvestDayOfYear) {
            _isGrowing = false;
            emit event(this, "harvest");
        }
    }

    if (!_isGrowing) {
        _Tsum = _lai = 0;
        if (dayOfYear == _sowingDayOfYear) {
            _isGrowing = true;
            emit event(this, "sowing");
        }
    }

}

void Crop::decodeLai() {
    QString s = _laiCalendar.simplified();
    if (s.size() == 0) throw UniSim::Exception("LAI calendar is empty: " + s);
    if (s.left(1) != "(") throw UniSim::Exception("LAI calendar must begin with '(': " + s);

    QStringList parts = s.split("(");
    for (int i = 0; i < 2; ++i) {
        if (parts[i].size() > 0 && parts[i].left(1) != " ")
            throw UniSim::Exception("LAI calendar must begin with two left parentheses: " + s);
    }

    if (parts.size() < 3) throw UniSim::Exception("LAI calendar is incomplete: " + s);

    for (int i = 2; i< parts.size(); ++i) {
        QString part = parts[i];
        chopRightParenthesis(part);
        if (i == parts.size() - 1)
            chopRightParenthesis(part);

        QStringList pair = part.split(" ");
        bool ok = pair.size() == 2;
        if (ok) {
            bool ok0, ok1;
            int tSum = pair[0].toInt(&ok0);
            double lai = pair[1].toDouble(&ok1);
            ok = ok0 && ok1;
            if (ok) _laiByTSum[tSum] = lai;
        }

        if (!ok)
            throw UniSim::Exception("LAI calendar must contain pairs of numbers: (integer decimal) "
                                    + parts[i]);
    }
}

void Crop::chopRightParenthesis(QString &s) const {
    if (s.right(1) != ")")
        throw UniSim::Exception("LAI calendar miss right parenthesis: (" + s);
    s.chop(1);
    s = s.simplified();
}

double Crop::lookupLai() const {
    return interpolate(_laiByTSum, int(_Tsum));
}

}

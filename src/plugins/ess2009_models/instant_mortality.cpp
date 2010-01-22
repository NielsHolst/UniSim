/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <usbase/exception.h>
#include <usbase/utilities.h>
#include "instant_mortality.h"

using namespace UniSim;

namespace ess2009 {

InstantMortality::InstantMortality(Identifier name, QObject *parent)
	: Model(name,parent) 
{ 
}

void InstantMortality::initialize() {
    setParameter("day", &_day, 1);
    setParameter("month", &_month, 6);
    setParameter("mortalities", &_mortalityString, QString());

    _dayOfYear = UniSim::toDayOfYear(_day, _month);
    decodeMortalities();

    _calendar = findOne<Model*>("calendar");
}

void InstantMortality::update() {
    int today = int(_calendar->state("dayOfYear"));
    if (today == _dayOfYear) {
        for (int i = 0; i < _targetMortalities.size(); ++i) {
            TargetMortality mortality = _targetMortalities[i];
            mortality.target->setInput("instantMortality", mortality.value);
        }
    }
}

void InstantMortality::decodeMortalities() {
    QString s = _mortalityString.simplified();
    if (s.size() == 0) return;
    if (s.left(1) != "(") throw UniSim::Exception("Mortalities list must begin with '(': " + s);

    QStringList parts = s.split("(");
    for (int i = 0; i < 2; ++i) {
        if (parts[i].size() > 0 && parts[i].left(1) != " ")
            throw UniSim::Exception("Mortalities list must begin with two left parentheses: " + s);
    }

    if (parts.size() < 3) throw UniSim::Exception("Mortalities list is incomplete: " + s);

    for (int i = 2; i< parts.size(); ++i) {
        QString part = parts[i];
        chopRightParenthesis(part);
        if (i == parts.size() - 1)
            chopRightParenthesis(part);

        QStringList pair = part.split(" ");
        bool ok = pair.size() == 2;
        if (ok) {
            Model *target = findOne<Model*>("weed/" + pair[0]);
            double value = pair[1].toDouble(&ok);
            if (ok) {
                TargetMortality tm = {target, value};
                _targetMortalities.append(tm);
            }
        }

        if (!ok)
            throw UniSim::Exception("LAI calendar must contain pairs of numbers: (integer decimal) "
                                    + parts[i]);
    }
}

void InstantMortality::chopRightParenthesis(QString &s) const {
    if (s.right(1) != ")")
        throw UniSim::Exception("Mortalities list miss right parenthesis: (" + s);
    s.chop(1);
    s = s.simplified();
}


}

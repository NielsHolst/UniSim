/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <usbase/exception.h>
#include <usbase/utilities.h>
#include "instant_mortality.h"
#include "plant_growth_stage.h"

using namespace UniSim;

namespace awe {

InstantMortality::InstantMortality(Identifier name, QObject *parent)
	: Model(name,parent) 
{ 
}

void InstantMortality::initialize() {
    setParameter("day", &day, 1, "Day in month of imposed mortality");
    setParameter("month", &month, 6, "Month of imposed mortality");
    setParameter("mortalities", &mortalityString, QString(),
                 "List of instant mortalities (0-100%) applied to the different weed growth stages. "
                 "Defined as a list of (@I {growth stage mortality}) pairs. For example, @F "
                 "{((seedling 100) (juvenile 80) (mature 10))}, which will apply the respective mortalities "
                 "to all @F PlantGrowhtStage models named "
                 "@F {seedling}, @F juvenile and @F {mature}.");

    dayOfYear = UniSim::toDayOfYear(day, month);
    decodeMortalities();

    calendar = seekOne<Model*>("calendar");
}

void InstantMortality::update() {
    int today = int(calendar->pullVariable("dayOfYear"));
    if (today == dayOfYear) {
        for (int i = 0; i < targetMortalities.size(); ++i) {
            TargetMortality mortality = targetMortalities[i];
            int n = mortality.targets.size();
            for (int j = 0; j < n; ++j)
                mortality.targets[j]->pushVariable("instantMortality", mortality.value);
        }
    }
}

void InstantMortality::decodeMortalities() {
    QString s = mortalityString.simplified();
    if (s.size() == 0) return;
    if (s.left(1) != "(") throw Exception("Mortalities list must begin with '(': " + s);

    QStringList parts = s.split("(");
    for (int i = 0; i < 2; ++i) {
        if (parts[i].size() > 0 && parts[i].left(1) != " ")
            throw Exception("Mortalities list must begin with two left parentheses: " + s);
    }

    if (parts.size() < 3) throw Exception("Mortalities list is incomplete: " + s);

    for (int i = 2; i< parts.size(); ++i) {
        QString part = parts[i];
        chopRightParenthesis(part);
        if (i == parts.size() - 1)
            chopRightParenthesis(part);

        QStringList pair = part.split(" ");
        bool ok = pair.size() == 2;
        if (ok) {
            TargetMortality tm;
            tm.targets = seekMany<PlantGrowthStage*>(pair[0]);
            if (tm.targets.size() == 0)
                throw Exception("Could not find target model for InstantMortality: " + parts[i]);
            tm.value = pair[1].toDouble(&ok);
            if (ok)
                targetMortalities.append(tm);
        }

        if (!ok)
            throw Exception("LAI calendar must contain pairs of numbers: (integer decimal) "
                            + parts[i]);
    }
}

void InstantMortality::chopRightParenthesis(QString &s) const {
    if (s.right(1) != ")")
        throw Exception("Mortalities list miss right parenthesis: (" + s);
    s.chop(1);
    s = s.simplified();
}


}

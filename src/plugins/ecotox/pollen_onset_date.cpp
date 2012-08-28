/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <boost/random/uniform_01.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <QMessageBox>
#include <usbase/data_grid.h>
#include <usbase/model.h>
#include <usbase/random.h>
#include <usbase/utilities.h>
#include <usengine/simulation.h>
#include "pollen_onset_date.h"

using namespace UniSim;

namespace ecotox {

//
// PollenOnsetDateFromFile
//

PollenOnsetDateFromFile::PollenOnsetDateFromFile(QString fileName)
    : PollenOnsetDate(), data(simulation()->inputFilePath(fileName))
{
    DataGrid data(simulation()->inputFilePath(fileName));
    for (int i = 0; i < data.rowNumber(); ++i) {
        QDate date = stringToValue<QDate>(data.row(i).at(1));
        int year = date.year();
        if (!yearsFirstLine.contains(year)) {
            yearsFirstLine[year] = i;
        }
    }
    QList<int> years = yearsFirstLine.keys();
    lastYear = years.last();
}

QDate PollenOnsetDateFromFile::calculate() {
    Model *calendar = seekOne<Model*>("calendar");
    QDate date = calendar->pullValue<QDate>("date");
    int year = date.year();
    if (date.month() == 12)
        ++year;
    int ixMin = yearsFirstLine.value(year);
    int ixMax = (year == lastYear) ? (data.rowNumber() - 1) : (yearsFirstLine.value(year+1) - 1);
    Q_ASSERT(ixMin < ixMax);

    typedef boost::random::uniform_int_distribution<> Distribution;
    typedef boost::variate_generator<UniSim::Random::Generator&, Distribution> Variate;
    Distribution distribution(ixMin, ixMax);
    Variate variate(*randomGenerator(), distribution);

    int index = variate();
    Q_ASSERT(index>=0 && index<data.rowNumber());
    QDate onset = stringToValue<QDate>(data.row(index).at(1));
    Q_ASSERT(onset.year() == year);
    return onset;
}

//
// PollenOnsetDateFromCurve
//

PollenOnsetDateFromCurve::PollenOnsetDateFromCurve(Model *model_)
    : PollenOnsetDate(), model(model_)
{
}

QDate PollenOnsetDateFromCurve::calculate() {
    typedef boost::uniform_01<double>  Distribution;
    typedef boost::variate_generator<UniSim::Random::Generator&, Distribution> Variate;
    Distribution distribution;
    Variate variate(*randomGenerator(), distribution);

    double fractile = variate();
    Q_ASSERT(fractile >= 0. && fractile < 1.);

    Model *calendar = seekOne<Model*>("calendar");
    Model *weather = seekOne<Model*>("weather");

    calendar->deepReset();
    weather->deepCleanup();
    weather->deepReset();
    model->deepReset();

    const double *curveTotal = model->pullValuePtr<double>("total");
    int day = 0;
    while (*curveTotal < fractile && day < 365) {
        calendar->update();
        weather->update();
        model->update();
        ++day;
    }
    Q_ASSERT(day < 365);

    calendar->deepReset();
    weather->deepCleanup();
    weather->deepReset();
    model->deepReset();

    QDate firstDate = calendar->pullValue<QDate>("initialDate");
    return firstDate.addDays(day);
}

} //namespace


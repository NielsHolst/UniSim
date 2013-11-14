/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <boost/random/uniform_01.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <usbase/data_grid.h>
#include <usbase/random.h>
#include <usengine/simulation.h>
#include "pollen_onset_date_from_phenology.h"

using namespace UniSim;

namespace ecotox {

//
// PollenOnsetDateFromFile
//

PollenOnsetDateFromPhenology::PollenOnsetDateFromPhenology(QString fileName)
    : Model(name, parent), data(0)
{
    addParameter<QString>(Name(fileName), 0., "File with observations");
    addVariable<QDate>(Name(value), "Randomly picked date of pollen deposition onset");
}

PollenOnsetDateFromPhenology::~PollenOnsetDateFromPhenology() {
    delete data;
}

void PollenOnsetDateFromPhenology::amend() {
    data = new DataGrid (simulation()->inputFilePath(fileName));
    for (int i = 0; i < data.rowNumber(); ++i) {
        QDate date = stringToValue<QDate>(data.row(i).at(1));
        int year = date.year();
        if (!yearsFirstRow.contains(year)) {
            yearsFirstRow[year] = i;
        }
    }
    QList<int> years = yearsFirstRow.keys();
}

void PollenOnsetDateFromPhenology::initialize() {
    Model *calendar = seekOne<Model*>("calendar");
    date = calendar->pullValuePtr<QDate>("date");
}


void PollenOnsetDateFromPhenology::reset() {
    int year = date->year();
    if (date->month() == 12)
        ++year;
    int ixMin = yearsFirstRow.value(year);
    int ixMax = (year == years.last()) ? (data.rowNumber() - 1) : (yearsFirstRow.value(year+1) - 1);
    Q_ASSERT(ixMin < ixMax);

    typedef boost::random::uniform_int_distribution<> Distribution;
    typedef boost::variate_generator<UniSim::Random::Generator&, Distribution> Variate;
    Distribution distribution(ixMin, ixMax);
    Variate variate(*randomGenerator(), distribution);

    int index = variate();
    Q_ASSERT(index>=0 && index<data.rowNumber());
    value = stringToValue<QDate>(data.row(index).at(1));
    Q_ASSERT(value.year() == year);
}

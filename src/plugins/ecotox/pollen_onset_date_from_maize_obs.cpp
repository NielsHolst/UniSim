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
#include "pollen_onset_date_from_maize_obs.h"
#include "publish.h"

using namespace UniSim;

namespace ecotox {

PUBLISH(PollenOnsetDateFromMaizeObs)

/*! \class PollenOnsetDateFromMaizeObs
 * \brief Onset of pollen depositon sampled from maize observations
The onset of pollen deposition is sampled from a file with at least two columns named _Date_ and _Onset_.
A date is picked at random from within the observations of the current year (determined by the Calendar model).)
 */
PollenOnsetDateFromMaizeObs::PollenOnsetDateFromMaizeObs(UniSim::Identifier name, QObject *parent)
    : PollenOnsetDate(name, parent)
{
    addParameter<QString>(Name(obsFileName), QString(),
        "Name of file with maize phenology observations, "
        "a column-oriented text file with two colums, of which the first is ignored "
        "and the second contains the date when start-of-flowering was observed.");
}

void PollenOnsetDateFromMaizeObs::amend() {
    data = new DataGrid (simulation()->inputFilePath(obsFileName), this);
    for (int i = 0; i < data->rowNumber(); ++i) {
        QDate date = stringToValue<QDate>(data->row(i).at(1));
        int year = date.year();
        if (!yearsFirstLine.contains(year)) {
            yearsFirstLine[year] = i;
        }
    }
    QList<int> years = yearsFirstLine.keys();
    lastYear = years.last();
}

void PollenOnsetDateFromMaizeObs::reset() {
    int curYear = currentYear();
    int ixMin = yearsFirstLine.value(curYear);
    int ixMax = (curYear == lastYear) ?
                (data->rowNumber() - 1) :
                (yearsFirstLine.value(curYear+1) - 1);
    if (ixMin >= ixMax) {
        QString msg = "Calendar year (%1) not found in file with maize phenology data";
        throw Exception(msg.arg(curYear), this);
    }
    int index = sampleInterval(ixMin, ixMax);
    Q_ASSERT(index>=0 && index<data->rowNumber());
    date = stringToValue<QDate>(data->row(index).at(1));
    dayOfYear = date.dayOfYear();
    year = date.year();
    Q_ASSERT(date.year() == curYear);
}

int PollenOnsetDateFromMaizeObs::sampleInterval(int from, int to) {
    typedef boost::random::uniform_int_distribution<> Distribution;
    typedef boost::variate_generator<UniSim::Random::Generator&, Distribution> Variate;
    Distribution distribution(from, to);
    Variate variate(*randomGenerator(), distribution);
    return variate();
}


} //namespace


/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <boost/random/uniform_01.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <usbase/data_grid.h>
#include <usbase/exception.h>
#include <usbase/random.h>
#include <usengine/simulation.h>
#include "pollen_onset_date_from_trap_obs.h"

using namespace UniSim;

namespace ecotox {


PollenOnsetDateFromTrapObs::PollenOnsetDateFromTrapObs(UniSim::Identifier name, QObject *parent)
    : PollenOnsetDate(name, parent)
{
    addParameter<QString>(Name(obsFileName), QString(),
        "Name of file with pollen trap catches, "
        "a column-oriented text file at least two colums named @F Date and @F Pollen. "
        "The @F Pollen column must add up to 1 for each year in the file.");
}

void PollenOnsetDateFromTrapObs::reset() {
    UniSim::DataGrid data(simulation()->inputFilePath(obsFileName));
    colDate = findColumn(data, "Date");
    colPollen = findColumn(data, "Pollen");
    int curYear = currentYear();
    double pollenSum = 0,
           fractile = randomFractile();

    for (int i = 0; i < data.rowNumber(); ++i) {
        QDate rowDate = stringToValue<QDate>(data.row(i).at(colDate));
        int rowYear = rowDate.year();
        if (rowYear == curYear) {
            double pollen = stringToValue<double>(data.row(i).at(colPollen));
            pollenSum += pollen;
            if (pollenSum >= randomFractile() ) {
                date = rowDate;
                dayOfYear = rowDate.dayOfYear();
                year = rowDate.year();
                return;
            }
        }
    }
    QString msg = "Could not find pollen fractile (%1) in calendar year (%2)";
    throw Exception(msg.arg(fractile).arg(curYear), this);
}

int PollenOnsetDateFromTrapObs::findColumn(const UniSim::DataGrid &data, QString colName) const {
    const QStringList &colNames(data.columnNames());
    int colIndex = colNames.indexOf( QRegExp(colName, Qt::CaseInsensitive) );
    if (colIndex == -1) {
        QString msg = "Could not find column named '%1' among colums: '%2'";
        throw Exception(msg.arg(colName).arg(colNames.join(',')),
                        const_cast<PollenOnsetDateFromTrapObs*>(this));
    }
    return colIndex;
}

double PollenOnsetDateFromTrapObs::randomFractile() {
    typedef boost::uniform_01<double>  Distribution;
    typedef boost::variate_generator<UniSim::Random::Generator&, Distribution> Variate;
    Distribution distribution;
    Variate variate(*randomGenerator(), distribution);
    double fractile = variate();
    Q_ASSERT(fractile >= 0. && fractile < 1.);
    return fractile;
}

} //namespace


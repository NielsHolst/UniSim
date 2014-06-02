#include <iostream>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <QtAlgorithms>
#include <QTime>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/random.h>
#include <usbase/utilities.h>
#include <usengine/simulation.h>
#include "publish.h"
#include "records.h"

namespace UniSim {

PUBLISH(Records)

Records::Records(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<QString>("fileName", &fileName, "records.txt", this,
    "Name of input file. It may contain a relative path, e.g. @F {observations/experiment1.txt}. "
    "If date and/or time are included, their column titles must be @F date and @F {time}");
    new Parameter<QString>("fileLocation", &fileLocation, "input", this,
    "Valid locations are: @F {input} and @F {plugins}. The former will look first in the same folder where the recipe is reciding, "
    "then in a sub-folder of that called@ F {input}, and then for a sub-folder @F input in parent, grandparent and earlier ancestors."
    "A value of @F plugins refers to the standard folder for plug-ins.");
    new Parameter<bool>("imposeInitialDateTime", &imposeInitialDateTime, true, this,
    "Impose the first date and time on @F {calendar}. Either or both, date and time, are imposed; "
    "it depends on which are included in the file. If neither is included, there is no effect "
    "of setting @F imposeInitialDateTime to @F {yes}" );
    new Parameter<bool>("randomizeInitialYear", &randomizeInitialYear, false, this,
    "Pick a random initial year from the years available in the records file");

    new Variable<QDateTime>("currentDateTime", &currentDateTime, this,
    "The date and time of the current line in the input file. The @F calendar date and time will be at "
    "or past this");
    new Variable<QDateTime>("nextDateTime", &nextDateTime, this,
    "The date and time of the next line in the input file. The @F calendar date and time will be at "
    "or before this");
    new Variable<QDateTime>("firstDateTime", &firstDateTime, this,
    "The date and time of the first line in the input file");
    new Variable<QDateTime>("lastDateTime", &lastDateTime, this,
    "The date and time of the last line in the input file");
    new Variable<QDate>("currentDate", &currentDate, this, "The date part of @F {currentDateTime}");
    new Variable<QDate>("nextDate", &nextDate, this, "The date part of @F {nextDateTime}");
    new Variable<QTime>("currentTime", &currentTime, this, "The time part of @F {currentDateTime}");
    new Variable<QTime>("nextTime", &nextTime, this, "The time part of @F {nextDateTime}");

    currentColumnValues = new QVector<double>;
    nextColumnValues = new QVector<double>;
}

void Records::amend() {
    readColumnNames();
    createColumnPullVariables();
    readFromFirstToLastLine();
}

Records::~Records() {
    delete currentColumnValues;
    delete nextColumnValues;
}

void Records::initialize() {
	calendar = seekOne<Model*>("calendar");
}

void Records::readColumnNames() {
    openFile();
    readLineItems();
    if (pastLastLine)
        throw Exception("Records file is empty", this);

    dateColumn = -1;
    timeColumn = -1;
    for (int i = 0; i < lineItems.size(); ++i) {
        Identifier id = Identifier(lineItems[i]);
        columnNames.append(id);
        if (id.equals("date")) {
            dateColumn = i;
        }
        else if (id.equals("time")) {
            timeColumn = i;
        }
    }
    imposeInitialDateTime = imposeInitialDateTime && (dateColumn > -1 || timeColumn > -1);
    file.close();
}

void Records::openFile() {
    file.setFileName(filePath(fileName));
    bool fileOk = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!fileOk)
        throw Exception("Cannot open records file: " + filePath(fileName), this);
    pastLastLine = false;
}

QString Records::filePath(QString fileName) {
    Identifier id(fileLocation);
    if (id.equals("input"))
        return simulation()->inputFilePath(fileName);
    if (id.equals("plugins")) {
        QDir folder = FileLocations::location(FileLocationInfo::Plugins);
        return folder.absolutePath() + "/" + fileName;
    }
    QString msg("File location must be 'input' or 'plugins', not '%1'");
    throw Exception(msg.arg(fileLocation), this);
}

void Records::readLineItems() {
    QString line;
    while (!file.atEnd() && line.isEmpty()) {
        line = QString(file.readLine().simplified());
    }
    lineItems = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    pastLastLine = lineItems.isEmpty();
}

void Records::createColumnPullVariables() {
    int n = columnNames.size();
    values.fill(0., n);
    currentColumnValues->fill(0., n);
    nextColumnValues->fill(0., n);
    for (int i = 0; i < n; ++i) {
        Identifier id = columnNames[i];
        double *valuePtr = &values[i];
        if (i != dateColumn && i != timeColumn)
            new Variable<double>(id, valuePtr, this, id.label() + " from file records");
    }
}

void Records::readFromFirstToLastLine() {
    openFile();
    readLineItems(); // skip labels
    advanceFirstLine();
    firstDateTime = currentDateTime;
    while (!pastLastLine)
        advanceLine();
    lastDateTime = currentDateTime;
    file.close();
}

void Records::reset() {
    readToFirstLine();
    if (randomizeInitialYear)
        readToInitialYear();
    if (imposeInitialDateTime) {
        calendar->pushValue<QDate>("initialDate", currentDate);
        calendar->pushValue<QTime>("initialTimeOfDay", currentTime);
        calendar->deepReset();
    }
    update();
}

void Records::readToFirstLine() {
    currentDate = nextDate = QDate(2000,1,1);
    currentTime = nextTime = QTime(0,0,0);
    currentDateTime = nextDateTime = QDateTime(currentDate, currentTime, Qt::UTC);

    openFile();
    readLineItems(); // skip labels
    advanceFirstLine();
}

void Records::readToInitialYear() {
    int fromYear =  firstDateTime.date().year();
    int toYear =  lastDateTime.date().year();

    typedef boost::uniform_int<> Distribution;
    typedef boost::variate_generator<Random::Generator&, Distribution> Variate;
    Distribution distribution(fromYear, toYear);
    Variate variate(*randomGenerator(), distribution);
    int rndYear = variate();

    QDate initialDate = QDate(rndYear, 1, 1);
    while (initialDate > currentDate && !pastLastLine)
        advanceLine();
    if (pastLastLine)
        throw Exception("Read past end in records witout reaching" +
                        initialDate.toString() + " in file " + fileName);
}

void Records::advanceFirstLine() {
    readLineItems();
    if (pastLastLine)
        throw Exception("Records file is empty", this);
    extractValues();
    advanceTime();
    for (int i = 0; i < nextColumnValues->size(); ++i)
        (*currentColumnValues)[i] = nextColumnValues->at(i);

}

void Records::advanceTime() {
    currentDate = nextDate;
    currentTime = nextTime;
    currentDateTime = nextDateTime;

}

void Records::extractValues() {
    if (!pastLastLine && lineItems.size() != columnNames.size())
        throw Exception("Number of items in records file does not match number of column names.\n"
                        + lineItems.join(" "), this);

    for (int i = 0; i < lineItems.size(); ++i) {
        if (i == dateColumn)
            nextDate = UniSim::stringToValue<QDate>(lineItems[dateColumn]);
        else if (i == timeColumn)
            nextTime = UniSim::stringToValue<QTime>(lineItems[timeColumn]);
        else
            (*nextColumnValues)[i] = stringToValue<double>(lineItems[i], this);
    }
    nextDateTime = QDateTime(nextDate, nextTime, Qt::UTC);
}

void Records::advanceLine() {
    advanceTime();
    qSwap(currentColumnValues, nextColumnValues);
    readLineItems();

    if (!pastLastLine) {
        extractValues();
    }
    else {
        for (int i = 0; i < nextColumnValues->size(); ++i)
            (*nextColumnValues)[i] = currentColumnValues->at(i);
    }
}

void Records::update() {
    QDateTime calendarDateTime = calendar->pullValue<QDateTime>("dateTime");
    while (calendarDateTime > nextDateTime && !pastLastLine)
        advanceLine();

    for (int i = 0; i < values.size(); ++i) {
        if (i == dateColumn || i == timeColumn) continue;
        double dx = currentDateTime.secsTo(nextDateTime);
        double dy = nextColumnValues->at(i) - currentColumnValues->at(i);
        double x = currentDateTime.secsTo(calendarDateTime);
        values[i] = ((dx > 0) ? x*dy/dx : 0.) + currentColumnValues->at(i);
    }
}

void Records::cleanup() {
    file.close();
}


} //namespace

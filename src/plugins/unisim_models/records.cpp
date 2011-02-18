#include <iostream>
#include <QTime>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include <usbase/pull_variable_base.h>
#include <usbase/utilities.h>           // testing
#include "records.h"

namespace UniSim {

Records::Records(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<QString>("fileName", &fileName, "records.txt", this,
    "Name of input file");
    new Parameter<QString>("fileLocation", &fileLocation, "weather", this,
    "Valid locations are the standard folders: datasets, plugins and weather. "
    "The standard folders can be set from the File|Locations menu.");
    new Parameter<bool>("imposeInitialDateAndTime", &imposeInitialDateTime, false, this,
    "Set this to 'yes' if you want the simulation to begin on the first date (and time, possibly) "
    "found in the input file. This initial date (and time) will then be imposed on the @F calendar model "
    "for its @F initialDate (and @F {initialTime}) parameters.");

    new PullVariable<QDateTime>("currentDateTime", &currentDateTime, this,
    "The date and time of the current line in the input file. The @F calendar date and time will be at "
    "or past this");
    new PullVariable<QDateTime>("nextDateTime", &nextDateTime, this,
    "The date and time of the next line in the input file. The @F calendar date and time will be at "
    "or before this");
    new PullVariable<QDate>("currentDate", &currentDate, this, "The date part of @F {currentDateTime}");
    new PullVariable<QDate>("nextDate", &nextDate, this, "The date part of @F {nextDateTime}");
    new PullVariable<QTime>("currentTime", &currentTime, this, "The time part of @F {currentDateTime}");
    new PullVariable<QTime>("nextTime", &nextTime, this, "The time part of @F {nextDateTime}");

    currentColumnValues = new QVector<double>;
    nextColumnValues = new QVector<double>;
}

Records::~Records() {
    delete currentColumnValues;
    delete nextColumnValues;
}

void Records::initialize() {
	calendar = seekOne<Model*>("calendar");

    setFileLocationType();

    readColumnNames();
    int n = columnNames.size();
    values.fill(0., n);
    currentColumnValues->fill(0., n);
    nextColumnValues->fill(0., n);

    for (int i = 0; i < n; ++i) {
        Identifier id = columnNames[i];
        double *valuePtr = &values[i];
        if (i != dateColumn && i != timeColumn)
            new PullVariable<double>(id, valuePtr, this, id.label() + " from file records");
    }
}

void Records::setFileLocationType() {
    Identifier id(fileLocation);
    if (id.equals("weather"))
        fileLocationType = FileLocationInfo::Weather;
    else if (id.equals("datasets"))
        fileLocationType = FileLocationInfo::Datasets;
    else if (id.equals("plugins"))
        fileLocationType = FileLocationInfo::Plugins;
    else
        throw Exception("File location must be 'weather' or 'datasets', not '" +
                        fileLocation +"'");
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
        if (id.equals("date"))
            dateColumn = i;
        else if (id.equals("time"))
            timeColumn = i;
    }
    file.close();
}

void Records::openFile() {
    QString filePath = FileLocations::location(fileLocationType).absolutePath() +
                       "/" + fileName;
    file.setFileName(filePath);

    bool fileOk = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!fileOk)
        throw Exception("Cannot open records file: " + filePath, this);

    pastLastLine = false;
}

void Records::readLineItems() {
    QString line;
    while (!file.atEnd() && line.isEmpty()) {
        line = QString(file.readLine().simplified());
    }
    lineItems = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    pastLastLine = lineItems.isEmpty();
}

void Records::reset() {
    currentDate = nextDate = QDate(2000,1,1);
    currentTime = nextTime = QTime(0,0,0);
    currentDateTime = nextDateTime = QDateTime(currentDate, currentTime, Qt::UTC);

    openFile();
    readLineItems(); // skip labels (already read)
    advanceFirstLine();
    if (imposeInitialDateTime) {
        calendar->pushVariable<QDate>("initialDate", currentDate);
        calendar->pushVariable<QTime>("initialTimeOfDay", currentTime);
        calendar->deepReset();
    }
    update();
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

namespace {
    template <class T>
    void swap(T &a, T &b) {
        T c = a;
        a = b;
        b = c;
    }
}

void Records::advanceLine() {
    advanceTime();
    swap(currentColumnValues, nextColumnValues);
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
    QDateTime calendarDateTime = calendar->pullVariable<QDateTime>("dateTime");
    while (calendarDateTime > nextDateTime && !pastLastLine)
        advanceLine();

    for (int i = 0; i < values.size(); ++i) {
        if (i == dateColumn || i == timeColumn) continue;
        double dx = currentDateTime.secsTo(nextDateTime);
        double dy = nextColumnValues->at(i) - currentColumnValues->at(i);
        double x = currentDateTime.secsTo(calendarDateTime);
        values[i] = ((dx > 0) ? x*dy/dx : 0.) + currentColumnValues->at(i);
    }
    /*
    QString s = calendarDate.toString("d/M/yyyy") + " "
                + currentDate.toString("d/M/yyyy") + " "
                + nextDate.toString("d/M/yyyy") + "\n";
    std::cout << qPrintable(s);
    */
}

void Records::cleanup() {
    file.close();
}


} //namespace

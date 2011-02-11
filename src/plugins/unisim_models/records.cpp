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
    new Parameter<QString>("fileName", &fileName, "record.txt", this, "desc");
    /*new Parameter<QDate>("startingDate", &startingDate, "1/1/2000", this, "desc");
    new Parameter<QTime>("startingTime", &startingTime, "00:00:00", this, "desc");*/

    new PullVariable<QDate>("currentDate", &currentDate, this, "desc");
    new PullVariable<QDate>("nextDate", &nextDate, this, "desc");
    new PullVariable<QTime>("currentTime", &currentTime, this, "desc");
    new PullVariable<QTime>("nextTime", &nextTime, this, "desc");

    currentColumnValues = new QVector<double>;
    nextColumnValues = new QVector<double>;
}

Records::~Records() {
    delete currentColumnValues;
    delete nextColumnValues;
}

void Records::initialize() {
	calendar = seekOne<Model*>("calendar");
    //double timeStep = calendar->pullVariable<double>("timeStep");  // days

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

void Records::readColumnNames() {
    openFile();
    readLineItems();
    if (lineItems.isEmpty())
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
}

void Records::openFile() {
    QString filePath = FileLocations::location(FileLocationInfo::Datasets).absolutePath() +
                       "/" + fileName;
    file.setFileName(filePath);

    bool fileOk = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!fileOk) throw Exception("Cannot open records file: " + filePath, this);
}

void Records::readLineItems() {
    QString line;
    while (!file.atEnd() && line.isEmpty()) {
        line = QString(file.readLine().simplified());
    }
    lineItems = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
}

void Records::reset() {
    advanceFirstLine();
    update();
}

void Records::advanceFirstLine() {
    readLineItems();
    if (lineItems.isEmpty())
        throw Exception("Records file is empty", this);
    extractValues();
    currentDate = nextDate;
    currentTime = nextTime;
    for (int i = 0; i < nextColumnValues->size(); ++i)
        (*currentColumnValues)[i] = nextColumnValues->at(i);
}

void Records::extractValues() {
    if (!lineItems.isEmpty() && lineItems.size() != columnNames.size())
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
    currentDate = nextDate;
    currentTime = nextTime;
    swap(currentColumnValues, nextColumnValues);
    readLineItems();
    extractValues();

    bool pastLastLine = lineItems.isEmpty();
    if (pastLastLine) {
        for (int i = 0; i < nextColumnValues->size(); ++i)
            (*nextColumnValues)[i] = currentColumnValues->at(i);
    }
}

void Records::update() {
    QDate calendarDate = calendar->pullVariable<QDate>("date");
    if (calendarDate > nextDate)
        advanceLine();

    for (int i = 0; i < values.size(); ++i) {
        if (i == dateColumn || i == timeColumn) continue;
        double dx = currentDate.daysTo(nextDate);
        double dy = nextColumnValues->at(i) - currentColumnValues->at(i);
        double x = currentDate.daysTo(calendarDate);
        values[i] = ((dx > 0) ? x*dy/dx : 0.) + currentColumnValues->at(i);
    }

    QString s = calendarDate.toString("d/M/yyyy") + " "
                + currentDate.toString("d/M/yyyy") + " "
                + nextDate.toString("d/M/yyyy") + "\n";
    std::cout << qPrintable(s);
}

void Records::cleanup() {
    file.close();
}


} //namespace

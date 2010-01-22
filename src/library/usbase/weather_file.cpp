/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include "weather_file.h"

using namespace std;
using namespace UniSim;


inline QString colName(QString name) {
    return "col" + name;
}

WeatherFile::Column::Column(QString variableName_, int defaultColumn_, Model *parent)
    : variableName(variableName_), defaultColumn(defaultColumn_)
{
    Q_ASSERT(parent);
    parent->setState(variableName, &value);
}

void WeatherFile::Column::setParameter(Model *parent) {
    parent->setParameter(colName(variableName), &column, defaultColumn);
}

void WeatherFile::Column::updateState(const QStringList &items) {
    if (column <= 0)
        return;
    if (column > items.size())
        throw Exception("Missing value in weather file."
                        "Cannot find value in column: " + QString::number(column) +
                        " for variable: " + variableName);
    bool ok;
    int ix = column - 1;
    value = items[ix].toDouble(&ok);
    if (!ok)
        throw Exception("Value read for variable: " + variableName +
                        "in weather file is not a number: '" + items[ix] + "'");
}


namespace UniSim{

const double pi = 3.14159265;

WeatherFile::WeatherFile(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
}

WeatherFile::~WeatherFile() {
    for (Columns::iterator co = columns.begin(); co != columns.end(); ++co)
        delete co.value();
    columns.clear();
}

void WeatherFile::initialize()
{
    for (Columns::iterator co = columns.begin(); co != columns.end(); ++co)
        co.value()->setParameter(this);
    setParameter("fileName", &fileName, QString());
}

void WeatherFile::reset()
{
    if (file.isOpen()) file.close();
    QString filePath = FileLocations::location(FileLocations::Models).absolutePath() +
                       "/" + fileName;
    file.setFileName(filePath);

    bool fileOk = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!fileOk) throw Exception("Cannot open weather file: " + filePath);

    lineNo = 0;
}

void WeatherFile::update()
{
    if (file.atEnd()) return;
    QString line = QString(file.readLine()).simplified();
    ++lineNo;
    QStringList items = line.split(" ");

    try {
        for (Columns::iterator co = columns.begin(); co != columns.end(); ++co)
            co.value()->updateState(items);
    }
    catch (const Exception &ex) {
        throw Exception(ex.message() + "\nIn line " + QString::number(lineNo) +
                        ": " + line);
    }
}

void WeatherFile::setColumn(QString name, int defaultColumn) {
    if (columns.contains(colName(name)))
        changeParameter(colName(name), defaultColumn);
    else
        columns[name] = new Column(name, defaultColumn, this);
}

bool WeatherFile::variableExists(QString name) {
    return columns.contains(colName(name));
}

} //namespace


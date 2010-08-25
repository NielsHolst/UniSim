/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "weather_file.h"

using namespace std;

namespace UniSim{

WeatherFile::Column::Column(QString variableName_, int defaultColumn_, Model *parent)
    : variableName(variableName_), defaultColumn(defaultColumn_)
{
    Q_ASSERT(parent);
    new Parameter<int>(variableName, &column, defaultColumn, parent, "Column number (counting from 1) of variable in weather file");
    new PullVariable<double>(variableName, &value, parent, "Weather file variable");
}

void WeatherFile::Column::parseLine(const QStringList &items) {
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

WeatherFile::WeatherFile(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<QString>("fileName", &fileName, QString(), this, "description");
    new Parameter<QDate>("firstDate", &firstDate, QDate(), this, "description");
}

WeatherFile::~WeatherFile() {
    for (int i = 0; i < columns.size(); ++i)
        delete columns[i];
    columns.clear();
}

void WeatherFile::initialize()
{
    hasBeenReset = false;
}

void WeatherFile::reset()
{
    if (hasBeenReset) return;

    if (file.isOpen()) file.close();
    QString filePath = FileLocations::location(FileLocations::Weather).absolutePath() +
                       "/" + fileName;
    file.setFileName(filePath);

    bool fileOk = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!fileOk) throw Exception("Cannot open weather file: " + filePath);

    lineNo = 0;
    hasBeenReset = true;
}

void WeatherFile::update() {
    if (file.atEnd()) return;
    QString line = QString(file.readLine()).simplified();
    ++lineNo;
    QStringList items = line.split(" ");

    try {
        for (int i = 0; i < columns.size(); ++i)
            columns[i]->parseLine(items);
    }
    catch (const Exception &ex) {
        throw Exception(ex.message() + "\nIn line " + QString::number(lineNo) +
                        ": " + line);
    }
}

void WeatherFile::cleanup() {
    hasBeenReset = false;
}

void WeatherFile::setColumn(QString name, int defaultColumn) {
    QList<Parameter<int>*> params = seekChildren<Parameter<int>*>(name);
    if (params.size() == 0)
        columns.append(new Column(name, defaultColumn, this));
    else if (params.size() == 1)
        params[0]->setValue(defaultColumn);
    else
        throw Exception("Parameters cannot have same name: "
                        + name + " defined twice for " + fullName());
}

} //namespace


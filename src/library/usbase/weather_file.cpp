/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
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
    QString item = items[ix].trimmed();
    value = item.toDouble(&ok);
    if (!ok)
        throw Exception("Value read for variable: " + variableName +
                        "in weather file is not a number: '" + items[ix] + "'");
}

WeatherFile::WeatherFile(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<QString>("fileName", &fileName, QString(), this,
        "Name of weather file. The file will be looked up in the UniSim weather folder (set in the"
        "File | Locations menu)");
    new Parameter<QDate>("firstDate", &firstDate, QDate(), this,
        "Date of first line in the weather file");
    new Parameter<int>("headerLines", &headerLines, 0, this,
        "Number of header lines to skip in weather file. If the file begins with a line of "
        "column headings, for example, @F headerLines should be @F {1}");
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
    QString filePath = FileLocations::location(FileLocationInfo::Weather).absolutePath() +
                       "/" + fileName;
    file.setFileName(filePath);

    bool fileOk = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!fileOk) throw Exception("Cannot open weather file: " + filePath);

    lineNo = 0;
    hasBeenReset = true;
    for (int i = 0; i < headerLines; ++i)
        readLine();
}

bool WeatherFile::readLine() {
    if (atEnd()) return false;
    line = QString(file.readLine());
    ++lineNo;
    return true;
}

bool WeatherFile::atEnd() const {
    return file.atEnd();
}

void WeatherFile::update() {
    if (!readLine()) return;

    QRegExp whitespace = QRegExp("\\s+");
    QStringList items = line.split(whitespace, QString::SkipEmptyParts);

    try {
        for (int i = 0; i < columns.size(); ++i)
            columns[i]->parseLine(items);
    }
    catch (const Exception &ex) {
        throw Exception(ex.message() + "\nIn line " + QString::number(lineNo) +
                        ": " + line, this);
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
                        + name + " defined twice", this);
}

} //namespace


/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QList>
#include <QPair>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "dataset.h"

namespace UniSim{

Dataset::Dataset(Identifier name, QObject *parent)
    : Component(name, parent)
{
    new Parameter<QString>("fileName", &fileName, QString(), this,
        "Name of file. The file will be looked up in the UniSim weather folder (set in the"
        "File | Locations menu)");
    new Parameter<QDate>("firstDate", &firstDate, QDate(), this,
        "Date of first line in the file");
    new Parameter<char>("separator", &separator, '\t', this,
        "Character that separates columns in the file.");
    new Parameter<bool>("keepEmptyColumns", &keepEmptyColumns, true, this,
        "Should empty columns be kept? If true \"12\\t\\t34\" will counts as 3 columns, "
        "otherwise as 2 columns");
    new Parameter<int>("headerLines", &headerLines, 0, this,
        "Number of header lines to skip in file. If the file begins with a line of "
        "column headings, for example, @F headerLines should be @F {1}");

    new Parameter<QString>("columns", &columns, QString(), this,
    "Column variables given as (name column-number) pair, e.g."
    "( (x 2) (prey 4) predator 5) )");

}

void Dataset::initialize() {
    QString filePath = FileLocations::location(FileLocationInfo::Datasets).absolutePath() +
                       "/" + fileName;
    file.setFileName(filePath);

    bool fileOk = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!fileOk) throw Exception("Cannot open dataset file: " + filePath);

    for (int i = 0; i < headerLines; ++i)
        file.readLine();

    QString line;
    QString::SplitBehavior behavior =
        keepEmptyColumns ? QString::KeepEmptyParts : QString::SkipEmptyParts;

    initializeData();
    while (!file.atEnd()) {
        line = QString(file.readLine());
        if (line.isEmpty() || line.simplified() == " ") continue;
        QStringList items = line.split(separator, behavior);

        QMapIterator<QString, ColumnData > i(data);
        while (i.hasNext()) {
            i.next();
            itemToData(items, i.value().colIndex, i.key());
        }
    }
}

void Dataset::initializeData() {
    typedef QPair<QString, int> NameVal;
    QList<NameVal> nameValueList = decodeNameValueList<int>(columns, this);
    for (int i = 0; i < nameValueList.size(); ++i) {
        QString name = nameValueList[i].first;
        if (contains(name))
            throw Exception("Variable names must be unique in dataset: " + name);

        ColumnData colData;
        colData.colIndex = nameValueList[i].second - 1;
        data[name] = colData;
    }
}

void Dataset::itemToData(QStringList items, int colIndex, QString columnName) {
    if (colIndex > items.size()) {
        QString msg = "Too few columns in line, expected at least "
                      + QString::number(colIndex+1);
        throw Exception(msg, this);
    }
    bool ok;
    double value = items[colIndex].toDouble(&ok);
    if (!ok) {
        QString msg = "Value in dataset file is not a number: '" + items[colIndex] + "'";
        throw Exception(msg, this);
    }
    data[columnName].values.append(value);
}

bool Dataset::contains(QString columnName) const {
    return data.contains(columnName);
}


const QVector<double>* Dataset::columnData(QString columnName) const {
    if (!contains(columnName))
        throw("Dataset does not contain column named: '" + columnName + "'", this);
    return &(data[columnName].values);
}

} //namespace


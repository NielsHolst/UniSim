/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QFile>
#include "exception.h"
#include "parameter_file_base.h"
#include "utilities.h"

namespace UniSim{

ParameterFileBase::ParameterFileBase(Identifier id, QString defaultFilePath, QObject *parent, QString desc)
    :  ParameterBase(id, parent, desc)
{
    assertUniqueness(id, parent);
    setValueFromString(defaultFilePath);
}


QString ParameterFileBase::filePath() const {
    return _filePath;
}

QString ParameterFileBase::stringValue(QString rowKey, QString columnKey) {
    readData();
    QStringList row = data.value(rowKey);
    if (row.isEmpty()) {
        QString msg("ParameterFileBase '%1': file '%2' has no row matching the key: '%3'");
        throw Exception(msg.arg(id().label()).arg(filePath()).arg(rowKey), this);
    }

    int columnIndex = columnNames.indexOf(Identifier(columnKey));
    if (columnIndex == -1) {
        QString msg("ParameterFileBase '%1' has no column called '%2' in file '%3'");
        throw Exception(msg.arg(id().label()).arg(columnKey).arg(filePath()), this);
    }

    return row[columnIndex];
}

void ParameterFileBase::readData() {
    if (!data.isEmpty())
        return;
    Q_ASSERT(columnNames.isEmpty());

    file.setFileName(filePath());
    bool fileOk = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!fileOk) {
        QString msg("Cannot open ParameterFileBase '%1' file: '%2'");
        throw Exception(msg.arg(id().label()).arg(filePath()), this);
    }

    readLineItems();
    if (pastLastLine) {
        QString msg("ParameterFileBase '%1' file is empty: '%2'");
        throw Exception(msg.arg(id().label()).arg(filePath()), this);
    }
    for (int i = 0; i < lineItems.size(); ++i) {
        columnNames.append(Identifier(lineItems[i]));
    }

    while (true) {
        readLineItems();
        if (pastLastLine)
            break;

        if (lineItems.size() != columnNames.size()) {
            QString msg("ParameterFileBase '%1' in file '%2': number of columns labels (%3) does not match number of values in row (%4):\n%5");
            throw Exception(msg.arg(id().label()).arg(filePath()).arg(columnNames.size()).arg(lineItems.size()).arg(lineItems.join(";")), this);
        }

        QString rk = rowKey(lineItems);
        if (!data.value(rk).isEmpty()) {
            QString msg("ParameterFileBase '%1' in file '%2': key '%3' is duplicate; it does not uniquely identify a row");
            throw Exception(msg.arg(id().label()).arg(filePath()).arg(rk), this);
        }
        data[rk] = lineItems;
    }
}

void ParameterFileBase::readLineItems() {
    QString line;
    while (!file.atEnd() && line.isEmpty()) {
        line = QString(file.readLine().simplified());
    }
    lineItems = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    pastLastLine = lineItems.isEmpty();
}

QVariant ParameterFileBase::toVariant() const {
    return QVariant(filePath());
}

QString ParameterFileBase::toString() const {
    return filePath();
}

QString ParameterFileBase::typeId() const {
    return "ParameterFileBase";
}

void ParameterFileBase::setValueFromString(QString newValue) {
    _filePath = newValue;
}

void ParameterFileBase::followRedirection() {
}

void ParameterFileBase::createStrata(double deviance, int n, StrataBase::Type type) {
}

void ParameterFileBase::setValueFromNextSample() {
}

} //namespace



/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_PARAMETER_FILE_BASE_H
#define UNISIM_PARAMETER_FILE_BASE_H

#include <iostream>
#include <QFile>
#include <QHash>
#include <QStringList>
#include "exception.h"
#include "parameter_base.h"
#include "utilities.h"

namespace UniSim{

class ParameterFileBase : public ParameterBase
{
    // no Q_OBJECT
public:
    ParameterFileBase(Identifier name, QString defaultFilePath, QObject *parent, QString desc);
    // generic
    QVariant toVariant() const;
    QString toString() const;
    QString typeId() const;
    void setValueFromString(QString newValue);
    void followRedirection();
    void createStrata(double deviance, int n, StrataBase::Type type);
    void setValueFromNextSample();

    // special
    QString filePath() const;
    QString stringValue(QString rowKey, QString columnKey);
    virtual QString rowKey(QStringList keys) const = 0;

private:
    // methods
    void readData();
    void readLineItems();
    QString stringValue(QString rowKey1, QString rowKey2, QString columnKey);

    // data
    QHash<QString,QStringList> data;
    QList<Identifier> columnNames;

    // file data
    QString _filePath;
    QFile file;
    QStringList lineItems;
    bool pastLastLine;
};

} //namespace

#endif


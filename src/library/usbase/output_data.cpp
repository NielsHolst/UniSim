/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QRegExp>
#include "dataset.h"
#include "model.h"
#include "output_data.h"
#include "pull_variable.h"
#include "utilities.h"

namespace UniSim{
	

OutputData::OutputData(QString label, QString axis, Dataset *dataset_, QString columnName_, QObject *parent)
    : Component(label, parent), dataset(dataset_), columnName(columnName_)
{
    setAxisFromString(axis);
}

void OutputData::setAxisFromString(QString axis) {
    QString s = axis.toLower();
    if (axis!="x" && axis!="y")
        throw Exception("Axis must be either 'x'' or 'y', not '" + axis + "'");
    _axis = axis == "x" ? XAxis : YAxis;
}

OutputData::Axis OutputData::axis() const {
    return _axis;
}

const QVector<double>* OutputData::data() const {
    return dataset->columnData(columnName);
}

} //namespace


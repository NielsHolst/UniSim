/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/decode_list.h>
#include <usbase/variable.h>
#include "publish.h"
#include "average.h"

namespace UniSim{

PUBLISH(Average)

Average::Average(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    Input(QString, inputs, QString());
    Output(double, value);
}

void Average::initialize() {
    QStringList items = decodeList(inputs, this);
    int n = items.size();
    values.clear();
    for (int i = 0; i < n; ++i) {
        VariableBase *var = seekOne<Model*,VariableBase*>(items[i]);
        values << var;
    }
}

void Average::reset() {
    value = 0.;
}

void Average::update() {
    if (values.isEmpty())
        return;
    bool ok = true;
    double sum = 0;
    int i;
    for (i = 0; ok && i < values.size(); ++i)
        sum += values.at(i)->toVariant().toDouble(&ok);
    if (!ok) {
        QString msg{"Input '%1' cannot be converted to a number"};
        throw Exception(msg.arg(values.at(i)->toString()), this);
    }
    value = sum/values.size();
}

} //namespace


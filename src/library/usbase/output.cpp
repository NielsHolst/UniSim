/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "integrator.h"
#include "output.h"
#include "output_data.h"
#include "output_result.h"
#include "parameter.h"

namespace UniSim{
	
Output::Output(Identifier name, QObject *parent)
    : Component(name, parent), _isSummary(false)
{
}

void Output::initialize() {
    results = seekChildren<OutputResult*>("*");
    for (int i = 0; i < results.size(); ++i) {
        if (results[i]->axis() == OutputResult::XAxis)
            xRes.append(results[i]);
        else
            yRes.append(results[i]);
    }

    data = seekChildren<OutputData*>("*");
    for (int i = 0; i < data.size(); ++i) {
        if (data[i]->axis() == OutputData::XAxis)
            xDat.append(data[i]);
        else
            yDat.append(data[i]);
    }

    integrator = seekOne<Integrator*>("*");
}

const OutputResults& Output::xResults() const {
    return xRes;
}

const OutputResults& Output::yResults() const {
    return yRes;
}

const OutputDatas& Output::xData() const {
    return xDat;
}

const OutputDatas& Output::yData() const {
    return yDat;
}

int Output::runNumber() const {
    return integrator->pullVariable<int>("runNumber");
}

bool Output::isSummary() const {
    return _isSummary;
}

void Output::setIsSummary(bool value) {
    _isSummary = value;
}

} //namespace


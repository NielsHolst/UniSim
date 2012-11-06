/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef NETWORK_INDICATORS_IMPORT_INDICATOR_H
#define NETWORK_INDICATORS_IMPORT_INDICATOR_H

#include <QList>
#include <QVector>
#include <usbase/model.h>

namespace network_indicators {

class ImportIndicator : public UniSim::Model
{
public:
    ImportIndicator(UniSim::Identifier name, QObject *parent);
    void amend();
    void reset();
    void update();
private:
    // links
    QList<Model*> farms;
    QVector<double> farmIndicators;

    // methods
    double computeIndicator(UniSim::Model *farm);
};

} //namespace


#endif

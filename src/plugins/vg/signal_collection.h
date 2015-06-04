/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SIGNAL_COLLECTION_H
#define VG_SIGNAL_COLLECTION_H

#include <QList>
#include <usbase/string_map.h>
#include "base_signal.h"

namespace vg {

class SignalCollection : public BaseSignal
{
public:
    SignalCollection(UniSim::Identifier name, QObject *parent);
    void initialize();
    void localReset();
    double signal();
private:
    QString ruleStr;
    QList<const double *> childSignals;
    enum Rule {Min, Max, Sum};
    Rule rule;
    static UniSim::StringMap<Rule> rules;
    void setRules();
};

} //namespace
#endif

/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_H
#define UNISIM_OUTPUT_H

#include <QList>
#include <usbase/component.h>

namespace UniSim{

class OutputData;
class OutputVariable;

class Output : public Component
{
	Q_OBJECT
public:
    Output(Identifier name, QObject *parent=0);
    // standard methods
    virtual void initialize();
    // special methods
    const QList<OutputVariable *>& xVariables() const;
    const QList<OutputVariable *>& yVariables() const;

    const QList<OutputData *>& xData() const;
    const QList<OutputData *>& yData() const;

private:
    // links
    QList<OutputVariable *> variables, xVar, yVar;
    QList<OutputData *> data, xDat, yDat;
};

typedef QList<Output*> Outputs;

} //namespace

#endif


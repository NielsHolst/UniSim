/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_H
#define UNISIM_OUTPUT_H

#include <QList>
#include <usbase/component.h>

namespace UniSim{

class Integrator;
class OutputData;
class OutputResult;

class Output : public Component
{
	Q_OBJECT
public:
    Output(Identifier name, QObject *parent=0);
    // standard methods
    virtual void initialize();
    // special methods
    const QList<OutputResult *>& xResults() const;
    const QList<OutputResult *>& yResults() const;

    const QList<OutputData *>& xData() const;
    const QList<OutputData *>& yData() const;

    bool isSummary() const;
    void setIsSummary(bool value);
protected:
    int runNumber() const;
private:
    // data
    bool _isSummary;
    // links
    QList<OutputResult *> results, xRes, yRes;
    QList<OutputData *> data, xDat, yDat;
    Integrator *integrator;
};

typedef QList<Output*> Outputs;

} //namespace

#endif


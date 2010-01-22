/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_H
#define UNISIM_OUTPUT_H

#include <QList>
#include <usbase/component.h>
#include <usbase/parameters.h>

namespace UniSim{

class OutputVariable;

class Output : public Component, public Parameters
{
	Q_OBJECT
public:
    Output(Identifier name, QObject *parent=0);
    // standard methods
    virtual void initialize();
    // special methods
    const QList<OutputVariable *>& xVariables() const;
    const QList<OutputVariable *>& yVariables() const;
private:
    // parameters
    QString title;

    // links
    QList<OutputVariable *> variables, x, y;

    // methods
    void extendLabels();
    bool isDuplicateLabel(int ix);
    void segmentVariables();
	
};
typedef QList<Output*> Outputs;

} //namespace

#endif


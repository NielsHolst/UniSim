/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_SCENARIOS_H
#define UNISIM_SCENARIOS_H
#include <QVector>
#include <usbase/model.h>
#include <usbase/parameter_base.h>

namespace UniSim {

class DataGrid;

class Scenarios : public Model
{
    //Q_OBJECT
public:
    Scenarios(UniSim::Identifier name, QObject *parent=0);
    // standard methods
    void amend();
    void reset();
    void cleanup();
private:
    // parameters
    QString fileName;

    // variables
    bool value;
    int iteration, numIterations;

    // data
    DataGrid *scenarios;
    QVector<QString> line;
    QVector<ParameterBase*> refs;

    // methods
    void parseColumnNames();
};

}
#endif

/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef XPEST_INPUT_TABLE_H
#define XPEST_INPUT_TABLE_H

#include <usbase/model.h>

namespace UniSim {
    class DataGrid;
}

namespace xpest {

class InputTable : public UniSim::Model
{
public:
    InputTable(UniSim::Identifier name, QObject *parent);
    void amend();
    void reset();
    void update();

private:
    // Parameters
    QString fileName, xName, yName;
    double xValue;

    // Variables
    double yValue;

    // Data
    UniSim::DataGrid *table;
    QVector<double> x, y;

    // Methods
    void checkXColumn();
};

} //namespace


#endif

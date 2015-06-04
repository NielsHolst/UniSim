/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/data_grid.h>
#include <usengine/simulation.h>
#include "input_table.h"
#include "publish.h"

using namespace UniSim;

namespace xpest {
	
PUBLISH(InputTable)

InputTable::InputTable(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(QString, fileName, QString());    // Name of column-based text file
    Input(QString, xName, QString());       // Name of x column
    Input(QString, yName, QString());       // Name of y column
    Input(double, xValue, 0.);              // Value of x to interpolate to y
    Output(double, yValue);                 // Y value interpolated from x
}

void InputTable::amend() {
    QString filePath = simulation()->inputFilePath(fileName);
    table = new DataGrid(filePath, this);
    int xColumn = table->findColumn(xName),
        yColumn = table->findColumn(yName);
    x = table->column<double>(xColumn);
    y = table->column<double>(yColumn);
    checkXColumn();
}

void InputTable::reset() {
    yValue = 0.;
}

void InputTable::update() {
    if (xValue <= x.first()) {
        yValue = y.first();
    }
    else if (xValue >= x.last()) {
        yValue = y.last();
    }
    else {
        int i = 0;
        while (i < x.size() && x.at(i) < xValue) { ++i; }
        yValue = y.at(i-1) +
                (xValue - x.at(i-1)) * (y.at(i) - y.at(i-1)) / (x.at(i) - x.at(i-1));
    }
}

void InputTable::checkXColumn() {
    for (int i = 1; i < x.size(); ++i) {
        double rowX = x.at(i), prevX = x.at(i-1);
        if (rowX <= prevX) {
            QString msg = "X column must have increasing values (%1 <= %2)";
            throw Exception(msg.arg(rowX).arg(prevX), this);
        }
    }

}


} //namespace


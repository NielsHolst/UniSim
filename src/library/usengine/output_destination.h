/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UniSim_OUTPUT_DESTINATION_H
#define UniSim_OUTPUT_DESTINATION_H

#include <QList>
#include <QVector>
#include <usbase/component.h>


namespace UniSim{

class Output;
class XYStateVariables;

class OutputDestination : public Component
{
	Q_OBJECT
public:
	typedef QVector<double> Data;
    OutputDestination(QString name, Output *parent=0);
	
    void cleanup();
	
    void setXYStateVariablesList(const QList<XYStateVariables*> *xyList);
	
protected:
    const QList<XYStateVariables*> *_xyList;
    Output *_output;

private:
    virtual void showTitle() = 0;
    virtual void showLabels() = 0;
    virtual void showData() = 0;

};

}
#endif

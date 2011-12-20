/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_PARAMETER_H
#define UNISIM_OUTPUT_PARAMETER_H

#include <QList>
#include <QVector>
#include "output_result.h"

namespace UniSim{	

class ParameterBase;

class OutputParameter : public OutputResult
{
	Q_OBJECT
public:
    OutputParameter(const QMap<QString,QString> &attributes,  ParameterBase *parameter, QObject *parent = 0);
	
    // standard methods
	void update();

private:
    // links
    const Model *model;		//!< Pointer to the model holding the parameter
    const ParameterBase *parameterPtr;
};

typedef QList<OutputParameter*> OutputParameters;

} //namespace


#endif

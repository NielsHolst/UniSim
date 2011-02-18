/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_H
#define UNISIM_OUTPUT_H

#include <usbase/component.h>
#include <usbase/parameters.h>

namespace UniSim{
	
class Output : public Component, public Parameters
{
	Q_OBJECT
public:
    Output(QString name, QObject *parent=0);
    // standard methods
    void initialize();

private:
    // parameters
    QString title;
	
};

} //namespace

#endif


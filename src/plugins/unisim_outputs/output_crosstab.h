/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_CROSSTAB
#define UNISIM_OUTPUT_CROSSTAB

#include <usbase/model.h>
#include "output_table.h"

namespace UniSim{

class OutputCrosstab : public OutputTable
{
	Q_OBJECT
public:
    OutputCrosstab(Identifier name, QObject *parent=0);
    // standard methods
    void amend();
    void debrief();
private:
    QStringList ancestorClassNames;

    void extendAncestors(Models *ancestors, const Models *myAncestors, QString traceLabel);
    void transformFile(QString filePath);
};

} //namespace


#endif

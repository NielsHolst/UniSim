/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_VARIABLE_COLLECTION_H
#define VG_VARIABLE_COLLECTION_H

#include <usbase/named_object.h>
#include <usbase/variable.h>

namespace vg {

class VariableCollection
{
public:
    VariableCollection();
    void set(UniSim::NamedObject *source, QString nameExpression, bool assertNotEmpty = true);
    double sum() const;
    double average() const;
    double product() const;
    int size() const;
private:
    QList<UniSim::Variable<double>*> variables;
};

} //namespace


#endif

/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_ATTRIBUTES_H
#define UNISIM_ATTRIBUTES_H
#include <QHash>
#include <QString>
#include <QVariant>

namespace UniSim{

class Attributes
{
public: 
    typedef QHash<QString, QVariant> Table;

    Attributes()
    { }

    void setAttribute(QString name, QVariant value)
    { table[name] = value; }

    bool hasAttribute(QString name) const
    { return table.contains(name); }

    QVariant attribute(QString name) const
    { return table.value(name); }

    const Table& attributes() const
    { return table; }

    void appendAttributes(const Attributes &attr)
    { table.unite(attr.table); }

private:
    Table table;
};



} //namespace
#endif

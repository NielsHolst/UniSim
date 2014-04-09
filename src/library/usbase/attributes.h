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

//! Attributes can be used as a stand-alone or mix-in contained of name-value pairs.
//! Names are strings and values are variants.
class Attributes
{
public: 
    //! Table with all attributes, stored as name-value pairs
    typedef QHash<QString, QVariant> Table;

    //! Constructs empty attributes
    Attributes()
    { }

    //! Sets (creates or over-writes) the attribute named \em name to a variant \em value
    void setAttribute(QString name, QVariant value)
    { table[name] = value; }

    //! Returns true if attribute with \em name exists; otherwise returns false.
    bool hasAttribute(QString name) const
    { return table.contains(name); }

    /*! Returns attribute value associated with \em name.
        If \em name does not exist, the function returns a default-constructed variant.
     */
    QVariant attribute(QString name) const
    { return table.value(name); }

    //! Returns a reference to the attributes table
    const Table& attributes() const
    { return table; }

    /*! Appends all the given \em attributes.
        If a name already exist, its value will be over-written by the value found in \em attr.
     */
    void appendAttributes(const Attributes &attributes)
    {
        QHashIterator<QString, QVariant> i(attributes.table);
        while (i.hasNext()) {
            i.next();
            setAttribute(i.key(), i.value());
        }
    }

private:
    Table table;
};



} //namespace
#endif

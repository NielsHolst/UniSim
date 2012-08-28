/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_NAME_LIST_H
#define UNISIM_NAME_LIST_H

#include <QList>
#include <QString>
#include <QStringList>

namespace UniSim{

class NameList {
	public:
    NameList(const QStringList &names);
	QStringList simplified();
	
	private:
    QObject *root;
    QStringList names, result;
    QObjectList leaves;
    int test;

    void addNameToTree(QString name, int index);
    void addChildToTree(QString parentName, QString childName, int index);
    void appendLeaves(QObject *object);
    void putResult(QObject *object);
};

} //namespace

#endif

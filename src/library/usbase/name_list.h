/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
    QStringList names;
    int test;

    void prepareNames();
    void addNameToTree(QString name);
    void addChildToTree(QString parentName, QString childName);
    void appendLeaves(QObject *tree);
    void appendAtFork(QObject *tree);

};

} //namespace

#endif

/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_BUILDER_H
#define UNISIM_BUILDER_H

#include <QList>
#include <QStack>
#include <QString>

class Builder {
public:
    Builder& simulation();
    Builder& controller(QString type, QString name);
    Builder& model(QString type, QString name);
    Builder& bare(QString name);
    Builder& parameter(QString name, QString value);
    Builder& begin();
    Builder& end();
    friend Builder& builder();

private:
    Builder() {}
	static Builder* theBuilder;
    typedef enum {Simulation, Controller, Model, Bare, Parameter, Begin, End} Type;
	struct Node {
        Type type;
		QString text;
		QList<Node*> children;
	};
	QStack<Node*> nodes;
};

Builder& builder();

#endif

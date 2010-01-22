/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <usbase/utilities.h>
#include "pt_development.h"

using namespace UniSim;

namespace grainstore{

PtDevelopment::PtDevelopment(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    setRecursionPolicy(Component::Update, Component::ChildrenNot);
}

void PtDevelopment::initialize()
{
    egg = findChild<Model*>("egg");
    larva = findChild<Model*>("larva");
    pupa = findChild<Model*>("pupa");
    adult = findChild<Model*>("adult");
    calendar = findOne<Model*>("calendar");
    weather = findOne<Model*>("weather");
    immigration = findSibling<Model*>("immigration");
}

void PtDevelopment::reset()
{
}

void PtDevelopment::update()
{
    egg->deepUpdate();

    larva->setInput("inflow", egg->state("outflow"));
    larva->deepUpdate();

    pupa->setInput("inflow", larva->state("outflow"));
    pupa->deepUpdate();

    double immRate = immigration->state("current");
    adult->setInput("inflow", immRate + pupa->state("outflow"));
    adult->deepUpdate();
}

} //namespace


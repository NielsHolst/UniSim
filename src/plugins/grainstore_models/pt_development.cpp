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
    egg = seekOneChild<Model*>("egg");
    larva = seekOneChild<Model*>("larva");
    pupa = seekOneChild<Model*>("pupa");
    adult = seekOneChild<Model*>("adult");
    calendar = seekOne<Model*>("calendar");
    weather = seekOne<Model*>("weather");
    immigration = seekOneSibling<Model*>("immigration");
}

void PtDevelopment::reset()
{
}

void PtDevelopment::update()
{
    egg->deepUpdate();

    larva->pushVariable("inflow", egg->pullVariable<double>("outflow"));
    larva->deepUpdate();

    pupa->pushVariable("inflow", larva->pullVariable<double>("outflow"));
    pupa->deepUpdate();

    double immRate = immigration->pullVariable<double>("current");
    adult->pushVariable("inflow", immRate + pupa->pullVariable<double>("outflow"));
    adult->deepUpdate();
}

} //namespace


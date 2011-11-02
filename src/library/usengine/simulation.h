/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_SIMULATION_H
#define UNISIM_SIMULATION_H

#include <QDir>
#include <QObject>
#include <QList>
#include <usbase/exception.h>
#include <usbase/named_object.h>

namespace UniSim{

class Integrator;
class Component;
class Model;
class Output;
class SimulationMaker;

class Simulation : public NamedObject
{
	Q_OBJECT
public:
    enum State {Uninitialized, Initialized, Faulty};

    Simulation(Identifier name, QString version="1.0", QObject *parent=0);
    void initialize(const Identifiers &sequence, SimulationMaker *simMaker = 0);
    virtual void execute();
    QString version() const;
    State state() const;
    const QList<Model*>& sequence() const;

    int runCount() const;
    int stepCount() const;
    void stopCurrentRun();
    void stopAllRuns();

    void setFilePath(QString filePath);
    QString inputFilePath(QString fileName);

protected:
    Integrator *_integrator;

private:
    QList<Model*> modelInstances(QString modelName);

    QList<Model*> _models;  // models ordered in sequence of execution
    QList<Output*> _outputs;
    State _state;
    QString _version;
    bool _stopCurrentRun, _stopAllRuns;
    int _runCount, _stepCount;
    QString _filePath;
    QDir _fileFolder;
};

Simulation* simulation();

} //namespace

#endif

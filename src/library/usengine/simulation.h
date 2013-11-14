/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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

class Simulation : public NamedObject
{
	//Q_OBJECT
public:
    enum State {Uninitialized, Initialized, Faulty};

    Simulation(Identifier name);
//    ~Simulation();
    void execute();
    QString version() const;
    State state() const;

    int runCount() const;
    int stepCount() const;
    void stopCurrentRun();
    void stopAllRuns();

    void setFilePath(QString filePath);
    QString inputFilePath(QString fileName);
    friend Simulation* simulation();

private:
    static Simulation *theSimulation;
    State _state;
    QString _version;
    bool _stopCurrentRun, _stopAllRuns;
    int _runCount, _stepCount;
    QString filePath;
    QDir fileFolder;
};

Simulation* simulation();

} //namespace

#endif

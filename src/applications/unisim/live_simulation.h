/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef LIVE_SIMULATION
#define LIVE_SIMULATION
#include <QMap>
#include <QObject>	
#include <QProcess>
#include <QString>


namespace UniSim {
	class Simulation;
	class SimulationMaker;
}
class LogBase;

class LiveSimulation : public QObject
{
		Q_OBJECT

public:
	enum State{
		Closed, 
		Ready, 
		Error, 
		Opening, 
		EndOpening, 
		Expanding, 
		EndExpanding, 
        Initializing,
        EndInitializing,
		ViewBuilding, 
		EndViewBuilding, 
		Running, 
		EndRunning, 
        Closing
	};
	
    LiveSimulation(QObject *parent);
	~LiveSimulation();
    void open(QString filePath);
    void writeGraph();
    void run();
    void close();

    State state() const;
	UniSim::Simulation* simulation();
	static QString stateText(State state);
    QString graphFilePath() const;
		
signals:
    void stateChanged(int oldState, int newState);
	
private slots:
    // Incoming events
    void acceptBeginExpansion();
    void acceptEndExpansion();
    void acceptBeginInitialization();
    void acceptEndInitialization();
    void graphProcessChanged(QProcess::ProcessState state);

private:
	// Methods
    void checkFilePath(QString filePath);
    void parseFile();

    void createGraphProcess();
    bool establishNullProcess(QProcess *process);

    void setupStateTexts();
	void changeState(State newState);

	// Data
    State _state;
    UniSim::Simulation *_simulation;
    UniSim::SimulationMaker *simulationMaker;
    QString filePath, _graphFilePath;
    QList<QProcess *> graphProcesses;
    static QMap<State, QString> stateTexts;
};

#endif

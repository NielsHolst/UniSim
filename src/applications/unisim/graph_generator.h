/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UniSim_GRAPH_GENERATOR_H
#define UniSim_GRAPH_GENERATOR_H

#include <QObject>
#include <QProcess>

class QFile;
namespace UniSim {
    class Simulation;
}

//! \addtogroup UniSimSimulator
//@{
class GraphGenerator : public QObject
{
	Q_OBJECT
	
public:

        GraphGenerator(UniSim::Simulation *simulation, QString outputPath);
	~GraphGenerator();
	
	//! Returns a QProcess which the caller must not delete until it has finished
	QProcess* generate();
	void generateDot();
	QProcess* generatePng();
			
private:
	// Methods
	void writeModel(QFile *f, QObject *parent, QObject *child, int parentNumber);
	
	// Data
	UniSim::Simulation *_simulation;
	QString _UniSimFilePath, _dotToolPath, _outputPath, _dotFilePath;
	
	QProcess _process;
	int _nodeNumber;
};
//@}
#endif

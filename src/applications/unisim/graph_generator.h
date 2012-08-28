/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UniSim_GRAPH_GENERATOR_H
#define UniSim_GRAPH_GENERATOR_H

#include <QMap>
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
    GraphGenerator(UniSim::Simulation *simulation);

    typedef enum {Postscript, PNG, SVG, Dot} OutputFormat;

	//! Returns a QProcess which the caller must not delete until it has finished
    QProcess* generate(OutputFormat format);
    QString outputFilePath(OutputFormat format);

private:
	// Methods
    QString dotCommand();
    void writeDotFile();
    void writeOutputFile(OutputFormat format);
    void writeModel(QFile *f, QObject *parent, QObject *child, int parentNumber);
	
	// Data
    UniSim::Simulation *simulation;
    bool dotFileOk;
    QMap<OutputFormat, QString> dotOption, fileType;
    int nodeNumber;
};
//@}
#endif

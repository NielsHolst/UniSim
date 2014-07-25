/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iomanip>
#include <iostream>
//#include <limits>
//#include <cmath>
#include <QDate>
//#include <QFile>
//#include <QFileInfo>
#include <QMap>
//#include <QObject>
//#include <QStringList>
#include <QXmlStreamWriter>
#include "exception.h"
//#include "model.h"
#include "utilities.h"

/*! \namespace UniSim
    \brief Contains all classes, global functions and definitions.
*/

using namespace std;

namespace UniSim {

//
// File handling
//

//! Returns the full path for the file with given file name
/*! The file is searched in the order:
    1. The given home directory or the subFolder of that
    2. In the parent folder or the subFolder of that
    3. In the grandparent folder or the subFolder of that, and so on
    If the file is not found, a file path in the home directory is returned, in which case the returned
    full path refers to a non-existing file
*/
QFileInfo findNearestFile(QDir home, QString subFolder, QString fileName) {
    QDir dir = home;
    while (true) {
        QString filePath[2];
        filePath[0] = dir.absolutePath() + "/" + fileName;
        filePath[1] = dir.absolutePath() + "/" + subFolder + "/" + fileName;
        if (QFileInfo(filePath[0]).exists()) {
            break;
        }
        if (QFileInfo(filePath[1]).exists()) {
            dir.cd(subFolder);
            break;
        }
        if (!dir.cdUp()) {
            dir = home;
            break;
        }
    }
    return QFileInfo(dir.filePath(fileName));
}

//
// Mathematics
//

double accum(const QVector<double> &x) {
    double sum = 0.;
    const double *p = x.data();
    int n = x.size();
    for (int i = 0; i < n; ++i, ++p)
        sum += *p;
    return sum;
}

void increment(QVector<double> &x, const QVector<double> &toAdd) {
    int n = x.size();
    Q_ASSERT(n == toAdd.size());
    increment(const_cast<double*>(x.data()), toAdd.data(), n);
}

void increment(double *x, const double *toAdd, int n) {
    for (int i = 0; i < n; ++i, ++x, ++toAdd)
        *x += *toAdd;
    }

QVector<double> add(const QVector<double> &x, const QVector<double> &y) {
    QVector<double> v;
    int n = x.size();
    Q_ASSERT(n == y.size());
    v.resize(n);
    double *destination = const_cast<double*>( v.data() );
    const double *source1 = x.data();
    const double *source2 = y.data();
    for (int i = 0; i < n; ++i, ++destination, ++source1, ++source2)
        *destination = *source1 + *source2;
    return v;
}

void scale(QVector<double> &x, double factor) {
    double *p = const_cast<double*>(x.data());
    int n = x.size();
    for (int i = 0; i < n; ++i, ++p)
        *p *= factor;
}

//! Power function that tolerates x equal to zero
double pow0(double x, double c, QObject *context) {
    if (x == 0) {
        if (c >= 0) return 0.;
        QString msg = "Cannot raise zero to power of '%1'";
        throw Exception(msg.arg(c), context);
    }
    else
        return std::pow(x, c);
}

//! Returns exp(-x) where x>=0
/*! Also works for extreme values
*/
double negExp(double x)
{
    return (x > MAX_EXP) ? 0. : (x < MIN_EXP) ? 1. : exp(-x);
}

//! Safe floating point division with bound on result
double divBounded(double x, double y, double bound) {
    if (y==0)
        return bound;
    bool positive_xy = (x>0. && y>0.) || (x<0. && y<0.);
    if (positive_xy) {
        if (bound>0. && x/bound > y)
            return bound;
        else
            return x/y;
    }
    else {
        if (bound<0. && x/bound < y)
            return bound;
        else
            return x/y;
    }
}

//! Gutierrez-Baumgaertner functional response
double GBFuncResp(double demand, double resource) {
    if (demand < 0 ||  resource < 0)
        throw Exception("Illegal arguments to GBFuncResp(d,s), "
                        "d = " + QString::number(demand) +
                        "s = " + QString::number(resource));

    if (demand <= numeric_limits<double>::epsilon())
        return 0.;

    double supply = demand*(1. - negExp(resource/demand));
    return supply;
}

//! Returns the Julian day (1..365)
int toDayOfYear(int day, int month) {
    QDate date(2001,month, day);
    return date.dayOfYear();
}

//! Convert time from one unit to another
double convertTime(double time, char fromUnit, char toUnit, QObject *context) {
    static QMap<char, int> u;
    if (u.isEmpty()) {
        u['s'] = 1;
        u['m'] = 60;
        u['h'] = 60*60;
        u['d'] = 60*60*24;
        u['y'] = 60*60*24*365;
    }
    QString msg = "Time units must be 's', 'm', 'h', 'd' or 'y', not '%1'";
    if (!u.contains(fromUnit))
        throw Exception(msg.arg(fromUnit), context);
    if (!u.contains(toUnit))
        throw Exception(msg.arg(toUnit), context);
    return time*u.value(fromUnit)/u.value(toUnit);
}

QString fullName(const QObject *object) {
    if (!object) return QString();
    QString name = object->objectName();
    if (name.isEmpty() || name.toLower() == "anonymous")
        name = QString("unnamed[") +
               object->metaObject()->className() + "]";
    return fullName(object->parent()) + "/" + name;
}

void splitAtNamespace(QString s, QString *namespacePart, QString *ownNamePart) {
    Q_ASSERT(namespacePart && ownNamePart);
    int i = s.lastIndexOf("::");
    if (i == -1) {
        *namespacePart = QString();
        *ownNamePart = s;
    }
    else {
        *namespacePart = s.left(i);
        *ownNamePart = s.right(s.size() - i - 2);
    }
}

//QStringList splitParentChildExpression(QString expression_, QObject *context) {
//    QString expression = expression_.trimmed();
//    int begin = expression.indexOf('[');
//    int end = expression.indexOf(']');
//    QString msg;
//    if (begin == -1)
//        msg = "Missing '[' in expression";
//    else if (end == -1)
//        msg = "Missing ']' in expression";
//    else if (end < expression.size() - 1)
//        msg = "Expression must end with ']'";
//    else if (begin == 0)
//        msg = "Expression misses parent name before '['";
//    else if (end - begin == 1)
//        msg = "Expression misses child name inside the brackets";
//    if (!msg.isEmpty()) {
//        msg += ": '" + expression + "'";
//        throw Exception(msg, context);
//    }
//    QString parent = expression.left(begin).trimmed();
//    QString child = expression.mid(begin + 1, end - begin -1).trimmed();

//    QStringList result;
//    result.append(parent);
//    result.append(child);
//    return result;
//}


//! Write object tree to std::cout
/*!
    Write the name of an object and all its descendants on std::cout (intended for debugging).
    \param root		the root of the tree to be written
    \param level	indentation (spaces) written before the object's name
*/
void writeObjectTree(QObject *obj, int level) {
	if (level > 0) cout << setw(2*level) << " ";
	
	cout
		<< obj->metaObject()->className() 
		<< " '" << qPrintable(obj->objectName()) << "'\n";
		
	for (QList<QObject*>::const_iterator ch = obj->children().begin(); ch != obj->children().end(); ++ch) {
		writeObjectTree(*ch, level+1);
	}
}	

//! \cond
namespace local {
	
	void writeIntegrator(QXmlStreamWriter *xw)
	{
		xw->writeStartElement("integrator");
        xw->writeAttribute("name", "integrator");
        xw->writeAttribute("type", "Steps");
		xw->writeEmptyElement("parameter");
        xw->writeAttribute("name", "maxSteps");
		xw->writeAttribute("value", "365");

        xw->writeStartElement("model");
        xw->writeAttribute("name", "calendar");
        xw->writeAttribute("type", "Calendar");
        xw->writeEndElement();

		xw->writeStartElement("sequence");
		xw->writeEmptyElement("model");
		xw->writeAttribute("name", "world");
		xw->writeEmptyElement("model");
		xw->writeAttribute("name", "wasp");
		xw->writeEmptyElement("model");
		xw->writeAttribute("name", "butterfly");
		xw->writeEndElement();

		xw->writeEndElement();
	}

	void writeWorld(QXmlStreamWriter *xw)
	{
		xw->writeStartElement("model");
		xw->writeAttribute("name", "world");
		xw->writeAttribute("type", "ConstantWorld");
		xw->writeEmptyElement("parameter");
		xw->writeAttribute("name", "temperature");
		xw->writeAttribute("value", "20");
		xw->writeEndElement();
	}
	
	void writeStage(QXmlStreamWriter *xw, QString name, double duration, unsigned k)
	{
		xw->writeStartElement("model");
		xw->writeAttribute("type", "LifeStage");
		xw->writeAttribute("name", name);
		
		xw->writeEmptyElement("parameter");
		xw->writeAttribute("name", "duration");
		xw->writeAttribute("value", QString::number(duration));
	
		xw->writeEmptyElement("parameter");
		xw->writeAttribute("name", "k");
		xw->writeAttribute("value", QString::number(k));
		
		xw->writeEndElement();
	}
	
	void writeModel(QXmlStreamWriter *xw)
	{
				
        // Butterfly
		xw->writeStartElement("model");
		xw->writeAttribute("type", "LifeCycle");
		xw->writeAttribute("name", "butterfly");
		
		writeStage(xw, "egg", 1.1, 5);
		writeStage(xw, "larva", 2.3, 10);
		writeStage(xw, "pupa", 5.8, 20);
		writeStage(xw, "adult", 13.21, 40);
		xw->writeEndElement();
		
		// Wasp
		xw->writeStartElement("model");
		xw->writeAttribute("type", "LifeCycle");
		xw->writeAttribute("name", "wasp");
		
		writeStage(xw, "egg", 5, 15);
		writeStage(xw, "larva", 14, 30);
		writeStage(xw, "pupa", 3, 20);
		writeStage(xw, "adult", 90, 10);
		xw->writeEndElement();

        // Extra
        xw->writeStartElement("model");
        xw->writeAttribute("select", "../model[@name='butterfly']");
        xw->writeAttribute("name", "extra");
        xw->writeEndElement();
    }

    void writeVariable(QXmlStreamWriter *xw, QString label, QString axis, QString value) {
        xw->writeEmptyElement("variable");
        xw->writeAttribute("label", label);
        xw->writeAttribute("axis", axis);
        xw->writeAttribute("value", value);
	}

    void writePlot(QXmlStreamWriter *xw, QString insect) {
		xw->writeStartElement("output");
		xw->writeAttribute("name", insect+"PhasePlot");
		xw->writeAttribute("type", "plot");

        xw->writeEmptyElement("parameter");
        xw->writeAttribute("name", "title");
        xw->writeAttribute("value", insect+" phase plot");

        writeVariable(xw, "larva", "x", insect + "/egg[contents]");
        writeVariable(xw, "larva", "y", insect + "/adult[contents]");

        xw->writeEndElement();
    }
	
	void writeTable(QXmlStreamWriter *xw, QString insect) {
		xw->writeStartElement("output");
		xw->writeAttribute("name", insect+"Table");
		xw->writeAttribute("type", "table");

        xw->writeEmptyElement("parameter");
        xw->writeAttribute("name", "filename");
        xw->writeAttribute("value", "unit_test_output.prn");

        writeVariable(xw, "larva", "x", insect + "/egg[contents]");
        writeVariable(xw, "larva", "y", insect + "/adult[contents]");

        xw->writeEndElement();
	}
	
	void writeOutput(QXmlStreamWriter *xw, QString insect)
	{
        // writePlot(xw, insect);
        // A plot needs mainWindow (with plotWidget) to run
        writeTable(xw, insect);
	}

	void writeSimulation(QXmlStreamWriter *xw)
	{
		xw->setAutoFormatting(true);
		xw->setCodec("ISO-8859-1");
		xw->writeStartDocument();
	
		xw->writeStartElement("simulation");
		xw->writeAttribute("version", "1.0");
	
		writeIntegrator(xw);
		writeWorld(xw);
		writeModel(xw);
		writeOutput(xw, "butterfly");
		writeOutput(xw, "wasp");
	
		xw->writeEndElement();
		xw->writeEndDocument();
	}	
	
} //namespace
//! \endcond

//! Write standard UniSim file for test purposes

void writeStandardTestFile(QString filePath)  {
	QFile file(filePath);
	bool ok = file.open(QFile::WriteOnly | QFile::Text);
    Q_ASSERT_X(ok, "In utilities.h (writeStandardTestFile)",
               qPrintable("Could not open file for writing: " + filePath));
	
	QXmlStreamWriter *xw = new QXmlStreamWriter(&file);
	local::writeSimulation(xw);
	file.close();
	delete xw;
}



} // namespace

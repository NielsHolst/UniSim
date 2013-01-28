/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iomanip>
#include <iostream>
#include <cmath>
#include <QDate>
#include <QFile>
#include <QFileInfo>
#include <QMap>
#include <QObject>
#include <QStringList>
#include <QXmlStreamWriter>
#include "exception.h"
#include "model.h"
#include "utilities.h"

/*! \namespace UniSim
    \brief Contains all classes, global functions and definitions.
*/

using namespace std;

namespace UniSim {

//
// Navigation
//

//! \cond
QObject *SimulationObject::simulation = 0;
//! \endcond

//! Sets simulation object directly
void setSimulationObject(QObject *simulation) {
    SimulationObject::simulation = simulation;
}

//! Sets simulation object from descendent
/*! The parent line of the decendent is followed until an object of Simulation class is found.
    Or else the simulation object is set to null.
*/
void setSimulationObjectFromDescendent(QObject *descendent) {
    QObject *child = descendent;
    QObject *parent = child ? child->parent() : 0;
    while (parent) {
        child = parent;
        parent = child->parent();
    }
    SimulationObject::simulation = child;
}

//! Returns Simulation object
/*! Throws Exception if the Simulation object has not been set.
*/
QObject* simulationObject() {
    if (!SimulationObject::simulation)
        throw Exception("Simulation object has not been set");
    return SimulationObject::simulation;
}

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


//! Interpolate from (x,y) table
/*! Interpolates a y-value from the given x-value. For x-values less than the first x-value in the table,
the first y-value is returned. For x-values greater then the last x-value in the table, the last y-value
is returned.
*/
double interpolate(const QMap<int, double> xy, int x) {
    if (xy.size() == 0) return 0;
    double lastY = xy[0];

    QMap<int, double>::const_iterator low = xy.begin();
    for (QMap<int, double>::const_iterator high = xy.begin();
                                           high != xy.end();
                                           ++high)
    {
        if (x < high.key()) {
            if (high.key() == low.key()) return low.value();
            return double(x - low.key())/(high.key() - low.key())*(high.value() - low.value()) + low.value();
        }
        lastY = high.value();
        low = high;
    }
    return lastY;
}

//! Power function that tolerates x equal to zero
double pow0(double x, double c) {
    return (x == 0) ? 0. : std::pow(x, c);
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

//! Gutierrez-Baumgärtner functional response
double GBFuncResp(double demand, double supply) {
    if (demand < 0 ||  supply < 0)
        throw Exception("Illegal arguments to GBFuncResp(d,s), "
                        "d = " + QString::number(demand) +
                        "s = " + QString::number(supply));

    if (demand <= DBL_EPSILON)
        return 0.;

    double res = demand*(1. - negExp(supply/demand));
    return (res > supply) ? supply : res;
}

//! Returns the Julian day (1..365)
int toDayOfYear(int day, int month) {
    QDate date(2001,month, day);
    return date.dayOfYear();
}

//
// String handling
//


    void chopParentheses(QString &s, QObject *concerning) {
        if (s.left(1) != "(")
            throw UniSim::Exception("Value list miss left parenthesis: " + s, concerning);
        s.remove(0, 1);

        if (s.right(1) != ")")
            throw UniSim::Exception("Value list miss right parenthesis: " + s, concerning);

        s.chop(1);
        s = s.simplified();
    }

    void chopRightParenthesis(QString &s, QObject *concerning) {
        if (s.right(1) != ")")
            throw Exception("Missing right parenthesis: (" + s, concerning);
        s.chop(1);
        s = s.simplified();
    }


QString fullName(const QObject *object) {
    if (!object) return QString();
    QString name = object->objectName();
    if (name.isEmpty() || name.toLower() == "anonymous")
        name = QString("unnamed[") +
               object->metaObject()->className() + "]";
    return fullName(object->parent()) + "/" + name;
}


//! Parses a simple list "(A B C)"
/*! Throws an Exception if the list is ill-formated or is empty.
*/
QStringList decodeSimpleList(QString parenthesizedList, QObject *concerning) {
    QString s = parenthesizedList.simplified();
    if (s.size() == 0)
        return QStringList();
    if (s[0] != '(') {
        if (s.endsWith(')'))
            throw UniSim::Exception("Value list misses left parenthesis", concerning);
        else
            return QStringList() << s;
    }
    chopParentheses(s, concerning);
    return s.split(" ", QString::SkipEmptyParts);
}


namespace {

    bool nextItem(QString *s, QString *item, QObject *context) {
        *s = s->trimmed();
        if (s->isEmpty())
            return false;

        if (s->startsWith("(")) {
            int left = 1, right = 0, n = s->size();
            int i = 1;
            while (i < n && left != right) {
                if (s->at(i) == '(')
                    ++left;
                else if (s->at(i) == ')')
                    ++right;
                ++i;
            }
            if (left != right)
                throw Exception("Unmatched parentheses in " + *s, context);
            int end = i - 1;
            *item = s->mid(0, end+1);
            s->remove(0, end+1);
            return true;
        }

        if (s->startsWith(")"))
            throw Exception("Unmatched parentheses in " + *s, context);

        int end = s->indexOf(QRegExp("[ ()]"));
        *item = s->mid(0, end);
        if (end==-1) {
            s->clear();
		}
        else {
			if (s->at(end) == ')')
				throw Exception("Surplus ')' in" + *s, context);
            s->remove(0, end+1);
		}
        return true;

    }
}

QStringList decodeList(QString s_, QObject *context) {
    QStringList result;
    QString s = s_.trimmed();

    if (!s.startsWith("("))
        throw Exception("Missing '(' at beginning of " + s, context);
    if (!s.endsWith(")"))
        throw Exception("Missing ')' at end of " + s, context);
    s.remove(0,1);
    s.chop(1);

    QString item;
    while (nextItem(&s, &item, context))
        result.append(item);

    return result;
}

//
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

QStringList splitParentChildExpression(QString expression) {
    int begin = expression.indexOf('[');
    int end = expression.indexOf(']');
    QString msg;
    if (begin == -1)
        msg = "Missing '[' in expression";
    else if (end == -1)
        msg = "Missing ']' in expression";
    else if (end < expression.size() - 1)
        msg = "Expression must end with ']'";
    else if (begin == 0)
        msg = "Expression misses parent name before '['";
    else if (end - begin == 1)
        msg = "Expression misses child name inside the brackets";
    if (!msg.isEmpty()) {
        msg += ": '" + expression + "'";
        throw (Exception(msg));
    }
    QString parent = expression.left(begin).trimmed();
    QString child = expression.mid(begin + 1, end - begin -1).trimmed();

    QStringList result;
    result.append(parent);
    result.append(child);
    return result;
}

template<> QString stringToValue<QString>(QString s_, QObject *) {
    return s_;
}

template<> double stringToValue<double>(QString s_, QObject *concerning) {
    QString s = s_.trimmed();
    bool ok;
    double value = s.toDouble(&ok);
    if (!ok) {
        QString msg = "Cannot convert '" + s + "' to double";
        throw Exception(msg, concerning);
    }
    return value;
}

template<> int stringToValue<int>(QString s_, QObject *concerning) {
    QString s = s_.trimmed();
    bool ok;
    int value = s.toInt(&ok);
    if (!ok) {
        QString msg = "Cannot convert '" + s + "' to int";
        throw Exception(msg, concerning);
    }
    return value;
}

template<> char stringToValue<char>(QString s_, QObject *concerning) {
    QString s = s_.trimmed();
    if (s.size() != 1) {
        QString msg = "Cannot convert '" + s + "' to char";
        throw Exception(msg, concerning);
    }
    return s[0].toLatin1();
}

template<> bool stringToValue<bool>(QString s_, QObject *concerning) {
    QString s = s_.trimmed().toLower();
    bool value;
    if (s=="yes" || s=="true")
        value = true;
    else if (s=="no" || s=="false")
        value = false;
    else {
        QString msg = "Cannot convert '" + s + "' to bool";
        throw Exception(msg, concerning);
    }
    return value;
}

template<> QDate stringToValue<QDate>(QString s, QObject *concerning) {
    QDate date = QDate::fromString(s.trimmed(), "d/M/yyyy");
    if (!date.isValid())
        date = QDate::fromString(s.trimmed(), "d.M.yyyy");
    if (!date.isValid()) {
        QString msg = "Cannot convert '" + s + "' to a date";
        throw Exception(msg, concerning);
    }
    return date;
}

template<> QTime stringToValue<QTime>(QString s_, QObject *concerning) {
    QString s = s_.trimmed();
    QTime time = QTime::fromString(s, "h:m:s");
    if (!time.isValid())
            time = QTime::fromString(s, "h:m");
    if (!time.isValid()) {
        QString msg = "Cannot convert '" + s + "' to a time";
        throw Exception(msg, concerning);
    }
    return time;
}

template<> QString valueToString<char>(char value) {
    return QString(value);
}

template<> QString valueToString<bool>(bool value) {
    return value ? "yes" : "no";
}

template<> QString valueToString<QDate>(QDate value) {
    return value.toString("d/M/yyyy");
}

template<> QString valueToString<QTime>(QTime value) {
    return value.toString("0:0:0");
}

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

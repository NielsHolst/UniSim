/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <qfileinfo.h>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/utilities.h>
#include "output_crosstab.h"

using std::max;

namespace UniSim{
	
OutputCrosstab::OutputCrosstab(Identifier name, QObject *parent)
    : OutputTable(name, parent)
{
    fileType = FileLocationInfo::Temporary;
}

void OutputCrosstab::amend() {
    OutputTable::amend();
    Models ancestors;
    for (int i = 0; i < yTraces().size(); ++i) {
        TraceRecord &rec( yTraces()[i] );
        QStringList items = rec.label.split("/");
        int numAncestors = items.size() - 1;
        if (numAncestors == 0) continue;

        Models myAncestors;
        Model *parent = dynamic_cast<Model*>( rec.trace->traceParent() );
        Q_ASSERT(parent);
        int ixAncestor = 0;
        while (ixAncestor < numAncestors) {
            myAncestors << parent;
            parent = parent->seekParent<Model*>("*");
            ++ixAncestor;
        }
        extendAncestors(&ancestors, &myAncestors, rec.label);
    }
    for (int i = 0; i < ancestors.size(); ++i) {
        QString nsName, className;
        splitAtNamespace(ancestors[i]->classId().label(), &nsName, &className);
        ancestorClassNames << className;
    }
}

void OutputCrosstab::extendAncestors(
        Models *ancestors,
        const Models *myAncestors,
        QString traceLabel)
{
    const Models *shorter, *longer;
    if (ancestors->size() < myAncestors->size()) {
        shorter = ancestors;
        longer = myAncestors;
    }
    else {
        shorter = myAncestors;
        longer = ancestors;
    }
    int n = shorter->size();
    for (int i = 0; i < n; ++i) {
        Identifier class1 = shorter->at(i)->classId();
        Identifier class2 = longer->at(i)->classId();
        if (class1 != class2) {
            QString msg = "Traces must belong to same model hierarchy: "
                          "trace '%1' disagrees with earlier trace; "
                          "conflicting classes: '%2' and '%3'";
            throw Exception(msg.arg(traceLabel).arg(class1.label()).arg(class2.label()));
        }
    }
    if (shorter == ancestors) {
        int offset = shorter->size();
        int extend = longer->size() - offset;
        for (int i = 0; i < extend; ++i) {
            int ix = offset + i;
            *ancestors << longer->at(ix);
        }
    }
}

void OutputCrosstab::debrief() {
    OutputTable::debrief();
    for (int i = 0; i < outputFilePaths().size(); ++i) {
        transformFile(outputFilePaths()[i]);
    }
}

void OutputCrosstab::transformFile(QString filePath) {
    QString fileName = QFileInfo(filePath).fileName();
    QString path = FileLocations::location(FileLocationInfo::Output).absolutePath();
    QString outFilePath = path + "/" + fileName;
    QFile in(filePath), out(outFilePath);
    if (!in.open(QIODevice::Text | QIODevice::ReadOnly)) {
        QString msg("Could not open file for reading: '%1");
        throw Exception(msg.arg(filePath));
    }
    if (!out.open(QIODevice::Text | QIODevice::WriteOnly)) {
        QString msg("Could not open file for reading: '%1");
        throw Exception(msg.arg(outFilePath));
    }

    QString line;
    while (!in.atEnd()) {
        line = QString(in.readLine());
        if (line.isEmpty()) continue;
        QStringList lineItems = line.split("\t");
        out.write(qPrintable(ancestorClassNames.join("\t") + "\t" + lineItems.join("\t")));
    }

}


} //namespace


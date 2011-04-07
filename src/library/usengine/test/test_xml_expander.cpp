#include <iostream>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usbase/file_location_info.h>
#include "../xml_expander.h"
#include "test_xml_expander.h"

using namespace UniSim;
	
void TestXmlExpander::testFlatFile()
{	
    QString path = filePath("test_xml_expander_flat.xml");
    XmlExpander expander(path);
	try {
        expander.expand();
	}
	catch (const UniSim::Exception &ex) {
        QString msg = "Unexpected exception: " + ex.message();
        QFAIL(qPrintable(msg));
	}

    QCOMPARE(expander.newFileName(), path);
}

void TestXmlExpander::testOneLevelFile() {
    QString path = filePath("test_xml_expander_one_level.xml");
    XmlExpander expander(path);
    try {
        expander.expand();
    }
    catch (const UniSim::Exception &ex) {
        QString msg = "Unexpected exception: " + ex.message();
        QFAIL(qPrintable(msg));
    }

    QVERIFY(expander.newFileName() != path);
}

void TestXmlExpander::testWeb() {
    /*
    QString path = filePath("test_xml_expander_web.xml");
    XmlExpander expander(path);
    try {
        expander.expand();
    }
    catch (const UniSim::Exception &ex) {
        QString msg = "Unexpected exception: " + ex.message();
        QFAIL(qPrintable(msg));
    }

    QVERIFY(expander.newFileName() != path);
    */
}


QString TestXmlExpander::filePath(QString fileName) const {
    QDir dir = FileLocations::location(FileLocationInfo::Weather);
    dir.cdUp();
    return dir.absolutePath() + "/src/library/usengine/test/" + fileName;
}


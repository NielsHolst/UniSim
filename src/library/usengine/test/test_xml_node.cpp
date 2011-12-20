#include <iostream>
#include <QObject>
#include <QXmlStreamReader>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include <usengine/xml_node.h>
#include "test_xml_node.h"

using namespace UniSim;

void TestXmlNode::testParseNodesOneRoot() {
    QString xml =
    "<a>"
        "<b>"
            "<b1/>"
            "<b2/>"
        "</b>"
        "<d>"
            "<d1/>"
        "</d>"
        "<c/>"
    "</a>"
    ;

    XmlNode *root;
    try {
        root = XmlNode::createFromString(xml);
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }

    QCOMPARE(root->objectName(), QString("a"));
    checkParsedNodes(root);
    delete root;
}

void TestXmlNode::checkParsedNodes(QObject *a) {
    QCOMPARE(a->findChildren<QObject*>().size(), 6);
    QCOMPARE(a->children().size(), 3);

    QCOMPARE(a->children()[0]->objectName(), QString("b"));
    QCOMPARE(a->children()[2]->objectName(), QString("c"));

    QObject *b = a->children()[0];
    QCOMPARE(b->children().size(), 2);
    QCOMPARE(b->children()[0]->objectName(), QString("b1"));
    QCOMPARE(b->children()[1]->objectName(), QString("b2"));

    QObject *c = a->children()[2];
    QCOMPARE(c->children().size(), 0);
}

void TestXmlNode::testParseNodesManyRoots() {
    QString xml =
    "<b>"
        "<b1/>"
        "<b2/>"
    "</b>"
    "<d>"
        "<d1/>"
    "</d>"
    "<c/>"
    ;

    XmlNode *root;
    try {
        root = XmlNode::createFromString(xml);
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }

    QCOMPARE(root->objectName(), QString("root"));
    QCOMPARE(root->findChildren<QObject*>().size(), 6);

    checkParsedNodes(root);
    delete root;
}

void TestXmlNode::testParseWithAttributes() {
    QString strBase =
    "<model name=\"a\">"
        "<model name=\"d\">"
            "<model name=\"d1\"/>"
            "<model name=\"d2\"/>"
        "</model>"
        "<model name=\"e\"/>"
    "</model>"
    ;

    XmlNode *root;
    try {
        root = XmlNode::createFromString(strBase);
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }
    XmlNode *a = root;
    QCOMPARE(a->id(), Identifier("model"));
    QCOMPARE(a->attribute("name"), QString("a"));

    XmlNode *d2 = a->childNode(0)->childNode(1);

    QCOMPARE(d2->attribute("name"), QString("d2"));
    delete root;
}

void TestXmlNode::testParseFromFile() {
    QString xml =
    "<model name=\"a\">"
        "<model name=\"d\">"
            "<model name=\"d1\"/>"
            "<model name=\"d2\"/>"
        "</model>"
        "<model name=\"e\"/>"
    "</model>"
    ;

    XmlNode *rootStr, *rootFile;
    try {
        rootStr = XmlNode::createFromString(xml);
        rootFile = XmlNode::createFromFile(filePath("test_xml_node_parse_from_file.xml"));
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }
    QVERIFY(deepEquals(rootStr,rootFile));
}

void TestXmlNode::testParseMissingFile() {
    XmlNode *rootFile;
    bool excepted = false;
    try {
        rootFile = XmlNode::createFromFile(filePath("non_exising_file.xml"));
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestXmlNode::testWriteToFile() {
    QString xml =
    "<model name=\"a\">"
        "<model name=\"d\">"
            "<model name=\"d1\"/>"
            "<model name=\"d2\"/>"
        "</model>"
        "<model name=\"e\"/>"
    "</model>"
    ;
    QDir dir = FileLocations::location(FileLocationInfo::Temporary);
    QString filePath = dir.absolutePath() + "/test_xml_node.xml";

    XmlNode *rootStr, *rootFile;
    try {
        rootStr = XmlNode::createFromString(xml);
        rootStr->writeToFile(filePath);
        rootFile = XmlNode::createFromFile(filePath);
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }
    QVERIFY(deepEquals(rootStr,rootFile));
}

void TestXmlNode::testDeepEqualsTrue() {
    QString xml =
    "<a>"
        "<b>"
            "<b1/>"
            "<b2/>"
        "</b>"
        "<d>"
            "<d1/>"
        "</d>"
        "<c/>"
    "</a>"
    ;
    XmlNode *a, *b;
    a = XmlNode::createFromString(xml);
    b = XmlNode::createFromString(xml);
    QVERIFY(deepEquals(a,b));
    delete a;
    delete b;
}

void TestXmlNode::testDeepEqualsUnequalNodes() {
    QString xml1 =
    "<a>"
        "<b>"
            "<b1/>"
            "<b2/>"
        "</b>"
        "<d>"
            "<d1/>"
        "</d>"
        "<c/>"
    "</a>"
    ;
    QString xml2 =
    "<a>"
        "<b>"
            "<b1/>"
            "<b2/>"
            "<b3/>"
        "</b>"
        "<d>"
            "<d1/>"
        "</d>"
        "<c/>"
    "</a>"
    ;
    XmlNode *a, *b;
    a = XmlNode::createFromString(xml1);
    b = XmlNode::createFromString(xml2);
    QVERIFY(!deepEquals(a,b));
    delete a;
    delete b;
}

void TestXmlNode::testDeepEqualsUnequalAttributes() {
    QString xml1 =
    "<model name=\"x\">"
        "<model name=\"d\">"
            "<model name=\"d1\"/>"
            "<model name=\"d2\"/>"
        "</model>"
        "<model name=\"e\"/>"
    "</model>"
    ;
    QString xml2 =
    "<model name=\"x\">"
        "<model name=\"d\">"
            "<model name=\"d1\"/>"
            "<model name=\"d2\" extra=\"x\"/>"
        "</model>"
        "<model name=\"e\"/>"
    "</model>"
    ;
    XmlNode *a, *b;
    a = XmlNode::createFromString(xml1);
    b = XmlNode::createFromString(xml2);
    QVERIFY(!deepEquals(a,b));
    delete a;
    delete b;
}

void TestXmlNode::testMergeAttributes() {
    QString strBase =
    "<model name=\"m\" name1=\"x\" name2=\"y\"/>"
    ;
    QString strDerived =
    "<model name=\"m\" name1=\"z\" name3=\"xyz\" />"
    ;
    QString strMerged =
    "<model name=\"m\" name1=\"z\" name2=\"y\" name3=\"xyz\" />"
    ;

    XmlNode *rootBase, *rootDerived, *rootMerged;
    rootBase = XmlNode::createFromString(strBase);
    rootDerived = XmlNode::createFromString(strDerived);
    rootMerged = XmlNode::createFromString(strMerged);
    rootBase->merge(rootDerived);

    QVERIFY(deepEquals(rootBase,rootMerged));
    delete rootBase;
    delete rootDerived;
    delete rootMerged;
}

void TestXmlNode::testMergeNodes() {
    QString strBase =
    "<model name=\"x\" origin=\"base\" >"
        "<model name=\"d\" origin=\"base\" >"
            "<model name=\"d1\" origin=\"base\" />"
            "<model name=\"d2\"/>"
        "</model>"
        "<model name=\"e\"/>"
    "</model>"
    ;
    QString strDerived =""
    "<model name=\"x\" origin=\"derived\" extra=\"x\" >"
        "<model name=\"d1\">"
            "<model name=\"e\"/>"
        "</model>"
        "<model origin=\"derived\" name=\"d\" >"
            "<model name=\"a\"/>"
            "<model name=\"d1\" origin=\"derived\" />"
        "</model>"
        "<model name=\"x1\" extra=\"x\"/>"
        "<model name=\"x2\"/>"
    "</model>"
    ;
    QString strMerged =""
    "<model name=\"x\" extra=\"x\" origin=\"derived\"  >"
        "<model name=\"d\" origin=\"derived\" >"
            "<model name=\"d1\" origin=\"derived\" />"
            "<model name=\"d2\"/>"
            "<model name=\"a\"/>"
        "</model>"
        "<model name=\"e\"/>"
        "<model name=\"d1\">"
            "<model name=\"e\"/>"
        "</model>"
        "<model name=\"x1\" extra=\"x\"/>"
        "<model name=\"x2\"/>"
    "</model>"
    ;

    XmlNode *rootBase, *rootDerived, *rootMerged;
    rootBase = XmlNode::createFromString(strBase);
    rootDerived = XmlNode::createFromString(strDerived);
    rootMerged = XmlNode::createFromString(strMerged);
    rootBase->merge(rootDerived);

    QVERIFY(deepEquals(rootBase,rootMerged));
    delete rootBase;
    delete rootDerived;
    delete rootMerged;
}

void TestXmlNode::testBuildQueryString()  {
    QString xml =
    "<model name=\"a\">"
        "<model name=\"d\">"
            "<model name=\"d1\"/>"
            "<model name=\"d2\"/>"
        "</model>"
        "<model name=\"e\"/>"
    "</model>"
    ;

    XmlNode *root = XmlNode::createFromString(xml);
    XmlNode *d1 = root->childNode(0)->childNode(0);

    QString filePath = "C:/filepath/somefile.xml";
    QString qInsideRel, qInsideAbs, qLocal, qRemote;
    qInsideRel = "../../model[@name='a']";
    qInsideAbs = "/simulation/model[@name='a']";
    qLocal = "doc('anotherfile.xml')/simulation/model[@name='a']";
    qRemote = "doc('http://www.ecolmod.org/test/somefile.xml')/simulation/model[@name='a']";

    QString qsInsideRel, qsInsideAbs, qsLocal, qsRemote;
    qsInsideRel = d1->buildQueryString(filePath, qInsideRel);
    qsInsideAbs = d1->buildQueryString(filePath, qInsideAbs);
    qsLocal = d1->buildQueryString(filePath, qLocal);
    qsRemote = d1->buildQueryString(filePath, qRemote);

    QCOMPARE(qsInsideRel, QString("doc('C:/filepath/somefile.xml')/model[@name='a']/model[@name='d']/model[@name='d1']/../../model[@name='a']"));
    QCOMPARE(qsInsideAbs, QString("doc('C:/filepath/somefile.xml')/simulation/model[@name='a']"));
    QCOMPARE(qsLocal, QString("doc('C:/filepath/anotherfile.xml')/simulation/model[@name='a']"));
    QCOMPARE(qsRemote, QString("doc('http://www.ecolmod.org/test/somefile.xml')/simulation/model[@name='a']"));

    delete root;
}

void TestXmlNode::testCompileNoSelect() {
    const char *FN = "test_xml_node_parse_from_file.xml";
    XmlNode *uncompiled, *compiled;
    try {
        uncompiled = XmlNode::createFromFile(filePath(FN));
        compiled = XmlNode::createFromFile(filePath(FN));
        compiled->compile(filePath(FN));
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }
    QVERIFY(deepEquals(uncompiled, compiled));
}

void TestXmlNode::testCompileMissingFile() {
    XmlNode *root;
    QString xml = "<b><b1/></b>";
    root = XmlNode::createFromString(xml);
    bool excepted = false;
    try {
        root->compile(filePath("non_exising_file.xml"));
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}


void TestXmlNode::testCompileInvalidSelect() {
    QString path = filePath("test_xml_node_compile_invalid.xml");
    XmlNode *root;
    bool excepted = false;
    try {
        root = XmlNode::createFromFile(path);
        root->compile(path);
    }
    catch (Exception &ex) {
        excepted = true;
        bool invalidDetected = ex.message().contains("Could not carry out");
        QVERIFY(invalidDetected);
    }
    QVERIFY(excepted);
}

void TestXmlNode::testCompileEmptySelect() {
    QString path = filePath("test_xml_node_compile_empty.xml");
    XmlNode *root;
    bool excepted = false;
    try {
        root = XmlNode::createFromFile(path);
        root->compile(path);
    }
    catch (Exception &ex) {
        excepted = true;
        bool emptyDetected = ex.message().contains("Nothing found");
        QVERIFY(emptyDetected);
    }
    QVERIFY(excepted);
}

void TestXmlNode::testCompileOneSelect() {
    QString strMerged =
    "<simulation>"
        "<model name=\"A\" >"
             "<model name=\"x\" origin=\"base\" >"
                 "<model name=\"d\" origin=\"base\" >"
                     "<model name=\"d1\" origin=\"base\" />"
                     "<model name=\"d2\"/>"
                 "</model>"
                 "<model name=\"e\"/>"
             "</model>"
        "</model>"
        "<model name=\"B\" >"
            "<model name=\"xyz\" extra=\"x\" origin=\"derived\"  >"
                "<model name=\"d\" origin=\"derived\" >"
                    "<model name=\"d1\" origin=\"derived\" />"
                    "<model name=\"d2\"/>"
                    "<model name=\"a\"/>"
                "</model>"
                "<model name=\"e\"/>"
                "<model name=\"d1\">"
                    "<model name=\"e\"/>"
                "</model>"
                "<model name=\"x1\" extra=\"x\"/>"
                "<model name=\"x2\"/>"
            "</model>"
        "</model>"
        "<model name=\"C\" />"
    "</simulation>"
    ;


    QString path = filePath("test_xml_node_compile_one.xml");
    XmlNode *root, *merged;
    try {
        merged = XmlNode::createFromString(strMerged);
        root = XmlNode::createFromFile(path);
        root->compile(path);
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }
    QVERIFY(deepEquals(root,merged));
    delete root;
    delete merged;
}

void TestXmlNode::testCompileNestedSelect() {
    QString strMerged =
    "<simulation>"
        "<model name=\"A\" >"
            "<model name=\"local\" origin=\"base\">"
                "<model name=\"local1\"/>"
            "</model>"
             "<model name=\"x\" origin=\"base\" >"
                 "<model name=\"d\" origin=\"base\" >"
                     "<model name=\"new1\" origin=\"base\">"
                         "<model name=\"local1\"/>"
                     "</model>"
                     "<model name=\"d1\" origin=\"base\" />"
                     "<model name=\"new2\" origin=\"base\">"
                         "<model name=\"local1\"/>"
                     "</model>"
                     "<model name=\"d2\"/>"
                 "</model>"
                 "<model name=\"e\"/>"
             "</model>"
        "</model>"
        "<model name=\"B\" >"
            "<model name=\"xyz\" extra=\"x\" origin=\"derived\"  >"
                "<model name=\"d\" origin=\"derived\" >"
                    "<model name=\"new1\" origin=\"base\">"
                        "<model name=\"local1\"/>"
                    "</model>"
                    "<model name=\"d1\" origin=\"derived\" />"
                    "<model name=\"new2\" origin=\"base\">"
                        "<model name=\"local1\"/>"
                    "</model>"
                    "<model name=\"d2\"/>"
                    "<model name=\"a\"/>"
                "</model>"

                "<model name=\"e\"/>"
                "<model name=\"d1\">"
                    "<model name=\"e\"/>"
                "</model>"
                "<model name=\"x1\" extra=\"x\"/>"
                "<model name=\"x2\"/>"
            "</model>"
        "</model>"
        "<model name=\"C\" />"
    "</simulation>"
    ;

    QString path = filePath("test_xml_node_compile_nested.xml");
    XmlNode *root, *merged;
    try {
        merged = XmlNode::createFromString(strMerged);
        root = XmlNode::createFromFile(path);
        root->compile(path);
	}
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }
    QVERIFY(deepEquals(root,merged));
    delete root;
    delete merged;
}

void TestXmlNode::testCaseCirsium() {

    XmlNode *original;
    QString s;
    try {
        QString fp = filePath("case_cirsium.xml");
        original = XmlNode::createFromFile(fp);
        original->compile(fp);
        original->getTree(&s);
        // cout << qPrintable(s);
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }
}

QString TestXmlNode::filePath(QString fileName) const {
    QDir uniSimDir = QDir("../..");
    return uniSimDir.absolutePath() + "/src/library/usengine/test/input/" + fileName;
}


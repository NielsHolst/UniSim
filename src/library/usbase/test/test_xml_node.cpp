#include <iostream>
#include <QObject>
#include <QXmlStreamReader>
#include <usbase/exception.h>
#include "test_xml_node.h"
#include "xml_node.h"

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
        root = XmlNode::fromString(xml);
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }

    QCOMPARE(root->objectName(), QString("root"));
    QCOMPARE(root->findChildren<QObject*>().size(), 7);
    QCOMPARE(root->children().size(), 1);

    QObject *a = root->children()[0];
    QCOMPARE(a->objectName(), QString("a"));
    checkParsedNodes(a);
    delete root;
}

void TestXmlNode::checkParsedNodes(QObject *a) {
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
        root = XmlNode::fromString(xml);
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
        root = XmlNode::fromString(strBase);
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }
    XmlNode *a = dynamic_cast<XmlNode*>(root->children()[0]);
    QCOMPARE(a->id(), Identifier("model"));
    QCOMPARE(a->attribute("name"), QString("a"));

    XmlNode *d2 = dynamic_cast<XmlNode*>(a->children()[0]->children()[1]);

    QCOMPARE(d2->attribute("name"), QString("d2"));
    delete root;
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
    a = XmlNode::fromString(xml);
    b = XmlNode::fromString(xml);
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
    a = XmlNode::fromString(xml1);
    b = XmlNode::fromString(xml2);
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
    a = XmlNode::fromString(xml1);
    b = XmlNode::fromString(xml2);
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
    rootBase = XmlNode::fromString(strBase);
    rootDerived = XmlNode::fromString(strDerived);
    rootMerged = XmlNode::fromString(strMerged);
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
    rootBase = XmlNode::fromString(strBase);
    rootDerived = XmlNode::fromString(strDerived);
    rootMerged = XmlNode::fromString(strMerged);
    rootBase->merge(rootDerived);

    QVERIFY(deepEquals(rootBase,rootMerged));
    delete rootBase;
    delete rootDerived;
    delete rootMerged;
}

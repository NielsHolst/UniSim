#ifndef UNISIM_TEST_XML_NODE_H
#define UNISIM_TEST_XML_NODE_H

#include <QObject>
#include <QtTest/QtTest>
#include <usbase/test/autotest.h>

	
class TestXmlNode : public QObject
{
    Q_OBJECT
    
private slots:
    void testParseNodesOneRoot();
    void testParseNodesManyRoots();
    void testParseWithAttributes();

    void testDeepEqualsTrue();
    void testDeepEqualsUnequalNodes();
    void testDeepEqualsUnequalAttributes();

    void testMergeAttributes();
    void testMergeNodes();

private:
    void checkParsedNodes(QObject *a);
};

DECLARE_TEST(TestXmlNode)

#endif 

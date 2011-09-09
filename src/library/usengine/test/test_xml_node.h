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
    void testParseFromFile();
    void testParseMissingFile();
    void testWriteToFile();

    void testDeepEqualsTrue();
    void testDeepEqualsUnequalNodes();
    void testDeepEqualsUnequalAttributes();

    void testMergeAttributes();
    void testMergeNodes();

    void testBuildQueryString();
    void testCompileNoSelect();
    void testCompileMissingFile();
    void testCompileInvalidSelect();
    void testCompileEmptySelect();
    void testCompileOneSelect();
    void testCompileNestedSelect();

    void testCaseCirsium();

private:
    void checkParsedNodes(QObject *a);
    QString filePath(QString fileName) const;
};

DECLARE_TEST(TestXmlNode)

#endif 

#ifndef UNISIM_TEST_XML_EXPANDER_H
#define UNISIM_TEST_XML_EXPANDER_H

#include <usbase/test/autotest.h>

class TestXmlExpander : public QObject
{
    Q_OBJECT
private slots:
    void testFlatFile();
    void testOneLevelFile();
    void testWeb();
private:
    QString filePath(QString fileName) const;

};

DECLARE_TEST(TestXmlExpander)

#endif

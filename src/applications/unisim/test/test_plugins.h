#ifndef TEST_PLUGINS_H
#define TEST_PLUGINS_H

#include <usbase/test/autotest.h>

class TestPlugins : public QObject
{
    Q_OBJECT
private slots:
    void testCorrectXmlModels();
    void testMissingXmlModel();
};

DECLARE_TEST(TestPlugins)

#endif

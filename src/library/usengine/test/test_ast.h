#ifndef UNISIM_TEST_ENGINE_AST_H
#define UNISIM_TEST_ENGINE_AST_H

#include <usbase/test/autotest.h>

class TestAst : public QObject
{
    Q_OBJECT
private slots:
	void testOptionalParts();
    void testMissingParts();
    void testKeyValue();
    void testNestedModels();
    void testUniFile();
    void testInputNotInOrder();
    void testOutputResetNotInOrder();
    void testOutputInitNotInOrder();
};

DECLARE_TEST(TestAst)


#endif

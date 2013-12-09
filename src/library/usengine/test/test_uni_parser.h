#ifndef UNISIM_TEST_UNI_PARSER_H
#define UNISIM_TEST_UNI_PARSER_H
#include <QBuffer>
#include <usbase/test/autotest.h>

class TestUniParser : public QObject
{
    Q_OBJECT
private slots:
    void testParseOne();
    void testParseOneWs();
    void testParseMany();
    void testParseManyWs();
    void testParseSymbols();
    void testParseSymbolsWs();
    void testParseDelimiters();
    void testParseDelimitersWs();
    void testParseDelimitersEmpty();
    void testParseDelimitersSymbol();
    void testParseDelimitersSymbolEmpty();
    void testParseDelimitersCollide();
    void testParseDelimitersUnbalanced();

    void testError1();

    void testParseFileFlat();

};

DECLARE_TEST(TestUniParser)


#endif

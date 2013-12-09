#include <iostream>
#include <QDir>
#include <usbase/exception.h>
#include <usengine/uni_parser.h>
#include "test_uni_parser.h"

using namespace UniSim;

namespace {
    QString filePath(QString fileName) {
        QDir uniSimDir = QDir("../..");
        return uniSimDir.absolutePath() +
                "/src/library/usengine/test/input/" +
                fileName;
    }
}

void TestUniParser::testParseOne() {
    QStringList output, expected;
    UniParser p;
    p.parse("ABC", output);
    expected << "ABC";
    QCOMPARE(output, expected);
}

void TestUniParser::testParseOneWs() {
    QStringList output, expected;
    UniParser p;
    p.parse("  ABC\n", output);
    expected << "ABC";
    QCOMPARE(output, expected);
}

void TestUniParser::testParseMany() {
    QStringList output, expected;
    UniParser p;
    p.parse("ABC DEF GHI", output);
    expected << "ABC" << "DEF" << "GHI";
    QCOMPARE(output, expected);
}

void TestUniParser::testParseManyWs() {
    QStringList output, expected;
    UniParser p;
    p.parse(" ABC DEF GHI ", output);
    expected << "ABC" << "DEF" << "GHI";
    QCOMPARE(output, expected);
}

void TestUniParser::testParseSymbols() {
    QStringList output, expected;
    UniParser p;
    p.parse("ABC{DEF}GHI*:", output);
    expected << "ABC" << "{" << "DEF" << "}" << "GHI" << "*" << ":";
    QCOMPARE(output, expected);
}

void TestUniParser::testParseSymbolsWs() {
    QStringList output, expected;
    UniParser p;
    p.parse("ABC { DEF } GHI * : ", output);
    expected << "ABC" << "{" << "DEF" << "}" << "GHI" << "*" << ":";
    QCOMPARE(output, expected);
}

void TestUniParser::testParseDelimiters() {
    QStringList output, expected;
    UniParser p;
    p.parse("ABC \"{DEF}\" GHI * : ", output);
    expected << "ABC" << "{DEF}" << "GHI" << "*" << ":";
    QCOMPARE(output, expected);
}

void TestUniParser::testParseDelimitersWs() {
    QStringList output, expected;
    UniParser p;
    p.parse("ABC \" { DEF } \" GHI * : ", output);
    expected << "ABC" << " { DEF } " << "GHI" << "*" << ":";
    QCOMPARE(output, expected);
}

void TestUniParser::testParseDelimitersEmpty() {
    QStringList output, expected;
    UniParser p;
    p.parse("\"\" ABC \"\" DEF \"\"", output);
    expected << "" << "ABC" << "" << "DEF" << "";
    QCOMPARE(output, expected);
}

void TestUniParser::testParseDelimitersSymbol() {
    QStringList output, expected;
    UniParser p;
    p.parse("value:\" ABC \"", output);
    expected << "value" << ":" << " ABC ";
    QCOMPARE(output, expected);
}


void TestUniParser::testParseDelimitersSymbolEmpty() {
    QStringList output, expected;
    UniParser p;
    p.parse("value:\"\"", output);
    expected << "value" << ":" << "";
    QCOMPARE(output, expected);
}

void TestUniParser::testParseDelimitersCollide() {
    QStringList output;
    UniParser p;
    bool excepted = false;
    try {
        p.parse("\"ABD\"DEF", output);
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestUniParser::testParseDelimitersUnbalanced() {
    QStringList output;
    UniParser p;
    bool excepted = false;
    try {
        p.parse("ABD\"DEF", output);
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestUniParser::testError1() {
    QStringList output, expected;
    UniParser p;
//    p.parse("simulation  {	title: \"Insect phenology\"\n}", output);
    p.parse("{ \"ABC\"\n}\n\nDEF", output);
    expected << "{" << "ABC" << "}" << "DEF";
    QCOMPARE(output, expected);
}




void TestUniParser::testParseFileFlat() {
    UniParser p(filePath("test_uni_parser_flat.uni"));
    p.parse(filePath("test_uni_parser_flat.txt"));
}

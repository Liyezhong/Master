#include <QString>
#include <QtTest>

class TestProtocolParser : public QObject
{
    Q_OBJECT

public:
    TestProtocolParser();

private Q_SLOTS:
    void testCase1();
};

TestProtocolParser::TestProtocolParser()
{
}

void TestProtocolParser::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(TestProtocolParser)

#include "TestProtocolparser.moc"

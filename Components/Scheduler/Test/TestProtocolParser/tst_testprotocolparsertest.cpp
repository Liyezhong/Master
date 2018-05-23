#include <QString>
#include <QtTest>

class TestProtocolParserTest : public QObject
{
    Q_OBJECT

public:
    TestProtocolParserTest();

private Q_SLOTS:
    void testCase1();
};

TestProtocolParserTest::TestProtocolParserTest()
{
}

void TestProtocolParserTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(TestProtocolParserTest)

#include "tst_testprotocolparsertest.moc"

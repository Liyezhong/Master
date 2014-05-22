    /****************************************************************************/
/*! \file TestServiceDataManager.cpp
 *
 *  \brief Implementation file for class TestServiceDataManager.
 *
 *  \b Description:
 *         Checks the CTestServiceDataManager class implementation
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2014-5-19
 *  $Author:    $ Dixiong.Li
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include <QTest>
#include <QFile>
#include "Global/Include/Exception.h"
#include "Global/Include/Utils.h"
#include "Global/Include/SystemPaths.h"
#include "ServiceDataManager/Include/TestCaseGuide.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"

namespace DataManager {

/****************************************************************************/
/**
 * \brief Test class for TestServiceDataManager class.
 */
/****************************************************************************/
class CTestServiceDataManager : public QObject {
    Q_OBJECT
private slots:
    /****************************************************************************/
    /**
     * \brief Called before the first testfunction is executed.
     */
    /****************************************************************************/
    void initTestCase();
    /****************************************************************************/
    /**
     * \brief Called before each testfunction is executed.
     */
    /****************************************************************************/
    void init();
    /****************************************************************************/
    /**
     * \brief Called after each testfunction was executed.
     */
    /****************************************************************************/
    void cleanup();
    /****************************************************************************/
    /**
     * \brief Called after last testfunction was executed.
     */
    /****************************************************************************/
    void cleanupTestCase();

    /****************************************************************************/
    /**
     * \brief Test read test case parameters data container
     */
    /****************************************************************************/
    void TestTestCase();

    /****************************************************************************/
    /**
     * \brief Test read test case parameters data container
     */
    /****************************************************************************/
    void TestTestCaseGuide();

    /****************************************************************************/
    /**
     * \brief Test read test case factory data container
     */
    /****************************************************************************/
    void TestTestCaseFactory();

}; // end class CTestServiceDataManager

/****************************************************************************/
void CTestServiceDataManager::initTestCase() {
    Global::SystemPaths::Instance().SetSettingsPath("../../../Main/Build/Settings");
}

/****************************************************************************/
void CTestServiceDataManager::init() {
}

/****************************************************************************/
void CTestServiceDataManager::cleanup() {
}

/****************************************************************************/
void CTestServiceDataManager::cleanupTestCase() {
}

/****************************************************************************/
void CTestServiceDataManager::TestTestCase()
{
    /*CTestCase TestCase("Case1");
    TestCase.AddResult("key1", "value1");
    TestCase.AddResult("key2", "value2");

    QString StrReport("Case1:\n\tName: Case1\n\tResult: PASS\n\tkey1 : value1\n\tkey2 : value2\n");
    QCOMPARE(TestCase.GenReport(), StrReport);*/
}

void CTestServiceDataManager::TestTestCaseGuide()
{
    CTestCaseGuide *Guide = new CTestCaseGuide;
    GuideSteps OvenEmptyGuideSteps = Guide->GetGuideSteps("OvenHeatingEmpty", 0);

    QVERIFY(OvenEmptyGuideSteps.length() == 2);
    QCOMPARE(OvenEmptyGuideSteps[0], QString("the empty first step"));

    delete Guide;
}

void CTestServiceDataManager::TestTestCaseFactory()
{
    QString FileName = Global::SystemPaths::Instance().GetSettingsPath() + "/TestCaseConfig.xml";
    CTestCaseFactory::Instance().InitData(FileName);

    CTestCase* p_OvenTestEmpty = CTestCaseFactory::Instance().GetTestCase("OvenHeatingEmpty");
    QCOMPARE(p_OvenTestEmpty->GetParameter(QString("AmbTempLow")), QString("10"));
    QCOMPARE(p_OvenTestEmpty->GetParameter(QString("AmbTempHigh")), QString("40"));

    QCOMPARE(CTestCaseFactory::Instance().GetParameter("OvenHeatingWater", "TargetTemp"), QString("55"));

}

} // end namespace ImportExport

QTEST_MAIN(DataManager::CTestServiceDataManager)

#include "TestServiceDataManager.moc"

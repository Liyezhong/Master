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
#include "ServiceDataManager/Include/ModuleDataList.h"
#include "ServiceDataManager/Include/ModuleDataListAdapter.h"

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

    /****************************************************************************/
    /**
     * \brief Test read Mode data container
     */
    /****************************************************************************/
    void TestModuleDataList();

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
    QString FileName = Global::SystemPaths::Instance().GetSettingsPath() + "/TestCaseGuide.xml";
    CTestCaseGuide::Instance().InitData(FileName);

    GuideSteps OvenEmptyGuideSteps =  CTestCaseGuide::Instance().GetGuideSteps("OvenHeatingEmpty", 0);

    QVERIFY(OvenEmptyGuideSteps.length() == 1);
    QCOMPARE(OvenEmptyGuideSteps[0], QString("Please take out the wax baths."));
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

void CTestServiceDataManager::TestModuleDataList()
{
    ServiceDataManager::CModuleDataList* p_ModuleList = new ServiceDataManager::CModuleDataList;
    //DataManager::CInstrumentHistory* p_In
    QString FilenameModuleList = Global::SystemPaths::Instance().GetSettingsPath() + "/InstrumentHistory.xml";
    QVERIFY(p_ModuleList->ReadFile(FilenameModuleList));

    QVERIFY(p_ModuleList->GetNumberofModules() == 5);

    ServiceDataManager::CModule* p_Oven = p_ModuleList->GetModule("Paraffine Oven");
    QVERIFY(p_Oven);

    ServiceDataManager::CSubModule* p_Oven_Heater = p_Oven->GetSubModuleInfo("Heater");
    QVERIFY(p_Oven_Heater);
    QCOMPARE(p_Oven_Heater->GetSubModuleType(), QString("Heater"));
    //QVERIFY(p_Oven_Heater->GetNumberOfParameters() == 6);

    QString OperationTime1 = p_Oven_Heater->GetParameterInfo("OperationTime")->ParameterValue;

    ServiceDataManager::CModuleDataListAdapter* p_ModuleList_Adapter = new ServiceDataManager::CModuleDataListAdapter(p_ModuleList);
    QVERIFY(p_ModuleList_Adapter->Run());

    QString OperationTime2 = p_Oven_Heater->GetParameterInfo("OperationTime")->ParameterValue;

    //QVERIFY(OperationTime1 != OperationTime2);
}

} // end namespace ImportExport

QTEST_MAIN(DataManager::CTestServiceDataManager)

#include "TestServiceDataManager.moc"

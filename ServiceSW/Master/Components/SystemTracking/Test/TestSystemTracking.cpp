/****************************************************************************/
/*! \file TestSystemTracking.cpp
 *
 *  \brief Unit test for SystemTracking menu
 *
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-08-05
 *  $Author:    $ Soumya. D
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include <QtTest/QTest>
#include <QDebug>
#include <SystemTracking/Include/AddModifyConfiguration.h>
//#include <SystemTracking/Include/AddModifyConfigurationTwo.h>
//#include <SystemTracking/Include/CoverSlipper.h>
//#include <SystemTracking/Include/CurrentConfigParameterDlg.h>
#include <SystemTracking/Include/CurrentConfiguration.h>
#include <SystemTracking/Include/CurrentConfigurationDlg.h>
//#include <SystemTracking/Include/ModuleWithBoardDlg.h>
//#include <SystemTracking/Include/ModuleWithoutBoardDlg.h>
#include <SystemTracking/Include/ViewHistory.h>
//#include <SystemTracking/Include/ViewHistoryDiffDlg.h>
#include <SystemTracking/Include/ViewHistoryDlg.h>
//#include <ServiceDataManager/Include/ModuleDataListArchive.h>
#include <QObject>

namespace SystemTracking {

/****************************************************************************/
/**
 * \brief Test class for SystemTracking class.
 */
/****************************************************************************/
class CTestSystemTracking : public QObject {
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
     * \brief Test SystemTracking Menu object
     */
    /****************************************************************************/
    void utTestSystemTracking();

}; // end class CTestSystemTracking

/****************************************************************************/
void CTestSystemTracking::initTestCase() {
}

/****************************************************************************/
void CTestSystemTracking::init() {
}

/****************************************************************************/
void CTestSystemTracking::cleanup() {
}

/****************************************************************************/
void CTestSystemTracking::cleanupTestCase() {
}

/****************************************************************************/
void CTestSystemTracking::utTestSystemTracking() {
//    MainMenu::CMainWindow *p_MainWindow = new MainMenu::CMainWindow();
//    Core::CServiceGUIConnector *p_ServiceGUIConnector = new Core::CServiceGUIConnector(p_MainWindow);
//    SystemTracking::CAddModifyConfig *p_AddModifyConfig = new
//            SystemTracking::CAddModifyConfig(p_ServiceGUIConnector, p_MainWindow);

//    SystemTracking::CAddModifyConfigTwo *p_AddModifyConfigTwo = new
//            SystemTracking::CAddModifyConfigTwo(p_ServiceGUIConnector, p_MainWindow);

//    SystemTracking::CCoverSlipper *p_CoverSlipper = new
//            SystemTracking::CCoverSlipper(p_ServiceGUIConnector, p_MainWindow);

//    SystemTracking::CCurrentConfiguration *p_CurrentConfiguration = new
//            SystemTracking::CCurrentConfiguration(p_ServiceGUIConnector, p_MainWindow);

//    SystemTracking::CCurrentConfigParameterDlg *p_CurrentConfigParameterDlg = new
//            SystemTracking::CCurrentConfigParameterDlg(p_MainWindow);

//    SystemTracking::CCurrentConfigurationDlg *p_CurrentConfigDlg = new
//            SystemTracking::CCurrentConfigurationDlg(p_MainWindow);

//    SystemTracking::CModuleWithBoardDlg *p_ModuleWithBoardDlg = new
//            SystemTracking::CModuleWithBoardDlg(p_ServiceGUIConnector, p_MainWindow);

//    SystemTracking::CModuleWithoutBoardDlg *p_ModuleWithoutBoardDlg = new
//            SystemTracking::CModuleWithoutBoardDlg(p_ServiceGUIConnector, p_MainWindow);

//    SystemTracking::CViewHistory *p_ViewHistory = new
//            SystemTracking::CViewHistory(p_ServiceGUIConnector, p_MainWindow);

////    SystemTracking::CViewHistoryDiffDlg *p_ViewHistoryDiffDlg = new
////            SystemTracking::CViewHistoryDiffDlg(p_ServiceGUIConnector, MainWindow);

////    SystemTracking::CViewHistoryDlg *p_ViewHistoryDlg = new
////            SystemTracking::CViewHistoryDlg(p_ServiceGUIConnector, MainWindow);

//    p_AddModifyConfig->ButtonSelected(0);

//    p_AddModifyConfigTwo->EnableFinalizeConfigButton();

//    QString LineEditString("SN_COVERSLIPPER_0001");
//    p_CoverSlipper->SetModuleName("Cover Slipper");
//    p_CoverSlipper->SetLineEditString(LineEditString);
//    QCOMPARE(p_CoverSlipper->GetLineEditString(), QString("SN_COVERSLIPPER_0001"));

//    QString ModuleName("Drawer Loader");
//    p_CurrentConfiguration->AddItem(ModuleName);

//    DataManager::Parameter_t *p_Parameter = new DataManager::Parameter_t;
//    p_Parameter->ParameterName = QString("Serial Number");
//    p_Parameter->ParameterUnit = QString("QString");
//    p_Parameter->ParameterValue = QString("SN");
//    p_CurrentConfigParameterDlg->InitDialog(p_Parameter);
//    p_CurrentConfigParameterDlg->AddItem(p_Parameter->ParameterName, p_Parameter->ParameterValue);
//    p_CurrentConfigParameterDlg->ClearModel();

//    DataManager::CModule *p_Module = new DataManager::CModule();
//    p_Module->SetSerialNumber(QString("SN_DRAWERLOADER_0001"));
//    p_Module->SetOperatingHours(QString("1:07:23:076"));
//    p_Module->SetTestResult(QString("PASS"));
//    p_Module->SetTestDate(QString("2013-07-23"));
//    p_Module->SetCalibrationResult(QString("PASS"));
//    p_Module->SetCalibrationDate(QString("2013-06-05"));
//    p_CurrentConfigDlg->InitDialog(p_Module);
//    p_CurrentConfigDlg->AddItem("asb4_0");

//    p_ModuleWithBoardDlg->ClearLineEdit();
//    p_ModuleWithBoardDlg->ManualFillModuleInformation();
//    p_ModuleWithBoardDlg->EnableDisableRemoveButton(true);

//    p_ModuleWithoutBoardDlg->ClearLineEdit();

//    QString ModuleTimeStamp("2012-08-12 09:19:49.611");
//    p_ViewHistory->AddItem(ModuleTimeStamp);

////    p_ViewHistoryDiffDlg->InitDialog(ModuleTimeStamp, ModuleTimeStampTwo);

////    DataManager::CModuleDataListArchive *p_ModuleDataListArchive = new DataManager::CModuleDataListArchive();
////    p_ModuleDataListArchive = p_ServiceGUIConnector->GetModuleListArchiveContainer()->GetModuleList(0);
////    p_ViewHistoryDlg->InitDialog(p_ModuleDataListArchive);
////    p_ViewHistoryDlg->AddItem(p_ModuleDataListArchive);

}

} // end namespace SystemTracking

QTEST_MAIN(SystemTracking::CTestSystemTracking)
#include "TestSystemTracking.moc"


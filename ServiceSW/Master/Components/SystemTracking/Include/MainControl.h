/****************************************************************************/
/*! \file MainControl.h
 *
 *  \brief Declaration of Main control configuration class.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-05-28
 *   $Author:  $ R.Wu
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef SYSTEMTRACKING_MAINCONTROL_H
#define SYSTEMTRACKING_MAINCONTROL_H

#include <QWidget>

#include "Core/Include/ServiceDefines.h"
#include "Core/Include/ServiceGUIConnector.h"

namespace SystemTracking {

namespace Ui {
class CMainControl;
}

class CMainControl : public QWidget
{
    Q_OBJECT
    
public:
    explicit CMainControl(Core::CServiceGUIConnector &DataConnector,
                          QWidget *parent = 0);

    ~CMainControl(void);

Q_SIGNALS:
    void ModuleListChanged(void);

    /****************************************************************************/
    /**
       * \brief Signal emitted to perform manufacturing tests
       * \iparam Test = Test name
       */
    /****************************************************************************/
    void PerformManufacturingTest(Service::ModuleTestCaseID Test, Service::ModuleTestCaseID AbortId=Service::TEST_CASE_ID_UNUSED);

public Q_SLOTS:
    void UpdateSubModule(ServiceDataManager::CSubModule &SubModule);

    void ModifyASB3(void);

    void ModifyASB5(void);

    void ModifyASB15(void);

    void ModifyTouchScreen(void);

    void ModifyVentFan(void);

    void OnFinalizeConfiguration(void);

    void CurrentTabChanged(int Index);

    void ConfirmModuleConfiguration();

private Q_SLOTS:
    void AutoDetect(ServiceDataManager::CSubModule &SubModule);

private:
    void ModifySubModule(const QString &ModuleName,
                         const QString &SubModuleName);

    void ConfirmModuleConfiguration(QString& Text);

    void ResetMessageBox();

    Ui::CMainControl *mp_Ui;
    MainMenu::CMessageDlg *mp_MessageDlg;                                   //!< Information dialog
    ServiceDataManager::CModuleDataList *mp_ModuleList;
    Core::CServiceGUIConnector *mp_DateConnector;
};


} // namespace SystemTracking
#endif // SYSTEMTRACKING_MAINCONTROL_H

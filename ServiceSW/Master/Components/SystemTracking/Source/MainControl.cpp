/****************************************************************************/
/*! \file MainControl.cpp
 *
 *  \brief Implementation of Main control configuration class.
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

#include "SystemTracking/Include/MainControl.h"

#include <QDebug>

#include "SystemTracking/Include/DlgModifySubModule.h"
#include "Main/Include/HimalayaServiceEventCodes.h"
#include "Global/Include/Utils.h"

#include "ui_MainControl.h"

#include "ServiceDataManager/Include/TestCaseGuide.h"
#include "ServiceDataManager/Include/TestCase.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"

namespace SystemTracking {

const QString MODULE_MAINCONTROL("Main Control");
const QString SUBMODULE_ASB3("ASB3");
const QString SUBMODULE_ASB5("ASB5");
const QString SUBMODULE_ASB15("ASB15");
const QString SUBMODULE_TOUCHSCREEN("Touch Screen");
const QString SUBMODULE_VENTFAN("Ventilation Fan");

CMainControl::CMainControl(Core::CServiceGUIConnector &DataConnector,
                           QWidget *parent)
    : QWidget(parent)
    , mp_DateConnector(&DataConnector)
    , mp_ModuleList(NULL)
    , mp_Ui(new Ui::CMainControl)
{
    mp_Ui->setupUi(this);

    mp_Ui->finalizeConfigBtn->setEnabled(false);

    mp_MessageDlg = new MainMenu::CMessageDlg(this);
    mp_MessageDlg->SetTitle(QApplication::translate("SystemTracking::CMainControl",
                                                    "Finalize Configuration", 0, QApplication::UnicodeUTF8));
    mp_MessageDlg->setModal(true);

    (void)connect(mp_Ui->modifyASB3,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyASB3()) );

    (void)connect(mp_Ui->modifyASB5,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyASB5()) );

    (void)connect(mp_Ui->modifyASB15,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyASB15()) );

    (void)connect(mp_Ui->modifyTouchScreen,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyTouchScreen()) );

    (void)connect(mp_Ui->modifyVentFan,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyVentFan()) );

    (void)connect(mp_Ui->finalizeConfigBtn,
                  SIGNAL(clicked()),
                  this,
                  SLOT(OnFinalizeConfiguration()) );
}

CMainControl::~CMainControl()
{
    try
    {
        delete mp_Ui;
        delete mp_MessageDlg;
    }
    catch (...) { }
}

void CMainControl::UpdateSubModule(ServiceDataManager::CSubModule &SubModule)
{
    if (!mp_ModuleList) {
        mp_ModuleList = new ServiceDataManager::CModuleDataList;
        ServiceDataManager::CModuleDataList* ModuleList = mp_DateConnector->GetModuleListContainer();
        if (!ModuleList) {
            qDebug() << "CMainControl::UpdateSubModule(): Invalid module list!";
            return;
        }

        *mp_ModuleList = *ModuleList;
    }

    ServiceDataManager::CModule *pModule = mp_ModuleList->GetModule(MODULE_MAINCONTROL);
    if (0 == pModule)
    {
        qDebug() << "CMainControl::UpdateSubModule(): Invalid module : "
                 << MODULE_MAINCONTROL;
        return;
    }

    pModule->UpdateSubModule(&SubModule);

    mp_Ui->finalizeConfigBtn->setEnabled(true);
}

void CMainControl::ModifyASB3(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_MAIN_CONTROL_SUBMODULE,
                                               Global::tTranslatableStringList()<<"ASB3");
    qDebug() << "CMainControl::ModifyASB3 !";

    this->ModifySubModule(MODULE_MAINCONTROL, SUBMODULE_ASB3);
}

void CMainControl::ModifyASB5(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_MAIN_CONTROL_SUBMODULE,
                                               Global::tTranslatableStringList()<<"ASB5");
    qDebug() << "CMainControl::ModifyASB5 !";

    this->ModifySubModule(MODULE_MAINCONTROL, SUBMODULE_ASB5);
}

void CMainControl::ModifyASB15(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_MAIN_CONTROL_SUBMODULE,
                                               Global::tTranslatableStringList()<<"ASB15");
    qDebug() << "CMainControl::ModifyASB15 !";

    this->ModifySubModule(MODULE_MAINCONTROL, SUBMODULE_ASB15);
}

void CMainControl::ModifyTouchScreen(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_MAIN_CONTROL_SUBMODULE,
                                               Global::tTranslatableStringList()<<"Touch Screen");
    qDebug() << "CMainControl::ModifyTouchScreen !";

    this->ModifySubModule(MODULE_MAINCONTROL, SUBMODULE_TOUCHSCREEN);
}

void CMainControl::ModifyVentFan(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_MAIN_CONTROL_SUBMODULE,
                                               Global::tTranslatableStringList()<<"Ventilation Fan");

    this->ModifySubModule(MODULE_MAINCONTROL, SUBMODULE_VENTFAN);
    qDebug() << "CMainControl::ModifyVentFan !";
}

void CMainControl::OnFinalizeConfiguration(void)
{
    //save config to file
    QString Text = QApplication::translate("SystemTracking::CMainControl",
                                           "Do you want to finalize the configuration for the Main Control?",
                                                       0, QApplication::UnicodeUTF8);
    ConfirmModuleConfiguration(Text);
}

void CMainControl::CurrentTabChanged(int Index)
{
    if (Index != 0) {
        ConfirmModuleConfiguration();
    }
}

void CMainControl::ConfirmModuleConfiguration()
{
    QString Text = QApplication::translate("SystemTracking::CMainControl",
                                           "Main Control Module has been modified. Do you want to finalize the configuration?",
                                           0, QApplication::UnicodeUTF8);

    if (mp_Ui->finalizeConfigBtn->isEnabled()) {
        ConfirmModuleConfiguration(Text);
    }
}

void CMainControl::ConfirmModuleConfiguration(QString& Text)
{
    mp_MessageDlg->SetButtonText(1, QApplication::translate("SystemTracking::CMainControl",
                                                            "Ok", 0, QApplication::UnicodeUTF8));
    mp_MessageDlg->SetButtonText(3,QApplication::translate("SystemTracking::CMainControl",
                                                           "Cancel", 0, QApplication::UnicodeUTF8));
    mp_MessageDlg->HideCenterButton();

    mp_MessageDlg->SetText(Text);
    mp_MessageDlg->SetIcon(QMessageBox::Warning);
    mp_MessageDlg->show();

    int Result = mp_MessageDlg->exec();

    ResetMessageBox();
    if (Result)
    {
        if(mp_DateConnector->UpdateInstrumentHistory(*mp_ModuleList))
        {
            emit ModuleListChanged();
            mp_MessageDlg->SetButtonText(1, QApplication::translate("SystemTracking::CMainControl",
                                                                    "Ok", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->HideButtons();
            mp_MessageDlg->SetText(QApplication::translate("SystemTracking::CMainControl",
                                          "Configuration file updated successfully.", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->SetIcon(QMessageBox::Information);
            mp_MessageDlg->show();
        }
        else
        {
            mp_MessageDlg->SetButtonText(1, QApplication::translate("SystemTracking::CMainControl",
                                                                    "Ok", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->HideButtons();
            mp_MessageDlg->SetText(QApplication::translate("SystemTracking::CMainControl",
                                          "Configuration file updation failed.", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->SetIcon(QMessageBox::Information);
            mp_MessageDlg->show();
        }
    }
    else
    {
        //Global::EventObject::Instance().RaiseEvent(EVENT_GUI_ADDMODIFY_SAVE_AND_OVERWRITE_CONFIGURATION_FAILURE);
        mp_MessageDlg->SetButtonText(1, QApplication::translate("SystemTracking::CMainControl",
                                                                "Ok", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetText(QApplication::translate("SystemTracking::CMainControl",
                                             "Finalize Configuration Cancelled.", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        mp_MessageDlg->show();
    }
    mp_Ui->finalizeConfigBtn->setEnabled(false);
}

void CMainControl::AutoDetect(ServiceDataManager::CSubModule &SubModule)
{
    qDebug() << "CMainControl::AutoDetect : " << SubModule.GetSubModuleName();

    Service::ModuleTestCaseID Id = Service::FIRMWARE_GET_SLAVE_INFO;
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    QString SubModuleName = SubModule.GetSubModuleName();
    QString SlaveType = SubModuleName.remove("ASB");
    p_TestCase->SetParameter("SlaveType", SlaveType);

    emit PerformManufacturingTest(Id);
}

void CMainControl::ModifySubModule(const QString &ModuleName,
                                   const QString &SubModuleName)
{
    if (!mp_ModuleList) {
        mp_ModuleList = new ServiceDataManager::CModuleDataList;
        ServiceDataManager::CModuleDataList* ModuleList = mp_DateConnector->GetModuleListContainer();
        if (!ModuleList) {
            qDebug() << "CMainControl::UpdateSubModule(): Invalid module list!";
            return;
        }

        *mp_ModuleList = *ModuleList;
    }

    ServiceDataManager::CModule *pModule = mp_ModuleList->GetModule(ModuleName);
    if (0 == pModule)
    {
        qDebug() << "CMainControl::ModifySubModule(): Invalid module : "
                 << ModuleName;
        return;
    }

    ServiceDataManager::CSubModule *pSubModule =
            pModule->GetSubModuleInfo(SubModuleName);
    if (0 == pSubModule)
    {
        qDebug() << "CMainControl::ModifySubModule(): Invalid sub module : "
                 << SubModuleName;
        return;
    }

    ServiceDataManager::CSubModule SubModule = *pSubModule;

    CDlgModifySubModule *dlg = new CDlgModifySubModule(SubModule, SubModuleName.contains("ASB"));

    dlg->UpdateGUI();

    (void)connect(dlg,
                  SIGNAL(AutoDetect(ServiceDataManager::CSubModule&)),
                  this,
                  SLOT(AutoDetect(ServiceDataManager::CSubModule&)) );

    (void)connect(dlg,
                  SIGNAL(UpdateSubModule(ServiceDataManager::CSubModule&)),
                  this,
                  SLOT(UpdateSubModule(ServiceDataManager::CSubModule&)) );

    dlg->exec();

    delete dlg;
}

void CMainControl::ResetMessageBox()
{
    if (mp_MessageDlg) {
        delete mp_MessageDlg;
        mp_MessageDlg = new MainMenu::CMessageDlg(this);
    }
}

} // namespace SystemTracking

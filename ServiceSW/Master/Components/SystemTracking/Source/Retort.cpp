/****************************************************************************/
/*! \file Retort.cpp
 *
 *  \brief Implementation of Retort configuration class.
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

#include "SystemTracking/Include/Retort.h"

#include <QDebug>

#include "SystemTracking/Include/DlgModifyModule.h"
#include "SystemTracking/Include/DlgModifySubModule.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "ui_RetortConfiguration.h"

#include "ServiceDataManager/Include/TestCaseGuide.h"
#include "ServiceDataManager/Include/TestCase.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"

namespace SystemTracking {

const QString MODULE_RETORT("Retort");                //!< Retort name
const QString SUBMODULE_HEATER("Heater");             //!< name of heater submodule
const QString SUBMODULE_LIDLOCK("Lid Lock");          //!< name of lid lock submodule
const QString SUBMODULE_LEVELSENSOR("Level Sensor");  //!< name of level sensor submodule

CRetort::CRetort(Core::CServiceGUIConnector &DataConnector,
                 QWidget *p_Parent)
    : QWidget(p_Parent)
    , mp_DateConnector(&DataConnector)
    , mp_Ui(new Ui::CRetortConfiguration)
    , mp_ModuleList(NULL)
{
    mp_Ui->setupUi(this);

    mp_Ui->finalizeConfigBtn->setEnabled(false);

    mp_MessageDlg = new MainMenu::CMessageDlg(this);
    mp_MessageDlg->SetTitle(QApplication::translate("SystemTracking::CRetort",
                                                    "Finalize Configuration", 0, QApplication::UnicodeUTF8));
    mp_MessageDlg->setModal(true);

    (void)connect(mp_Ui->modifyRetort,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyRetort()) );

    (void)connect(mp_Ui->modifyHeater,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyHeater()) );

    (void)connect(mp_Ui->modifyLidLock,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyLidLock()) );

    (void)connect(mp_Ui->modifyLevelSensor,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyLevelSensor()) );

    (void)connect(mp_Ui->finalizeConfigBtn,
                  SIGNAL(clicked()),
                  this,
                  SLOT(OnFinalizeConfiguration()));
}

CRetort::~CRetort()
{
    try
    {
        delete mp_Ui;
        delete mp_MessageDlg;
    }
    catch (...) { }
}

void CRetort::UpdateModule(ServiceDataManager::CModule &Module)
{
    qDebug() << "CRetort::UpdateModule !"
             << Module.GetModuleName();

    if (!mp_ModuleList) {
        mp_ModuleList = new ServiceDataManager::CModuleDataList;
        ServiceDataManager::CModuleDataList* ModuleList = mp_DateConnector->GetModuleListContainer();
        if (!ModuleList) {
            qDebug() << "CRetort::UpdateModule(): Invalid module list!";
            return;
        }

        *mp_ModuleList = *ModuleList;
    }

    m_SubModuleNames<<SUBMODULE_HEATER<<SUBMODULE_LEVELSENSOR;

    ServiceDataManager::CSubModule* p_SubModuleHeater = Module.GetSubModuleInfo(SUBMODULE_HEATER);
    if (p_SubModuleHeater) {
        p_SubModuleHeater->UpdateParameterInfo("OperationTime", "0");
        p_SubModuleHeater->UpdateParameterInfo("EndTestDate", "N/A");
        p_SubModuleHeater->UpdateParameterInfo("DateOfExchange", Module.GetDateOfProduction());
    }

    ServiceDataManager::CSubModule* p_SubModuleLSensor = Module.GetSubModuleInfo(SUBMODULE_LEVELSENSOR);
    if (p_SubModuleLSensor) {
        p_SubModuleLSensor->UpdateParameterInfo("OperationCycles", "0");
        p_SubModuleLSensor->UpdateParameterInfo("EndTestDate", "N/A");
        p_SubModuleLSensor->UpdateParameterInfo("DateOfExchange", Module.GetDateOfProduction());
    }

    (void)mp_ModuleList->UpdateModule(&Module);

    emit ModuleListChanged();
    mp_Ui->finalizeConfigBtn->setEnabled(true);
}

void CRetort::UpdateSubModule(ServiceDataManager::CSubModule &SubModule)
{
    qDebug() << "CRetort::UpdateSubModule !"
             << SubModule.GetSubModuleName();

    if (!mp_ModuleList) {
        mp_ModuleList = new ServiceDataManager::CModuleDataList;
        ServiceDataManager::CModuleDataList* ModuleList = mp_DateConnector->GetModuleListContainer();
        if (!ModuleList) {
            qDebug() << "CRetort::UpdateSubModule(): Invalid module list!";
            return;
        }

        *mp_ModuleList = *ModuleList;
    }

    ServiceDataManager::CModule *pModule = mp_ModuleList->GetModule(MODULE_RETORT);
    if (0 == pModule)
    {
        qDebug() << "CRetort::UpdateSubModule(): Invalid module : "
                 << MODULE_RETORT;
        return;
    }

    (void)pModule->UpdateSubModule(&SubModule);
    m_SubModuleNames<<SubModule.GetSubModuleName();

    mp_Ui->finalizeConfigBtn->setEnabled(true);
}

void CRetort::ModifyRetort(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_RETORT_MODULE);
    qDebug() << "CRetort::ModifyRetort !";

    if (!mp_ModuleList) {
        mp_ModuleList = new ServiceDataManager::CModuleDataList;
        ServiceDataManager::CModuleDataList* ModuleList = mp_DateConnector->GetModuleListContainer();
        if (!ModuleList) {
            qDebug() << "CRetort::UpdateSubModule(): Invalid module list!";
            return;
        }

        *mp_ModuleList = *ModuleList;
    }

    ServiceDataManager::CModule *pModule = mp_ModuleList->GetModule(MODULE_RETORT);
    if (0 == pModule)
    {
        qDebug() << "CRetort::ModifyRetort(): Invalid module : "
                 << MODULE_RETORT;
        return;
    }

    ServiceDataManager::CModule Module = *pModule;

    CDlgModifyModule *dlg = new CDlgModifyModule(Module);

    dlg->UpdateGUI();

    (void)connect(dlg,
                  SIGNAL(UpdateModule(ServiceDataManager::CModule&)),
                  this,
                  SLOT(UpdateModule(ServiceDataManager::CModule&)));

    (void)dlg->exec();

    delete dlg;
}

void CRetort::ModifyHeater(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_RETORT_HEATER_MODULE);
    qDebug() << "CRetort::ModifyHeater !";

    this->ModifySubModule(MODULE_RETORT, SUBMODULE_HEATER);
}

void CRetort::ModifyLidLock(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_RETORT_LIDLOCK_MODULE);
    qDebug() << "CRetort::ModifyLidLock !";

    this->ModifySubModule(MODULE_RETORT, SUBMODULE_LIDLOCK);
}

void CRetort::ModifyLevelSensor(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_RETORT_LEVELSENSOR_MODULE);
    qDebug() << "CRetort::ModifyLevelSensor !";

    this->ModifySubModule(MODULE_RETORT, SUBMODULE_LEVELSENSOR);
}

void CRetort::OnFinalizeConfiguration(void)
{
    QString Text = QApplication::translate("SystemTracking::CRetort",
                                           "Do you want to finalize the configuration for the Retort?",
                                                       0, QApplication::UnicodeUTF8);
    ConfirmModuleConfiguration(Text);
}

void CRetort::CurrentTabChanged(int Index)
{
    if (Index != 0) {
        ConfirmModuleConfiguration();
    }
}

void CRetort::ConfirmModuleConfiguration()
{
    QString Text = QApplication::translate("SystemTracking::CRetort",
                                           "Retort Module has been modified. Do you want to finalize the configuration?",
                                           0, QApplication::UnicodeUTF8);

    if (mp_Ui->finalizeConfigBtn->isEnabled()) {
        ConfirmModuleConfiguration(Text);
    }
}

void CRetort::ConfirmModuleConfiguration(QString& Text)
{
    mp_MessageDlg->SetButtonText(1, QApplication::translate("SystemTracking::CRetort",
                                                            "Ok", 0, QApplication::UnicodeUTF8));
    mp_MessageDlg->SetButtonText(3,QApplication::translate("SystemTracking::CRetort",
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
            ResetSubModuleLifeCycle();
            mp_MessageDlg->SetButtonText(1, QApplication::translate("SystemTracking::CRetort",
                                                                    "Ok", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->HideButtons();
            mp_MessageDlg->SetText(QApplication::translate("SystemTracking::CRetort",
                                          "Configuration file updated successfully.", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->SetIcon(QMessageBox::Information);
            mp_MessageDlg->show();
        }
        else
        {
            mp_MessageDlg->SetButtonText(1, QApplication::translate("SystemTracking::CRetort",
                                                                    "Ok", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->HideButtons();
            mp_MessageDlg->SetText(QApplication::translate("SystemTracking::CRetort",
                                          "Configuration file updation failed.", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->SetIcon(QMessageBox::Information);
            mp_MessageDlg->show();
        }
    }
    else
    {
        //Global::EventObject::Instance().RaiseEvent(EVENT_GUI_ADDMODIFY_SAVE_AND_OVERWRITE_CONFIGURATION_FAILURE);
        mp_MessageDlg->SetButtonText(1, QApplication::translate("SystemTracking::CRetort",
                                                                "Ok", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetText(QApplication::translate("SystemTracking::CRetort",
                                             "Finalize Configuration Cancelled.", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        mp_MessageDlg->show();
    }
    mp_Ui->finalizeConfigBtn->setEnabled(false);
}

void CRetort::ModifySubModule(const QString &ModuleName,
                              const QString &SubModuleName)
{
    if (!mp_ModuleList) {
        mp_ModuleList = new ServiceDataManager::CModuleDataList;
        ServiceDataManager::CModuleDataList* ModuleList = mp_DateConnector->GetModuleListContainer();
        if (!ModuleList) {
            qDebug() << "CRetort::UpdateSubModule(): Invalid module list!";
            return;
        }

        *mp_ModuleList = *ModuleList;
    }

    ServiceDataManager::CModule *pModule = mp_ModuleList->GetModule(ModuleName);
    if (0 == pModule)
    {
        qDebug() << "CRetort::ModifySubModule(): Invalid module : "
                 << ModuleName;
        return;
    }

    ServiceDataManager::CSubModule *pSubModule =
            pModule->GetSubModuleInfo(SubModuleName);
    if (0 == pSubModule)
    {
        qDebug() << "CRetort::ModifySubModule(): Invalid sub module : "
                 << SubModuleName;
        return;
    }

    ServiceDataManager::CSubModule SubModule = *pSubModule;

    CDlgModifySubModule *dlg = new CDlgModifySubModule(SubModule);

    dlg->UpdateGUI();

    (void)connect(dlg,
                  SIGNAL(UpdateSubModule(ServiceDataManager::CSubModule&)),
                  this,
                  SLOT(UpdateSubModule(ServiceDataManager::CSubModule&)));

    (void)dlg->exec();

    delete dlg;
}

void CRetort::ResetMessageBox()
{
    if (mp_MessageDlg) {
        delete mp_MessageDlg;
        mp_MessageDlg = new MainMenu::CMessageDlg(this);
    }
}

void CRetort::ResetSubModuleLifeCycle()
{
    qDebug() << "CRetort::ResetSubModuleLifeCycle";

    Service::ModuleTestCaseID Id = Service::RESET_OPERATION_TIME;
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    p_TestCase->SetParameter("Module", MODULE_RETORT);
    (void)m_SubModuleNames.removeDuplicates();

    for (int i = 0; i < m_SubModuleNames.count(); ++i) {
        p_TestCase->SetParameter("SubModule", m_SubModuleNames.at(i));
        emit PerformManufacturingTest(Id);
    }
}

} // namespace SystemTracking

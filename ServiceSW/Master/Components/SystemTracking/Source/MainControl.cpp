/****************************************************************************/
/*! \file SystemTracking/Source/MainControl.cpp
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

#include "ui_MainControlConfiguration.h"

#include "ServiceDataManager/Include/TestCaseGuide.h"
#include "ServiceDataManager/Include/TestCase.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"

namespace SystemTracking {

const QString MODULE_MAINCONTROL("Main Control");    //!< Main Control name
const QString SUBMODULE_EBOX("E Box");               //!< name of E box submodule
const QString SUBMODULE_ASB3("ASB3");                //!< name of ASB3 submodule
const QString SUBMODULE_ASB5("ASB5");                //!< name of ASB5 submodule
const QString SUBMODULE_ASB15("ASB15");              //!< name of ASB15 submodule
const QString SUBMODULE_TOUCHSCREEN("Touch Screen"); //!< name of touch screen submodule
const QString SUBMODULE_VENTFAN("Ventilation Fan");  //!< name of ventilation fan submodule

CMainControl::CMainControl(Core::CServiceGUIConnector &DataConnector,
                           QWidget *p_Parent)
    : QWidget(p_Parent)
    , mp_Ui(new Ui::CMainControlConfiguration)
    , mp_ModuleList(NULL)
    , mp_DateConnector(&DataConnector)
{
    mp_Ui->setupUi(this);

    mp_Ui->finalizeConfigBtn->setEnabled(false);

    mp_MessageDlg = new MainMenu::CMessageDlg(this);
    mp_MessageDlg->SetTitle(QApplication::translate("SystemTracking::CMainControl",
                                                    "Finalize Configuration", 0, QApplication::UnicodeUTF8));
    mp_MessageDlg->setModal(true);

    (void)connect(mp_Ui->modifyEBox,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyEBox()) );

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

ServiceDataManager::CModule* CMainControl::GetModule()
{
    if (!mp_ModuleList) {
        mp_ModuleList = new ServiceDataManager::CModuleDataList;
        ServiceDataManager::CModuleDataList* ModuleList = mp_DateConnector->GetModuleListContainer();
        if (!ModuleList) {
            qDebug() << "CMainControl::UpdateSubModule(): Invalid module list!";
            return NULL;
        }

        *mp_ModuleList = *ModuleList;
    }
    return mp_ModuleList->GetModule(MODULE_MAINCONTROL);
}

void CMainControl::DisableASB3()
{
    mp_Ui->modifyASB3->hide();
}

void CMainControl::UpdateSubModule(ServiceDataManager::CSubModule &SubModule)
{
    ServiceDataManager::CModule *pModule = GetModule();
    if (0 == pModule)
    {
        qDebug() << "CMainControl::UpdateSubModule(): Invalid module : "
                 << MODULE_MAINCONTROL;
        return;
    }

    (void)pModule->UpdateSubModule(&SubModule);

    m_SubModuleNames<<SubModule.GetSubModuleName();

    mp_Ui->finalizeConfigBtn->setEnabled(true);
}

void CMainControl::ModifyEBox()
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_MAIN_CONTROL_SUBMODULE,
                                               Global::tTranslatableStringList()<<"EBox");
    qDebug() << "CMainControl::ModifyEBox !";

    this->ModifySubModule(MODULE_MAINCONTROL, SUBMODULE_EBOX);
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
    (void)m_SubModuleNames.removeDuplicates();
    QString Text = QApplication::translate("SystemTracking::CMainControl",
                                           "Do you want to overwrite the configuration of the "
                                           "following module or submodules?", 0, QApplication::UnicodeUTF8);

    for (int i = 0; i < m_SubModuleNames.count(); ++i) {
        Text.append("<br>");
        Text.append(m_SubModuleNames.at(i));
    }

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
    (void)m_SubModuleNames.removeDuplicates();
    QString Text = QApplication::translate("SystemTracking::CMainControl",
                                           "Main Control Module has been modified. Do you want to overwrite the configuration "
                                           "of the following module or submodules?", 0, QApplication::UnicodeUTF8);

    for (int i = 0; i < m_SubModuleNames.count(); ++i) {
        Text.append("<br>");
        Text.append(m_SubModuleNames.at(i));
    }
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

    int Result = mp_MessageDlg->exec();

    ResetMessageBox();
    if (Result)
    {
        if(mp_DateConnector->UpdateInstrumentHistory(*mp_ModuleList))
        {
            emit ModuleListChanged();
            ResetSubModuleLifeCycle();

            if (m_SubModuleNames.contains("ASB3") || m_SubModuleNames.contains("ASB5") || m_SubModuleNames.contains("ASB15")) {
                emit UpdateSlaveVersion();
            }
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
        mp_MessageDlg->SetIcon(QMessageBox::Information);
        mp_MessageDlg->show();
    }
    m_SubModuleNames.clear();
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
    p_TestCase->SetParameter("TestType", "AutoDetect");

    emit PerformManufacturingTest(Id);
}

void CMainControl::ModifySubModule(const QString &ModuleName,
                                   const QString &SubModuleName)
{
    ServiceDataManager::CModule *pModule = GetModule();
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

    CDlgModifySubModule *dlg = new CDlgModifySubModule(*pSubModule, SubModuleName.contains("ASB"));

    dlg->UpdateGUI();

    (void)connect(dlg,
                  SIGNAL(AutoDetect(ServiceDataManager::CSubModule&)),
                  this,
                  SLOT(AutoDetect(ServiceDataManager::CSubModule&)));

    (void)connect(dlg,
                  SIGNAL(UpdateSubModule(ServiceDataManager::CSubModule&)),
                  this,
                  SLOT(UpdateSubModule(ServiceDataManager::CSubModule&)));

    (void)connect(this, SIGNAL(UpdateSerialNumber()),
                  dlg,
                  SLOT(UpdateSNOnGui()));

    (void)dlg->exec();

    delete dlg;
}

void CMainControl::ResetMessageBox()
{
    if (mp_MessageDlg) {
        delete mp_MessageDlg;
        mp_MessageDlg = new MainMenu::CMessageDlg(this);
        mp_MessageDlg->SetTitle(QApplication::translate("SystemTracking::CMainControl",
                                                        "Finalize Configuration", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->setModal(true);
    }
}

void CMainControl::ResetSubModuleLifeCycle()
{
    qDebug() << "CMainControl::ResetSubModuleLifeCycle";

    Service::ModuleTestCaseID Id = Service::RESET_OPERATION_TIME;
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    p_TestCase->SetParameter("Module", MODULE_MAINCONTROL);

    for (int i = 0; i < m_SubModuleNames.count(); ++i) {
        QString SubModuleName = m_SubModuleNames.at(i);
        if (SubModuleName.startsWith("ASB")) {
            SaveLifeCycle2Device(SubModuleName);
            //Core::CServiceUtils::delay(1000);
        }
        p_TestCase->SetParameter("SubModule", SubModuleName);
        emit PerformManufacturingTest(Id);
    }
}

void CMainControl::SaveLifeCycle2Device(QString& SlaveName)
{
    DeviceControl::DeviceLifeCycleRecord* p_DeviceRecord = new DeviceControl::DeviceLifeCycleRecord;
    if (p_DeviceRecord == NULL) {
        qDebug()<<"CMainControl::SaveLifeCycle2Device invalid device record.";
        return;
    }
    p_DeviceRecord->ReadRecord();

    ServiceDataManager::CModuleDataList* ModuleList = mp_DateConnector->GetModuleListContainer();
    if (!ModuleList) {
        qDebug() << "CMainControl::SaveLifeCycle2Device(): Invalid module list!";
        return;
    }

    if (SlaveName == "ASB3") {
        ServiceDataManager::CModule* RVModule = ModuleList->GetModule("Rotary Valve");
        DeviceControl::ModuleLifeCycleRecord* RVModuleRecord = p_DeviceRecord->m_ModuleLifeCycleMap.value("RotaryValveDevice");
        if (RVModule) {
            SetDeviceModuleLifeCycle(RVModuleRecord, "motor_rv",
                                     GetOperationTime2Second(RVModule->GetSubModuleInfo("Motor")));
            SetDeviceModuleLifeCycle(RVModuleRecord, "temp_rv",
                                     GetOperationTime2Second(RVModule->GetSubModuleInfo("Heater")));
        }
    }
    else if (SlaveName == "ASB5") {
        ServiceDataManager::CModule* OvenModule = ModuleList->GetModule("Paraffin Oven");
        DeviceControl::ModuleLifeCycleRecord* OvenModuleRecord = p_DeviceRecord->m_ModuleLifeCycleMap.value("OvenDevice");
        if (OvenModule) {
            SetDeviceModuleLifeCycle(OvenModuleRecord, "temp_oven_top",
                                     GetOperationTime2Second(OvenModule->GetSubModuleInfo("Heater")));
            SetDeviceModuleLifeCycle(OvenModuleRecord, "temp_oven_bottom",
                                     GetOperationTime2Second(OvenModule->GetSubModuleInfo("Heater")));
        }

        ServiceDataManager::CModule* RetortModule = ModuleList->GetModule("Retort");
        DeviceControl::ModuleLifeCycleRecord* RetortModuleRecord = p_DeviceRecord->m_ModuleLifeCycleMap.value("RetortDevice");
        if (RetortModule) {
            SetDeviceModuleLifeCycle(RetortModuleRecord, "temp_retort_bottom",
                                     GetOperationTime2Second(RetortModule->GetSubModuleInfo("Heater")));
        }
    }
    else if (SlaveName == "ASB15") {
        ServiceDataManager::CModule* LAModule = ModuleList->GetModule("L&A System");
        DeviceControl::ModuleLifeCycleRecord* LAModuleRecord = p_DeviceRecord->m_ModuleLifeCycleMap.value("LA");
        if (LAModule) {
            SetDeviceModuleLifeCycle(LAModuleRecord, "temp_tube1",
                                     GetOperationTime2Second(LAModule->GetSubModuleInfo("Liquid Heating Tube")));
            SetDeviceModuleLifeCycle(LAModuleRecord, "temp_tube2",
                                     GetOperationTime2Second(LAModule->GetSubModuleInfo("Air Heating Tube")));
            SetDeviceModuleLifeCycle(LAModuleRecord, "AL_pressure_ctrl",
                                     GetOperationTime2Second(LAModule->GetSubModuleInfo("Pump")));
        }
    }

    p_DeviceRecord->WriteRecord();
    delete p_DeviceRecord;
}

void CMainControl::SetDeviceModuleLifeCycle(DeviceControl::ModuleLifeCycleRecord* p_ModuleRecord, QString SubModuleName, QString CycleValue)
{
    if (p_ModuleRecord == NULL) {
        qDebug()<<"CMainControl::SetDeviceModuleLifeCycle invalid module record.";
        return;
    }

    DeviceControl::PartLifeCycleRecord* p_PartRecord = p_ModuleRecord->m_PartLifeCycleMap.value(SubModuleName);
    if (p_PartRecord == NULL) {
        qDebug()<<"CMainControl::SetDeviceModuleLifeCycle invalid part record.";
        return;
    }

    if (SubModuleName == "AL_pressure_ctrl") {
        p_PartRecord->m_ParamMap["History_Pump_OperationTime"] = CycleValue;
    }
    else {
        p_PartRecord->m_ParamMap["History_OperationTime"] = CycleValue;
    }
}

QString CMainControl::GetOperationTime2Second(ServiceDataManager::CSubModule* SubModule)
{
    if (SubModule) {
        ServiceDataManager::Parameter_t* Param = SubModule->GetParameterInfo("OperationTime");
        if (Param) {
            if (Param->ParameterUnit == "hours") {
                return QString::number(Param->ParameterValue.toDouble()*60*60);
            }
            else if (Param->ParameterUnit == "minutes") {
                return QString::number(Param->ParameterValue.toDouble()*60);
            }
        }
    }
    return QString("0");
}

} // namespace SystemTracking

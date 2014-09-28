/****************************************************************************/
/*! \file Oven.cpp
 *
 *  \brief Implementation of Oven configuration class.
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

#include "SystemTracking/Include/Oven.h"

#include <QDebug>

#include "SystemTracking/Include/DlgModifyModule.h"
#include "SystemTracking/Include/DlgModifySubModule.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "ui_OvenConfiguration.h"

#include "ServiceDataManager/Include/TestCaseGuide.h"
#include "ServiceDataManager/Include/TestCase.h"
#include "ServiceDataManager/Include/TestCaseFactory.h"

namespace SystemTracking {

const QString MODULE_OVEN("Paraffin Oven");         //!< Paraffin oven name
const QString SUBMODULE_HEATER("Heater");            //!< name of heater submodule
const QString SUBMODULE_COVERSENSOR("Cover Sensor"); //!< name of cover sensor submodule

COven::COven(Core::CServiceGUIConnector &DataConnector,
             QWidget *p_Parent)
    : QWidget(p_Parent)
    , mp_DataConnector(&DataConnector)
    , mp_Ui(new Ui::COvenConfiguration)
    , mp_ModuleList(NULL)
{
    mp_Ui->setupUi(this);

    mp_Ui->finalizeConfigBtn->setEnabled(false);
    mp_Ui->modifyHeater->hide();

    mp_MessageDlg = new MainMenu::CMessageDlg(this);
    mp_MessageDlg->SetTitle(QApplication::translate("SystemTracking::COven",
                                                    "Finalize Configuration", 0, QApplication::UnicodeUTF8));
    mp_MessageDlg->setModal(true);
    (void)connect(mp_Ui->modifyOven,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyOven()) );

    (void)connect(mp_Ui->modifyHeater,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyHeater()) );


    (void)connect(mp_Ui->modifyCoverSensor,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyCoverSensor()) );

    (void)connect(mp_Ui->finalizeConfigBtn,
                  SIGNAL(clicked()),
                  this,
                  SLOT(OnFinalizeConfiguration()));
}

COven::~COven()
{
    try
    {
        delete mp_Ui;
        delete mp_MessageDlg;
    }
    catch (...) { }
}

void COven::UpdateModule(ServiceDataManager::CModule &Module)
{
    qDebug() << "COven::UpdateModule !"
             << Module.GetModuleName();

    if (0 == mp_ModuleList) {
        qDebug() << "COven::UpdateModule(): Invalid module list!";
        return;
    }

    m_SubModuleNames<<SUBMODULE_HEATER;
    ServiceDataManager::CSubModule* p_SubModuleHeater = Module.GetSubModuleInfo(SUBMODULE_HEATER);
    if (p_SubModuleHeater) {
        (void)p_SubModuleHeater->UpdateParameterInfo("OperationTime", "0");
        (void)p_SubModuleHeater->UpdateParameterInfo("EndTestDate", "DATE");
        (void)p_SubModuleHeater->UpdateParameterInfo("DateOfExchange", Module.GetDateOfProduction());
    }

    (void)mp_ModuleList->UpdateModule(&Module);

    mp_Ui->finalizeConfigBtn->setEnabled(true);
}

void COven::UpdateSubModule(ServiceDataManager::CSubModule &SubModule)
{
    qDebug() << "COven::UpdateSubModule !"
             << SubModule.GetSubModuleName();

    if (!mp_ModuleList) {
        mp_ModuleList = new ServiceDataManager::CModuleDataList;
        ServiceDataManager::CModuleDataList* ModuleList = mp_DataConnector->GetModuleListContainer();
        if (!ModuleList) {
            qDebug() << "COven::UpdateSubModule(): Invalid module list!";
            return;
        }

        *mp_ModuleList = *ModuleList;
    }

    ServiceDataManager::CModule *pModule = mp_ModuleList->GetModule(MODULE_OVEN);
    if (0 == pModule)
    {
        qDebug() << "COven::UpdateSubModule(): Invalid module : "
                 << MODULE_OVEN;
        return;
    }

    (void)pModule->UpdateSubModule(&SubModule);
    m_SubModuleNames<<SubModule.GetSubModuleName();

    mp_Ui->finalizeConfigBtn->setEnabled(true);
}

void COven::ModifyOven(void)
{
    if (!mp_ModuleList) {
        mp_ModuleList = new ServiceDataManager::CModuleDataList;
        ServiceDataManager::CModuleDataList* ModuleList = mp_DataConnector->GetModuleListContainer();
        if (!ModuleList) {
            qDebug() << "COven::ModifyOven(): Invalid module list!";
            return;
        }

        *mp_ModuleList = *ModuleList;
    }

    ServiceDataManager::CModule *pModule = mp_ModuleList->GetModule(MODULE_OVEN);
    if (0 == pModule)
    {
        qDebug() << "COven::ModifyOven(): Invalid module : "
                 << MODULE_OVEN;
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

void COven::ModifyHeater(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_OVEN_HEATER_MODULE);
    qDebug() << "COven::ModifyHeater !";

    this->ModifySubModule(MODULE_OVEN, SUBMODULE_HEATER);
}

void COven::ModifyCoverSensor(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_OVEN_COVER_SENSOR_MODULE);
    qDebug() << "COven::ModifyCoverSensor !";

    this->ModifySubModule(MODULE_OVEN, SUBMODULE_COVERSENSOR);
}

void COven::OnFinalizeConfiguration(void)
{
    QString Text = QApplication::translate("SystemTracking::COven",
                                           "Do you want to finalize the configuration for the Oven?",
                                                       0, QApplication::UnicodeUTF8);
    ConfirmModuleConfiguration(Text);
}

void COven::CurrentTabChanged(int Index)
{
    if (Index != 0) {
        ConfirmModuleConfiguration();
    }
}

void COven::ConfirmModuleConfiguration()
{
    QString Text = QApplication::translate("SystemTracking::COven",
                                           "Oven Module has been modified. Do you want to finalize the configuration?",
                                           0, QApplication::UnicodeUTF8);

    if (mp_Ui->finalizeConfigBtn->isEnabled()) {
        ConfirmModuleConfiguration(Text);
    }
}

void COven::ConfirmModuleConfiguration(QString& Text)
{
    mp_MessageDlg->SetButtonText(1, QApplication::translate("SystemTracking::COven",
                                                            "Ok", 0, QApplication::UnicodeUTF8));
    mp_MessageDlg->SetButtonText(3,QApplication::translate("SystemTracking::COven",
                                                           "Cancel", 0, QApplication::UnicodeUTF8));
    mp_MessageDlg->HideCenterButton();

    mp_MessageDlg->SetText(Text);
    mp_MessageDlg->SetIcon(QMessageBox::Warning);
    mp_MessageDlg->show();

    int Result = mp_MessageDlg->exec();

    ResetMessageBox();
    if (Result)
    {
        if(mp_DataConnector->UpdateInstrumentHistory(*mp_ModuleList))
        {
            emit ModuleListChanged();
            ResetSubModuleLifeCycle();
            mp_MessageDlg->SetButtonText(1, QApplication::translate("SystemTracking::COven",
                                                                    "Ok", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->HideButtons();
            mp_MessageDlg->SetText(QApplication::translate("SystemTracking::COven",
                                          "Configuration file updated successfully.", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->SetIcon(QMessageBox::Information);
            mp_MessageDlg->show();
        }
        else
        {
            mp_MessageDlg->SetButtonText(1, QApplication::translate("SystemTracking::COven",
                                                                    "Ok", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->HideButtons();
            mp_MessageDlg->SetText(QApplication::translate("SystemTracking::COven",
                                          "Configuration file updation failed.", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->SetIcon(QMessageBox::Information);
            mp_MessageDlg->show();
        }
    }
    else
    {
        //Global::EventObject::Instance().RaiseEvent(EVENT_GUI_ADDMODIFY_SAVE_AND_OVERWRITE_CONFIGURATION_FAILURE);
        mp_MessageDlg->SetButtonText(1, QApplication::translate("SystemTracking::COven",
                                                                "Ok", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetText(QApplication::translate("SystemTracking::COven",
                                             "Finalize Configuration Cancelled.", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        mp_MessageDlg->show();
    }
    mp_Ui->finalizeConfigBtn->setEnabled(false);
}

void COven::ModifySubModule(const QString &ModuleName,
                            const QString &SubModuleName)
{
    if (!mp_ModuleList) {
        mp_ModuleList = new ServiceDataManager::CModuleDataList;
        ServiceDataManager::CModuleDataList* ModuleList = mp_DataConnector->GetModuleListContainer();
        if (!ModuleList) {
            qDebug() << "COven::UpdateSubModule(): Invalid module list!";
            return;
        }

        *mp_ModuleList = *ModuleList;
    }

    ServiceDataManager::CModule *pModule = mp_ModuleList->GetModule(ModuleName);
    if (0 == pModule)
    {
        qDebug() << "COven::ModifySubModule(): Invalid module : "
                 << ModuleName;
        return;
    }

    ServiceDataManager::CSubModule *pSubModule =
            pModule->GetSubModuleInfo(SubModuleName);
    if (0 == pSubModule)
    {
        qDebug() << "COven::ModifySubModule(): Invalid sub module : "
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

void COven::ResetMessageBox()
{
    if (mp_MessageDlg) {
        delete mp_MessageDlg;
        mp_MessageDlg = new MainMenu::CMessageDlg(this);
    }
}

void COven::ResetSubModuleLifeCycle()
{
    qDebug() << "COven::ResetSubModuleLifeCycle";

    Service::ModuleTestCaseID Id = Service::RESET_OPERATION_TIME;
    QString TestCaseName = DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id);
    DataManager::CTestCase* p_TestCase = DataManager::CTestCaseFactory::Instance().GetTestCase(TestCaseName);

    p_TestCase->SetParameter("Module", MODULE_OVEN);
    (void)m_SubModuleNames.removeDuplicates();

    for (int i = 0; i < m_SubModuleNames.count(); ++i) {
        p_TestCase->SetParameter("SubModule", m_SubModuleNames.at(i));
        emit PerformManufacturingTest(Id);
    }
}

} // namespace SystemTracking

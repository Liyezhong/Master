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

#include "ui_Oven.h"

namespace SystemTracking {

//const QString MODULE_OVEN("Oven");
const QString MODULE_OVEN("Paraffine Oven");
const QString SUBMODULE_HEATER("Heater");
const QString SUBMODULE_COVERSENSOR("Cover Sensor");

COven::COven(Core::CServiceGUIConnector &DataConnector,
             QWidget *parent)
    : QWidget(parent)
    , mp_DataConnector(&DataConnector)
    , mp_Ui(new Ui::COven)
    , mp_ModuleList(NULL)
{
    mp_Ui->setupUi(this);

    mp_Ui->finalizeConfigBtn->setEnabled(false);

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

    ServiceDataManager::CModuleDataList *pModuleList =
            mp_DataConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "COven::UpdateModule(): Invalid module list!";
        return;
    }

    pModuleList->UpdateModule(&Module);

    emit ModuleListChanged();

    mp_Ui->finalizeConfigBtn->setEnabled(true);
}

void COven::UpdateSubModule(ServiceDataManager::CSubModule &SubModule)
{
    qDebug() << "COven::UpdateSubModule !"
             << SubModule.GetSubModuleName();

    ServiceDataManager::CModuleDataList *pModuleList =
            mp_DataConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "COven::UpdateSubModule(): Invalid module list!";
        return;
    }

   ServiceDataManager::CModule *pModule = pModuleList->GetModule(MODULE_OVEN);
    if (0 == pModule)
    {
        qDebug() << "COven::UpdateSubModule(): Invalid module : "
                 << MODULE_OVEN;
        return;
    }

    pModule->UpdateSubModule(&SubModule);

    emit ModuleListChanged();

    mp_Ui->finalizeConfigBtn->setEnabled(true);
}

void COven::ModifyOven(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_OVEN_MODULE);
    qDebug() << "COven::ModifyOven !";

    ServiceDataManager::CModuleDataList *pModuleList =
            mp_DataConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "COven::ModifyOven(): Invalid module list!";
        return;
    }

    ServiceDataManager::CModule *pModule = pModuleList->GetModule(MODULE_OVEN);
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

    dlg->exec();

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
    QString Text = QApplication::translate("SystemTracking::CMainControl",
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
    QString Text = QApplication::translate("SystemTracking::CMainControl",
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
        if(mp_ModuleList && mp_ModuleList->Write())
        {
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
        mp_DataConnector->ReloadModuleList();
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
    ServiceDataManager::CModuleDataList *pModuleList =
            mp_DataConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "COven::ModifySubModule(): Invalid module list!";
        return;
    }

    ServiceDataManager::CModule *pModule = pModuleList->GetModule(ModuleName);
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

    dlg->exec();

    delete dlg;
}

void COven::ResetMessageBox()
{
    if (mp_MessageDlg) {
        delete mp_MessageDlg;
        mp_MessageDlg = new MainMenu::CMessageDlg(this);
    }
}

} // namespace SystemTracking

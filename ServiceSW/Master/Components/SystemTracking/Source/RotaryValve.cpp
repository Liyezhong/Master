/****************************************************************************/
/*! \file RotaryValve.cpp
 *
 *  \brief Implementation of Rotary Valve configuration class.
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

#include "SystemTracking/Include/RotaryValve.h"

#include <QDebug>

#include "SystemTracking/Include/DlgModifyModule.h"
#include "SystemTracking/Include/DlgModifySubModule.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "ui_RotaryValve.h"

namespace SystemTracking {

const QString MODULE_ROTARYVALVE("Rotary Valve");
const QString SUBMODULE_HEATER("Heater");
const QString SUBMODULE_MOTOR("Motor");

CRotaryValve::CRotaryValve(Core::CServiceGUIConnector &DataConnector,
             QWidget *parent)
    : QWidget(parent)
    , mp_DateConnector(&DataConnector)
    , ui(new Ui::CRotaryValve)
{
    ui->setupUi(this);

    (void)connect(ui->modifyRotaryValve,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyRotaryValve()) );

    (void)connect(ui->modifyHeater,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyHeater()) );


    (void)connect(ui->modifyMotor,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyMotor()) );
}

CRotaryValve::~CRotaryValve()
{
    try
    {
        delete ui;
    }
    catch (...) { }
}

void CRotaryValve::UpdateModule(ServiceDataManager::CModule &Module)
{
    qDebug() << "CRotaryValve::UpdateModule !"
             << Module.GetModuleName();

    ServiceDataManager::CModuleDataList *pModuleList =
            mp_DateConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "CRotaryValve::UpdateModule(): Invalid module list!";
        return;
    }

    pModuleList->UpdateModule(&Module);

    pModuleList->Write();

    emit ModuleListChanged();
}

void CRotaryValve::UpdateSubModule(ServiceDataManager::CSubModule &SubModule)
{
    qDebug() << "CRotaryValve::UpdateSubModule !"
             << SubModule.GetSubModuleName();

    ServiceDataManager::CModuleDataList *pModuleList =
            mp_DateConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "CRotaryValve::UpdateSubModule(): Invalid module list!";
        return;
    }

    ServiceDataManager::CModule *pModule = pModuleList->GetModule(MODULE_ROTARYVALVE);
    if (0 == pModule)
    {
        qDebug() << "CRotaryValve::UpdateSubModule(): Invalid module : "
                 << MODULE_ROTARYVALVE;
        return;
    }

    pModule->UpdateSubModule(&SubModule);

    pModuleList->Write();

    emit ModuleListChanged();
}

void CRotaryValve::ModifyRotaryValve(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_ROTARYVALVE_MODULE);
    qDebug() << "CRotaryValve::ModifyRotaryValve !";

    ServiceDataManager::CModuleDataList *pModuleList =
            mp_DateConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "CRotaryValve::ModifyRotaryValve(): Invalid module list!";
        return;
    }

    ServiceDataManager::CModule *pModule = pModuleList->GetModule(MODULE_ROTARYVALVE);
    if (0 == pModule)
    {
        qDebug() << "CRotaryValve::ModifyRotaryValve(): Invalid module : "
                 << MODULE_ROTARYVALVE;
        return;
    }

    ServiceDataManager::CModule Module = *pModule;

    CDlgModifyModule *dlg = new CDlgModifyModule(Module);

    dlg->UpdateGUI();

    (void)connect(dlg,
                  SIGNAL(UpdateModule(DataManager::CModule&)),
                  this,
                  SLOT(UpdateModule(DataManager::CModule&)));

    dlg->exec();

    delete dlg;
}

void CRotaryValve::ModifyHeater(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_ROTARYVALVE_HEATER_MODULE);
    qDebug() << "CRotaryValve::ModifyHeater !";

    this->ModifySubModule(MODULE_ROTARYVALVE, SUBMODULE_HEATER);
}

void CRotaryValve::ModifyMotor(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_ROTARYVALVE_MOTOR_MODULE);
    qDebug() << "CRotaryValve::ModifyMotor !";

    this->ModifySubModule(MODULE_ROTARYVALVE, SUBMODULE_MOTOR);
}

void CRotaryValve::ModifySubModule(const QString &ModuleName,
                                   const QString &SubModuleName)
{
    ServiceDataManager::CModuleDataList *pModuleList =
            mp_DateConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "CRotaryValve::ModifySubModule(): Invalid module list!";
        return;
    }

    ServiceDataManager::CModule *pModule = pModuleList->GetModule(ModuleName);
    if (0 == pModule)
    {
        qDebug() << "CRotaryValve::ModifySubModule(): Invalid module : "
                 << ModuleName;
        return;
    }

    ServiceDataManager::CSubModule *pSubModule =
            pModule->GetSubModuleInfo(SubModuleName);
    if (0 == pSubModule)
    {
        qDebug() << "CRotaryValve::ModifySubModule(): Invalid sub module : "
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

} // namespace SystemTracking

/****************************************************************************/
/*! \file LaSystem.cpp
 *
 *  \brief Implementation of Liquid and Air system configuration class.
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

#include "SystemTracking/Include/LaSystem.h"

#include <QDebug>

#include "SystemTracking/Include/DlgModifySubModule.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

#include "ui_LaSystem.h"

namespace SystemTracking {

const QString MODULE_LASYSTEM("L&A System");
const QString SUBMODULE_PUMP("Pump");
const QString SUBMODULE_VALVE1("Valve 1");
const QString SUBMODULE_VALVE2("Valve 2");
const QString SUBMODULE_HEATINGBELT1("Liquid Heating Tube");
const QString SUBMODULE_HEATINGBELT2("Air Heating Tube");
const QString SUBMODULE_PRESSURESENSOR("Pressure Sensor");
const QString SUBMODULE_CARBONFILTER("Carbon Filter");
const QString SUBMODULE_FAN("Fan");

CLaSystem::CLaSystem(Core::CServiceGUIConnector &DataConnector,
                           QWidget *parent)
    : QWidget(parent)
    , mp_DateConnector(&DataConnector)
    , ui(new Ui::CLaSystem)
{
    ui->setupUi(this);

    (void)connect(ui->modifyPump,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyPump()) );

    (void)connect(ui->modifyValve1,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyValve1()) );

    (void)connect(ui->modifyValve2,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyValve2()) );

    (void)connect(ui->modifyHeatingBelt1,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyHeatingBelt1()) );

    (void)connect(ui->modifyHeatingBelt2,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyHeatingBelt2()) );

    (void)connect(ui->modifyPressureSensor,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyPressureSensor()) );

    (void)connect(ui->modifyCarbonFilter,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyCarbonFilter()) );

    (void)connect(ui->modifyFan,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyFan()) );
}

CLaSystem::~CLaSystem()
{
    try
    {
        delete ui;
    }
    catch (...) { }
}

void CLaSystem::UpdateSubModule(ServiceDataManager::CSubModule &SubModule)
{
    qDebug() << "CLaSystem::UpdateSubModule : "
             << SubModule.GetSubModuleName();

    ServiceDataManager::CModuleDataList *pModuleList =
            mp_DateConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "CLaSystem::UpdateSubModule(): Invalid module list!";
        return;
    }

    ServiceDataManager::CModule *pModule = pModuleList->GetModule(MODULE_LASYSTEM);
    if (0 == pModule)
    {
        qDebug() << "CLaSystem::UpdateSubModule(): Invalid module : "
                 << MODULE_LASYSTEM;
        return;
    }

    pModule->UpdateSubModule(&SubModule);

    pModuleList->Write();

    emit ModuleListChanged();
}

void CLaSystem::ModifyPump(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_LASYSTEM_PUMP_MODULE);
    qDebug() << "CLaSystem::ModifyPump !";

    this->ModifySubModule(MODULE_LASYSTEM, SUBMODULE_PUMP);
}

void CLaSystem::ModifyValve1(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_LASYSTEM_VALVE1_MODULE);
    qDebug() << "CLaSystem::ModifyValve1 !";

    this->ModifySubModule(MODULE_LASYSTEM, SUBMODULE_VALVE1);
}

void CLaSystem::ModifyValve2(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_LASYSTEM_VALVE2_MODULE);
    qDebug() << "CLaSystem::ModifyValve2 !";

    this->ModifySubModule(MODULE_LASYSTEM, SUBMODULE_VALVE2);
}

void CLaSystem::ModifyHeatingBelt1(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_LASYSTEM_HEATING_LIQUID_MODULE);
    qDebug() << "CLaSystem::ModifyHeatingBelt1 !";

    this->ModifySubModule(MODULE_LASYSTEM, SUBMODULE_HEATINGBELT1);
}

void CLaSystem::ModifyHeatingBelt2(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_LASYSTEM_HEATING_AIR_MODULE);
    qDebug() << "CLaSystem::ModifyHeatingBelt2 !";

    this->ModifySubModule(MODULE_LASYSTEM, SUBMODULE_HEATINGBELT2);
}

void CLaSystem::ModifyPressureSensor(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_LASYSTEM_PRESSURE_SENSOR_MODULE);
    qDebug() << "CLaSystem::ModifyPressureSensor !";

    this->ModifySubModule(MODULE_LASYSTEM, SUBMODULE_PRESSURESENSOR);
}

void CLaSystem::ModifyCarbonFilter(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_LASYSTEM_CARBON_FILTER_MODULE);
    qDebug() << "CLaSystem::ModifyCarbonFilter !";

    this->ModifySubModule(MODULE_LASYSTEM, SUBMODULE_CARBONFILTER);
}

void CLaSystem::ModifyFan(void)
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MODIFY_LASYSTEM_FAN_MODULE);
    qDebug() << "CLaSystem::ModifyFan !";

    this->ModifySubModule(MODULE_LASYSTEM, SUBMODULE_FAN);
}

void CLaSystem::ModifySubModule(const QString &ModuleName,
                                const QString &SubModuleName)
{
    ServiceDataManager::CModuleDataList *pModuleList =
            mp_DateConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "CLaSystem::ModifySubModule(): Invalid module list!";
        return;
    }

    ServiceDataManager::CModule *pModule = pModuleList->GetModule(ModuleName);
    if (0 == pModule)
    {
        qDebug() << "CLaSystem::ModifySubModule(): Invalid module : "
                 << ModuleName;
        return;
    }

    ServiceDataManager::CSubModule *pSubModule =
            pModule->GetSubModuleInfo(SubModuleName);
    if (0 == pSubModule)
    {
        qDebug() << "CLaSystem::ModifySubModule(): Invalid sub module : "
                 << SubModuleName;
        return;
    }

    ServiceDataManager::CSubModule SubModule = *pSubModule;

    CDlgModifySubModule *dlg = new CDlgModifySubModule(SubModule);

    dlg->UpdateGUI();

    (void)connect(dlg,
                  SIGNAL(UpdateSubModule(ServiceDataManager::CSubModule&)),
                  this,
                  SLOT(UpdateSubModule(ServiceDataManager::CSubModule&)) );

    dlg->exec();

    delete dlg;
}


} // namespace SystemTracking

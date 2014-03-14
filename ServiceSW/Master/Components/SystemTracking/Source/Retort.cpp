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

#include "ui_Retort.h"

namespace SystemTracking {

const QString MODULE_RETORT("Retort");
const QString SUBMODULE_HEATER("Heater");
const QString SUBMODULE_LIDLOCK("Lid Lock");
const QString SUBMODULE_LEVELSENSOR("Level Sensor");

CRetort::CRetort(Core::CServiceGUIConnector &DataConnector,
                 QWidget *parent)
    : QWidget(parent)
    , mp_DateConnector(&DataConnector)
    , ui(new Ui::CRetort)
{
    ui->setupUi(this);

    (void)connect(ui->modifyRetort,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyRetort()) );

    (void)connect(ui->modifyHeater,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyHeater()) );

    (void)connect(ui->modifyLidLock,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyLidLock()) );

    (void)connect(ui->modifyLevelSensor,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyLevelSensor()) );
}

CRetort::~CRetort()
{
    try
    {
        delete ui;
    }
    catch (...) { }
}

void CRetort::UpdateModule(DataManager::CModule &Module)
{
    qDebug() << "CRetort::UpdateModule !"
             << Module.GetModuleName();

    DataManager::CModuleDataList *pModuleList =
            mp_DateConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "CRetort::UpdateModule(): Invalid module list!";
        return;
    }

    pModuleList->UpdateModule(&Module);

    pModuleList->Write();

    emit ModuleListChanged();
}

void CRetort::UpdateSubModule(DataManager::CSubModule &SubModule)
{
    qDebug() << "CRetort::UpdateSubModule !"
             << SubModule.GetSubModuleName();

    DataManager::CModuleDataList *pModuleList =
            mp_DateConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "CRetort::UpdateSubModule(): Invalid module list!";
        return;
    }

    DataManager::CModule *pModule = pModuleList->GetModule(MODULE_RETORT);
    if (0 == pModule)
    {
        qDebug() << "CRetort::UpdateSubModule(): Invalid module : "
                 << MODULE_RETORT;
        return;
    }

    pModule->UpdateSubModule(&SubModule);

    pModuleList->Write();

    emit ModuleListChanged();
}

void CRetort::ModifyRetort(void)
{
    qDebug() << "CRetort::ModifyRetort !";

    DataManager::CModuleDataList *pModuleList =
            mp_DateConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "CRetort::ModifyRetort(): Invalid module list!";
        return;
    }

    DataManager::CModule *pModule = pModuleList->GetModule(MODULE_RETORT);
    if (0 == pModule)
    {
        qDebug() << "CRetort::ModifyRetort(): Invalid module : "
                 << MODULE_RETORT;
        return;
    }

    DataManager::CModule Module = *pModule;

    CDlgModifyModule *dlg = new CDlgModifyModule(Module);

    dlg->UpdateGUI();

    (void)connect(dlg,
                  SIGNAL(UpdateModule(DataManager::CModule&)),
                  this,
                  SLOT(UpdateModule(DataManager::CModule&)));

    dlg->exec();

    delete dlg;
}

void CRetort::ModifyHeater(void)
{
    qDebug() << "CRetort::ModifyHeater !";

    this->ModifySubModule(MODULE_RETORT, SUBMODULE_HEATER);
}

void CRetort::ModifyLidLock(void)
{
    qDebug() << "CRetort::ModifyLidLock !";

    this->ModifySubModule(MODULE_RETORT, SUBMODULE_LIDLOCK);
}

void CRetort::ModifyLevelSensor(void)
{
    qDebug() << "CRetort::ModifyLevelSensor !";

    this->ModifySubModule(MODULE_RETORT, SUBMODULE_LEVELSENSOR);
}

void CRetort::ModifySubModule(const QString &ModuleName,
                              const QString &SubModuleName)
{
    DataManager::CModuleDataList *pModuleList =
            mp_DateConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "CRetort::ModifySubModule(): Invalid module list!";
        return;
    }

    DataManager::CModule *pModule = pModuleList->GetModule(ModuleName);
    if (0 == pModule)
    {
        qDebug() << "CRetort::ModifySubModule(): Invalid module : "
                 << ModuleName;
        return;
    }

    DataManager::CSubModule *pSubModule =
            pModule->GetSubModuleInfo(SubModuleName);
    if (0 == pSubModule)
    {
        qDebug() << "CRetort::ModifySubModule(): Invalid sub module : "
                 << SubModuleName;
        return;
    }

    DataManager::CSubModule SubModule = *pSubModule;

    CDlgModifySubModule *dlg = new CDlgModifySubModule(SubModule);

    dlg->UpdateGUI();

    (void)connect(dlg,
                  SIGNAL(UpdateSubModule(DataManager::CSubModule&)),
                  this,
                  SLOT(UpdateSubModule(DataManager::CSubModule&)));

    dlg->exec();

    delete dlg;
}

} // namespace SystemTracking

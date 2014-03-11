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

#include "ui_Oven.h"

namespace SystemTracking {

const QString MODULE_OVEN("Oven");
const QString SUBMODULE_HEATER("Heater");
const QString SUBMODULE_COVERSENSOR("Cover Sensor");

COven::COven(Core::CServiceGUIConnector &DataConnector,
             QWidget *parent)
    : QWidget(parent)
    , mp_DateConnector(&DataConnector)
    , ui(new Ui::COven)
{
    ui->setupUi(this);

    (void)connect(ui->modifyOven,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyOven()) );

    (void)connect(ui->modifyHeater,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyHeater()) );


    (void)connect(ui->modifyCoverSensor,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyCoverSensor()) );
}

COven::~COven()
{
    try
    {
        delete ui;
    }
    catch (...) { }
}

void COven::UpdateModule(DataManager::CModule &Module)
{
    qDebug() << "COven::UpdateModule !"
             << Module.GetModuleName();

    DataManager::CModuleDataList *pModuleList =
            mp_DateConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "COven::UpdateModule(): Invalid module list!";
        return;
    }

    pModuleList->UpdateModule(&Module);

    pModuleList->Write();

    emit ModuleListChanged();
}

void COven::UpdateSubModule(DataManager::CSubModule &SubModule)
{
    qDebug() << "COven::UpdateSubModule !"
             << SubModule.GetSubModuleName();

    DataManager::CModuleDataList *pModuleList =
            mp_DateConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "COven::UpdateSubModule(): Invalid module list!";
        return;
    }

    DataManager::CModule *pModule = pModuleList->GetModule(MODULE_OVEN);
    if (0 == pModule)
    {
        qDebug() << "COven::UpdateSubModule(): Invalid module : "
                 << MODULE_OVEN;
        return;
    }

    pModule->UpdateSubModule(&SubModule);

    pModuleList->Write();

    emit ModuleListChanged();
}

void COven::ModifyOven(void)
{
    qDebug() << "COven::ModifyOven !";

    DataManager::CModuleDataList *pModuleList =
            mp_DateConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "COven::ModifyOven(): Invalid module list!";
        return;
    }

    DataManager::CModule *pModule = pModuleList->GetModule(MODULE_OVEN);
    if (0 == pModule)
    {
        qDebug() << "COven::ModifyOven(): Invalid module : "
                 << MODULE_OVEN;
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

void COven::ModifyHeater(void)
{
    qDebug() << "COven::ModifyHeater !";

    this->ModifySubModule(MODULE_OVEN, SUBMODULE_HEATER);
}

void COven::ModifyCoverSensor(void)
{
    qDebug() << "COven::ModifyCoverSensor !";

    this->ModifySubModule(MODULE_OVEN, SUBMODULE_COVERSENSOR);
}

void COven::ModifySubModule(const QString &ModuleName,
                            const QString &SubModuleName)
{
    DataManager::CModuleDataList *pModuleList =
            mp_DateConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "COven::ModifySubModule(): Invalid module list!";
        return;
    }

    DataManager::CModule *pModule = pModuleList->GetModule(ModuleName);
    if (0 == pModule)
    {
        qDebug() << "COven::ModifySubModule(): Invalid module : "
                 << ModuleName;
        return;
    }

    DataManager::CSubModule *pSubModule =
            pModule->GetSubModuleInfo(SubModuleName);
    if (0 == pSubModule)
    {
        qDebug() << "COven::ModifySubModule(): Invalid sub module : "
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

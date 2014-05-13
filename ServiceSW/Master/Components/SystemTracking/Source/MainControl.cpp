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

//#include "EventHandler/Include/CrisisEventHandler.h"

//#include "../Include/HimalayaEventCodes.h"
#include "Main/Include/HimalayaServiceEventCodes.h"
#include "Global/Include/Utils.h"

#include "ui_MainControl.h"

namespace SystemTracking {

const QString MODULE_MAINCONTROL("Main Control");
const QString SUBMODULE_ASB3("ASB3");
const QString SUBMODULE_ASB5("ASB5");
const QString SUBMODULE_ASB15("ASB15");
const QString SUBMODULE_TOUCHSCREEN("Touch Screen");

CMainControl::CMainControl(Core::CServiceGUIConnector &DataConnector,
                           QWidget *parent)
    : QWidget(parent)
    , mp_DateConnector(&DataConnector)
    , ui(new Ui::CMainControl)
{
    ui->setupUi(this);

    (void)connect(ui->modifyASB3,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyASB3()) );

    (void)connect(ui->modifyASB5,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyASB5()) );

    (void)connect(ui->modifyASB15,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyASB15()) );

    (void)connect(ui->modifyTouchScreen,
                  SIGNAL(clicked()),
                  this,
                  SLOT(ModifyTouchScreen()) );
}

CMainControl::~CMainControl()
{
    try
    {
        delete ui;
    }
    catch (...) { }
}

void CMainControl::UpdateSubModule(DataManager::CSubModule &SubModule)
{
    qDebug() << "CMainControl::UpdateSubModule : "
             << SubModule.GetSubModuleName();

    DataManager::CModuleDataList *pModuleList =
            mp_DateConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "CMainControl::UpdateSubModule(): Invalid module list!";
        return;
    }

    DataManager::CModule *pModule = pModuleList->GetModule(MODULE_MAINCONTROL);
    if (0 == pModule)
    {
        qDebug() << "CMainControl::UpdateSubModule(): Invalid module : "
                 << MODULE_MAINCONTROL;
        return;
    }

    pModule->UpdateSubModule(&SubModule);

	pModuleList->Write();

    emit ModuleListChanged();
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

void CMainControl::AutoDetect(DataManager::CSubModule &SubModule)
{
    qDebug() << "CMainControl::AutoDetect : " << SubModule.GetSubModuleName();

    /// \todo: add auto detect sub module information here.
}

void CMainControl::ModifySubModule(const QString &ModuleName,
                                   const QString &SubModuleName)
{
    DataManager::CModuleDataList *pModuleList =
            mp_DateConnector->GetModuleListContainer();
    if (0 == pModuleList)
    {
        qDebug() << "CMainControl::ModifySubModule(): Invalid module list!";
        return;
    }

    DataManager::CModule *pModule = pModuleList->GetModule(ModuleName);
    if (0 == pModule)
    {
        qDebug() << "CMainControl::ModifySubModule(): Invalid module : "
                 << ModuleName;
        return;
    }

    DataManager::CSubModule *pSubModule =
            pModule->GetSubModuleInfo(SubModuleName);
    if (0 == pSubModule)
    {
        qDebug() << "CMainControl::ModifySubModule(): Invalid sub module : "
                 << SubModuleName;
        return;
    }

    DataManager::CSubModule SubModule = *pSubModule;

    CDlgModifySubModule *dlg = new CDlgModifySubModule(SubModule, true);

    dlg->UpdateGUI();

    (void)connect(dlg,
                  SIGNAL(AutoDetect(DataManager::CSubModule&)),
                  this,
                  SLOT(AutoDetect(DataManager::CSubModule&)) );

    (void)connect(dlg,
                  SIGNAL(UpdateSubModule(DataManager::CSubModule&)),
                  this,
                  SLOT(UpdateSubModule(DataManager::CSubModule&)) );

    dlg->exec();

    delete dlg;
}


} // namespace SystemTracking

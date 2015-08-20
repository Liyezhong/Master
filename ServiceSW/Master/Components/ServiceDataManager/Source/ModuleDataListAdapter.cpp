/****************************************************************************/
/*! \file ModuleDataListAdapter.cpp
 *
 *  \brief Implementation file for class CModuleDataListAdapter.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2014-07-31
 *  $Author:    $ Dixiong Li
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include <QDebug>
#include "Global/Include/SystemPaths.h"
#include "ServiceDataManager/Include/ModuleDataListAdapter.h"

namespace ServiceDataManager
{

CModuleDataListAdapter::CModuleDataListAdapter(ServiceDataManager::CModuleDataList* ModuleList):
    mp_ModuleList(ModuleList),
    mp_InstrumentHistoryRef(NULL),
    mp_OvenRef(NULL),
    mp_RetortRef(NULL),
    mp_LaRef(NULL),
    mp_RVRef(NULL),
    mp_OtherRef(NULL)
{
    mp_InstrumentHistoryRef = new DataManager::CInstrumentHistory;
}

CModuleDataListAdapter::~CModuleDataListAdapter()
{
    try {
        delete mp_InstrumentHistoryRef;
    }
    catch (...) {

    }

}

bool CModuleDataListAdapter::Run()
{  
    if (!InitContainers()) {
        return false;
    }

    SetRVLifeCycle();
    SetOvenLifeCycle();
    SetLaSystemLifeCycle();
    SetRetortLifeCycle();
    SetMCLifeCycle();

    if (!mp_ModuleList->Write()) {
        qDebug()<<"CModuleDateListAdaper::MoudleList write failed.";
        return false;
    }

    return true;
}

bool CModuleDataListAdapter::InitContainers()
{
    QString FilenameRef = Global::SystemPaths::Instance().GetSettingsPath() + "/InstrumentHistoryRef.xml";
    if (!mp_InstrumentHistoryRef->ReadFile(FilenameRef)) {
        qDebug()<<"CModuleDateListAdapter::InitContainers failed: read ref file failed.";
        return false;
    }

    DataManager::CModuleDataList* ModuleListRef = mp_InstrumentHistoryRef->GetModuleList(0);
    if (!ModuleListRef) {
        qDebug()<<"CModuleDataListAdapter::InitContainers failed: invalid MoudlistRef.";
        return false;
    }

    mp_OvenRef   = ModuleListRef->GetModule("OvenDevice");
    mp_RetortRef = ModuleListRef->GetModule("RetortDevice");
    mp_LaRef     = ModuleListRef->GetModule("AirLiquidDevice");
    mp_RVRef     = ModuleListRef->GetModule("RotaryValveDevice");
    mp_OtherRef  = ModuleListRef->GetModule("OtherDevice");

    if (!mp_OvenRef || !mp_RetortRef || !mp_LaRef || !mp_RVRef || !mp_OtherRef) {
        qDebug()<<"CModuleDataListAdapter::InitContainers failed: get Module Ref failed.";
        return false;
    }

    return true;
}

void CModuleDataListAdapter::SetRVLifeCycle()
{
    ServiceDataManager::CModule* RVModule = mp_ModuleList->GetModule("Rotary Valve");
    if (!RVModule) {
        qDebug()<<"CModuleDataListAdapter::SetRVLifeCycle: invalid Module";
        return;
    }

    SetSubModuleLifeCycle(RVModule->GetSubModuleInfo("Heater"),
                          mp_RVRef->GetSubModuleInfo("temp_rv"), TIME);

    SetSubModuleLifeCycle(RVModule->GetSubModuleInfo("Motor"),
                          mp_RVRef->GetSubModuleInfo("motor_rv"), TIME);
}

void CModuleDataListAdapter::SetMCLifeCycle()
{
    ServiceDataManager::CModule* MCModule = mp_ModuleList->GetModule("Main Control");
    if (!MCModule) {
        qDebug()<<"CModuleDataListAdapter::SetMCLifeCycle: invalid Module";
        return;
    }

    SetSubModuleLifeCycle(MCModule->GetSubModuleInfo("ASB3"), mp_RVRef->GetSubModuleInfo("asb3_0"), TIME);
    SetSubModuleLifeCycle(MCModule->GetSubModuleInfo("ASB5"), mp_OvenRef->GetSubModuleInfo("asb5_0"), TIME);
    SetSubModuleLifeCycle(MCModule->GetSubModuleInfo("ASB15"), mp_LaRef->GetSubModuleInfo("asb15_0"), TIME);

    SetSubModuleLifeCycle(MCModule->GetSubModuleInfo("E Box"),
                          mp_OtherRef->GetSubModuleInfo("EBox"), TIME);

    SetSubModuleLifeCycle(MCModule->GetSubModuleInfo("Touch Screen"),
                          mp_OtherRef->GetSubModuleInfo("Touch_Screen"), TIME);

    SetSubModuleLifeCycle(MCModule->GetSubModuleInfo("Ventilation Fan"),
                          mp_OtherRef->GetSubModuleInfo("Ventilation_Fan"), TIME);

}

void CModuleDataListAdapter::SetOvenLifeCycle()
{
    ServiceDataManager::CModule* OvenModule = mp_ModuleList->GetModule("Paraffin Oven");
    if (!OvenModule) {
        qDebug()<<"CModuleDataListAdapter::SetOvenLifeCycle: invalid Module";
        return;
    }

    SetSubModuleLifeCycle(OvenModule->GetSubModuleInfo("Heater"),
                           mp_OvenRef->GetSubModuleInfo("temp_oven_top"), TIME);

    SetSubModuleLifeCycle(OvenModule->GetSubModuleInfo("Cover Sensor"),
                          mp_OvenRef->GetSubModuleInfo("oven_door_status"), TIME);
}

void CModuleDataListAdapter::SetLaSystemLifeCycle()
{
    ServiceDataManager::CModule* LAModule = mp_ModuleList->GetModule("L&A System");

    if (!LAModule) {
        qDebug()<<"CModuleDataListAdapter::SetLaSystemLifeCycle: invalid Module";
        return;
    }

    SetSubModuleLifeCycle(LAModule->GetSubModuleInfo("Liquid Heating Tube"),
                          mp_LaRef->GetSubModuleInfo("temp_tube1"), TIME);

    SetSubModuleLifeCycle(LAModule->GetSubModuleInfo("Air Heating Tube"),
                          mp_LaRef->GetSubModuleInfo("temp_tube2"), TIME);

    SetSubModuleLifeCycle(LAModule->GetSubModuleInfo("Pressure Sensor"),
                          mp_OtherRef->GetSubModuleInfo("Pressure_Sensor"), TIME);

    SetSubModuleLifeCycle(LAModule->GetSubModuleInfo("Valve 1"), GetLAPressureCycle("Valve1OperationCycle", CYCLE), CYCLE);
    SetSubModuleLifeCycle(LAModule->GetSubModuleInfo("Valve 2"), GetLAPressureCycle("Valve2OperationCycle", CYCLE), CYCLE);
    SetSubModuleLifeCycle(LAModule->GetSubModuleInfo("Pump"), GetLAPressureCycle("PumpOperationTime", TIME), TIME);
    SetSubModuleLifeCycle(LAModule->GetSubModuleInfo("Carbon Filter"), GetLAPressureCycle("ActiveCarbonFilterLifeTime", TIME), TIME);
    SetSubModuleLifeCycle(LAModule->GetSubModuleInfo("Exhaust Fan"), GetLAPressureCycle("ExhaustFanLifeTime", TIME), TIME);
}

void CModuleDataListAdapter::SetRetortLifeCycle()
{
    ServiceDataManager::CModule* RetortModule = mp_ModuleList->GetModule("Retort");

    if (!RetortModule) {
        qDebug()<<"CModuleDataListAdapter::SetRetortLifeCycle: invalid Module";
        return;
    }

    SetSubModuleLifeCycle(RetortModule->GetSubModuleInfo("Heater"),
                          mp_RetortRef->GetSubModuleInfo("temp_retort_bottom"), TIME);

    SetSubModuleLifeCycle(RetortModule->GetSubModuleInfo("Lid Lock"),
                          mp_RetortRef->GetSubModuleInfo("lid_status"), TIME);

    SetSubModuleLifeCycle(RetortModule->GetSubModuleInfo("Level Sensor"),
                          mp_LaRef->GetSubModuleInfo("temp_lsensor"), CYCLE);
}

void CModuleDataListAdapter::SetSubModuleLifeCycle(ServiceDataManager::CSubModule* SubModule,
                                                   DataManager::CSubModule* SubMouleRef, LifeCycleType Type)
{
    if (SubModule == NULL || SubMouleRef == NULL) {
        qDebug()<<"CModuleDateListAdapter::SetSubModuleLifeCycle: invalid SubMoudle.";
        return;
    }

    qreal LifeCycle(0);
    QString ParameterName;

    if (Type == TIME) {
        ParameterName = "OperationTime";
    }
    else {
        ParameterName = "OperationCycles";
    }

    DataManager::Parameter_t* Param = SubMouleRef->GetParameterInfo(ParameterName);

    if (Param) {      
        if (Param->ParameterUnit == "") {
            ParameterName = "OperationCycles";
        }

        LifeCycle = Param->ParameterValue.toDouble();
        if (Type == TIME && SubModule->GetSubModuleName() != "Motor") {
            if (Param->ParameterUnit == "seconds") {
                LifeCycle = LifeCycle/3600.0; //to hours
            }
            else if (Param->ParameterUnit == "minutes") {
                LifeCycle = LifeCycle/60.0; //to hours
            }
        }
    }

    QString ParamValue;
    if (ParameterName == "OperationTime") {
        ParamValue = QString().sprintf("%.1f", LifeCycle);
    }
    else {
        ParamValue = QString::number(LifeCycle);
    }

    (void)SubModule->UpdateParameterInfo(ParameterName, ParamValue);
}

void CModuleDataListAdapter::SetSubModuleLifeCycle(CSubModule *SubModule, qreal LifeCycle, LifeCycleType Type)
{
    if (SubModule == NULL) {
        qDebug()<<"CModuleDateListAdapter::SetSubModuleLifeCycle-> invalid submoudle";
        return;
    }
    if (Type == TIME) {
        (void)SubModule->UpdateParameterInfo("OperationTime", QString().sprintf("%.1f", LifeCycle));
    }
    else {
        (void)SubModule->UpdateParameterInfo("OperationCycles", QString::number(LifeCycle));
    }
}

qreal CModuleDataListAdapter::GetLAPressureCycle(QString ParamName, LifeCycleType Type)
{
    qreal Cycle(0);
    DataManager::CSubModule* PressureRef = mp_LaRef->GetSubModuleInfo("pressurectrl");
    DataManager::Parameter_t* Param = PressureRef->GetParameterInfo(ParamName);

    if (Param) {
        Cycle = Param->ParameterValue.toDouble();

        if (Type == TIME) {
            if (Param->ParameterUnit == "seconds") {
                Cycle = Cycle/(60*60);
            }
            else if (Param->ParameterUnit == "minutes") {
                Cycle = Cycle/60;
            }
        }
    }
    return Cycle;
}

}// namespace ServiceDataManager

/****************************************************************************/
/*! \file DeviceProcessor.cpp
 *
 *  \brief Implementation file for class DeviceProcessor.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-05-22
 *  $Author:    $ Brandon Shao
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include <ServiceDeviceController/Include/DeviceProcessor/DeviceProcessor.h>

#include "DeviceControl/Include/Interface/IDeviceProcessing.h"
#include <ServiceDeviceController/Include/ServiceDeviceController.h>
#include "DeviceControl/Include/SlaveModules/DigitalOutput.h"
#include "DeviceControl/Include/SlaveModules/BaseModule.h"
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmStepperMotor.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmTempControl.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmPressureControl.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperUtils.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmDigitalInput.h>
#include "Core/Include/CMessageString.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "ServiceDeviceController/Include/DeviceProcessor/ServiceTestHandler.h"
#include "Diagnostics/Include/ServiceDeviceProcess/ServiceDeviceProcess.h"

//lint -e1013
//lint -e115
//lint -e1055

namespace DeviceControl {

/****************************************************************************/
DeviceProcessor::DeviceProcessor(IDeviceProcessing &iDevProc)
    : m_rIdevProc(iDevProc)
    ,  m_IsConfigured(false)
    , mp_ManufacturingTestHandler(NULL)
    , mp_ServicecTestHandler(NULL)
{
    m_UserAbort = false;
    mp_Utils = NULL; 
}

/****************************************************************************/

void DeviceProcessor::Connect()
{

}

void DeviceProcessor::InitialDevice()
{
    OnModuleManufacturingTest(Service::SYSTEM_SELF_TEST);

    Diagnostics::ServiceDeviceProcess::Instance()->Initialize();
}

/****************************************************************************/
void DeviceProcessor::CreateWrappers()
{
    /* Create Manufacturing Test Handler*/
    if(NULL != mp_ManufacturingTestHandler)
    {
        delete mp_ManufacturingTestHandler;
        mp_ManufacturingTestHandler = NULL;
    }
    mp_ManufacturingTestHandler = new ManufacturingTestHandler(m_rIdevProc);

    /* Return Messages signals connected here */
    if (!connect(mp_ManufacturingTestHandler, SIGNAL(ReturnMessagetoMain(const QString)),
                this, SIGNAL(ReturnMessagetoMain(const QString)))) {
        qDebug() << "DeviceProcessor::CreateWrappers cannot connect 'ReturnMessagetoMain' signal";
    }
    if (!connect(mp_ManufacturingTestHandler, SIGNAL(ReturnErrorMessagetoMain(QString)),
                this, SIGNAL(ReturnErrorMessagetoMain(const QString)))) {
        qDebug() << "DeviceProcessor::CreateWrappers cannot connect 'ReturnErrorMessagetoMain' signal";
    }
    if (!connect(mp_ManufacturingTestHandler, SIGNAL(RefreshTestStatustoMain(QString,Service::ModuleTestStatus)),
                this, SIGNAL(RefreshTestStatustoMain(QString,Service::ModuleTestStatus)))) {
        qDebug() << "DeviceProcessor::CreateWrappers cannot connect 'RefreshTestStatustoMain' signal";
    }

    if (!connect(mp_ManufacturingTestHandler, SIGNAL(ReturnManufacturingTestMsg(bool)),
                this, SIGNAL(ReturnManufacturingTestMsg(bool)))) {
        qDebug() << "DeviceProcessor::CreateWrappers cannot connect 'RefreshTestStatustoMain' signal";
    }

    // create service test handler
    if (NULL != mp_ServicecTestHandler) {
        delete mp_ServicecTestHandler;
        mp_ServicecTestHandler = NULL;
    }
    mp_ServicecTestHandler = ServiceTestHandler::NewInstance(m_rIdevProc);

    // return messages signals connected here
    if (!connect(mp_ServicecTestHandler, SIGNAL(ReturnServiceRequestResult(QString,int,QStringList)),
                this, SIGNAL(ReturnServiceRequestResult(QString,int,QStringList)))) {
        qDebug() << "DeviceProcessor::CreateWrappers cannot connect 'ReturnServiceRequestResult' signal";
    }
}

/****************************************************************************/
void DeviceProcessor::Initialize()
{
    (void) CreateWrappers();
    m_IsConfigured = true;

    mp_Utils = new WrapperUtils(this);
}

/****************************************************************************/
bool DeviceProcessor::IsInitialized()
{
    return m_IsConfigured;
}


/****************************************************************************/
void DeviceProcessor::OnAbortTest(Global::tRefType Ref, quint32 id)
{
    Q_UNUSED(Ref);
    Q_UNUSED(id);

    qDebug()<<"DeviceProcessor::OnAbortTest";
    if(!IsInitialized()){
        Initialize();
    }

    if(NULL == mp_Utils)
    {
        emit ReturnErrorMessagetoMain(Service::MSG_DEVICE_NOT_INITIALIZED);
        return;
    }

    mp_Utils->AbortPause();

    m_UserAbort = true;

}

void DeviceProcessor::OnModuleManufacturingTest(Service::ModuleTestCaseID TestName, Service::ModuleTestCaseID AbortId)
{
//    qDebug()<<"DeviceProcessor::OnModuleManufacturingTest  test="<<TestName;
    if(!IsInitialized()){
        Initialize();
    }

    if (mp_ManufacturingTestHandler) {
        mp_ManufacturingTestHandler->PerformModuleManufacturingTest(TestName, AbortId);
    }

}

void DeviceProcessor::OnServiceRequest(QString ReqName, QStringList Params)
{
    if (mp_ServicecTestHandler && !mp_ServicecTestHandler->IsInitialized() )
    {
        mp_ServicecTestHandler->Initialize();
    }

    if (mp_ServicecTestHandler) {
        mp_ServicecTestHandler->HandleRequest(ReqName, Params);
    }
}

} // end namespace DeviceControl

/****************************************************************************/
/*! \file ServiceDeviceController.cpp
 *
 *  \brief ServiceDeviceController Implementation
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-02-14
 *   $Author:  $ Srivathsa HH
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
//QT Headers
#include <QMetaType>
#include <QDebug>
//Std lib headers

//Project Headers
#include <ServiceDeviceController/Include/ServiceDeviceController.h>
#include <Core/Include/Startup.h>
#include <Global/Include/Utils.h>

#include "Application/Include/Application.h"

#include "DeviceControl/Include/Interface/IDeviceProcessing.h"
#include <ServiceDeviceController/Include/Commands/CmdCalibrateDevice.h>
#include <Core/Include/CMessageString.h>

namespace DeviceControl {

/****************************************************************************/
/*!
 *  \brief    Constructor for the ServiceDeviceController
 *
 *  \return
 *
 ****************************************************************************/
ServiceDeviceController::ServiceDeviceController(Global::gSourceType TheHeartBeatSource)
    : DeviceCommandProcessor::DeviceCommandProcessorThreadController(TheHeartBeatSource, "HimalayaDeviceCommand")
     , m_ProcessSettings(false)
{
    //qRegisterMetaType<DeviceControl::DevInstanceID_t>("DeviceControl::DevInstanceID_t");
    qRegisterMetaType<Service::DeviceCalibrationCmdType>("Service::DeviceCalibrationCmdType");
}


/****************************************************************************/
/*!
 *  \brief    Destructor for the ServiceDeviceController
 *
 *  \return
 *
 ****************************************************************************/
ServiceDeviceController::~ServiceDeviceController(){
}

/****************************************************************************/
/*!
 *  \brief    API function to call before EventHandler can be used
 *
 *      This function does everything to initialize the EventHandler. It
 *      shall be called before EventHandler can be used.
 *
 ****************************************************************************/
void ServiceDeviceController::CreateAndInitializeObjects(){
    // now register commands
    DeviceCommandProcessorThreadController::CreateAndInitializeObjects();
    RegisterCommands();
    mp_DeviceProcessor = new DeviceProcessor(*m_pDeviceProcessing);
    ConnectSignalsnSlots();
    RequestForDataContainers();
}
/****************************************************************************/
/*!
 *  \brief Requests ServiceMasterThread for required DataContainers.
 *
 *
 *  \return
 */
/****************************************************************************/
void ServiceDeviceController::RequestForDataContainers(){
    DeviceCommandProcessor::CmdGetDataContainers* commandPtr(new DeviceCommandProcessor::CmdGetDataContainers());
    SendCommand(GetNewCommandRef(), Global::CommandShPtr_t(commandPtr));
}

/****************************************************************************/
/*!
 *  \brief Destroy all used objects and prepare for shutdown.
 *
 *         See detailed description in the Base Class's header
 *
 *  \return
 */
/****************************************************************************/
void ServiceDeviceController::CleanupAndDestroyObjects(){
}

/****************************************************************************/
/*!
 *  \brief Register commands handled by thread Controller
 *
 */
/****************************************************************************/
void ServiceDeviceController::ConnectSignalsnSlots()
{
    /* Connect Signals and slots w.r.t. Device Data containers*/
    if (!connect(this, SIGNAL(DataContainersInitialized()),
                 mp_DeviceProcessor, SLOT(OnDataContainersInitialized()))) {
        qDebug() << "ServiceDeviceController::ConnectSignalsnSlots cannot connect 'DataContainersInitialized' signal";
    }
    if (!connect(this, SIGNAL(SetProcessSettingsDataContainer(DataManager::CProcessSettings)),
                 this, SLOT(SetProcessSettingsFlag(DataManager::CProcessSettings )))) {
        qDebug() << "ServiceDeviceController::ConnectSignalsnSlots cannot connect 'DataContainersInitialized' signal";
    }

    /* Return Messages signals connected here */
    if (!connect(mp_DeviceProcessor, SIGNAL(ReturnMessagetoMain(const QString)),
                 this, SLOT(ReturnMessagetoMain(const QString)))) {
        qDebug() << "ServiceDeviceController::ConnectSignalsnSlots cannot connect 'ReturnMessagetoMain' signal";
    }
    if (!connect(mp_DeviceProcessor, SIGNAL(ReturnErrorMessagetoMain(const QString)),
                 this, SLOT(ReturnErrorMessagetoMain(QString)))) {
        qDebug() << "ServiceDeviceController::ConnectSignalsnSlots cannot connect 'ReturnErrorMessagetoMain' signal";
    }

    if (!connect(mp_DeviceProcessor, SIGNAL(ReturnErrorMessagetoMain(const QString)),
                 this, SLOT(ReturnErrorMessagetoMain(QString)))) {
        qDebug() << "ServiceDeviceController::ConnectSignalsnSlots cannot connect 'ReturnErrorMessagetoMain' signal";
    }

    if (!connect(this, SIGNAL(SDC_AbortTest(Global::tRefType,DeviceControl::DevInstanceID_t)),
                 mp_DeviceProcessor, SLOT(OnAbortTest(Global::tRefType,DeviceControl::DevInstanceID_t)))) {
        qDebug() << "ServiceDeviceController::ConnectSignalsnSlots cannot connect 'OnAbortTest' signal";
    }

    if (!connect(mp_DeviceProcessor, SIGNAL(ReturnCalibrationInitMessagetoMain(QString,bool)),
                 this, SLOT(ReturnCalibrationInitMessagetoMain(QString,bool)))) {
        qDebug() << "ServiceDeviceController::ConnectSignalsnSlots cannot connect 'ReturnCalibrationInitMessagetoMain' signal";
    }


    // for Manufacturing Diagnostic
    if (!connect(this, SIGNAL(ModuleManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID)),
                 mp_DeviceProcessor, SLOT(OnModuleManufacturingTest(Service::ModuleTestCaseID, Service::ModuleTestCaseID)))) {
        qDebug() << "ServiceDeviceController::ConnectSignalsnSlots cannot connect 'ModuleManufacturingTest' signal";
    }
    if (!connect(mp_DeviceProcessor, SIGNAL(RefreshTestStatustoMain(QString,Service::ModuleTestStatus)),
                 this, SLOT(RefreshTestStatustoMain(QString,Service::ModuleTestStatus)))) {
        qDebug() << "ServiceDeviceController::ConnectSignalsnSlots cannot connect 'RefreshTestStatustoMain' signal";
    }

    if (!connect(mp_DeviceProcessor, SIGNAL(ReturnManufacturingTestMsg(bool)),
                 this, SLOT(ReturnManufacturingTestMsg(bool)))) {
        qDebug() << "ServiceDeviceController::ReturnManufacturingTestMsg cannot connect 'ReturnManufacturingTestMsg' signal";
    }

    // for service request
    if (!connect(this, SIGNAL(ServiceRequest(QString,QStringList)),
                 mp_DeviceProcessor, SLOT(OnServiceRequest(QString,QStringList)))) {
        qDebug() << "ServiceDeviceController::ConnectSignalsnSlots cannot connect 'ServiceRequest' signal";
    }

    if (!connect(mp_DeviceProcessor, SIGNAL(ReturnServiceRequestResult(QString,int,QStringList)),
                 this, SLOT(OnReturnServiceRequestResult(QString,int,QStringList)))) {
        qDebug() << "ServiceDeviceController::ReturnManufacturingTestMsg cannot connect 'ReturnServiceRequestResult' signal";
    }
}

/****************************************************************************/
/*!
 *  \brief Register commands handled by thread Controller
 *
 */
/****************************************************************************/
void ServiceDeviceController::RegisterCommands(){
    RegisterAcknowledgeForProcessing<Global::AckOKNOK, ServiceDeviceController>
            (&ServiceDeviceController::OnAcknowledge, this);

    RegisterCommandForProcessing<DeviceCommandProcessor::CmdAbortTest, ServiceDeviceController>
            (&ServiceDeviceController::OnSDC_AbortTest, this);


    RegisterCommandForProcessing<DeviceCommandProcessor::CmdModuleManufacturingTest, ServiceDeviceController>
            (&ServiceDeviceController::OnCmdModuleManufacturingTest, this);

    RegisterCommandForProcessing<DeviceCommandProcessor::CmdServiceTest, ServiceDeviceController>
            (&ServiceDeviceController::OnCmdServiceRequest, this);
}

/****************************************************************************/
/*!
 *  \brief This method is called when the base class receives Go signal.
 *
 *         See detailed description in the Base Class's header
 *
 *  \return
 */
/****************************************************************************/
void ServiceDeviceController::OnGoReceived(){
    qDebug()<<"ServiceDeviceController::OnGoReceived()";
    DeviceCommandProcessorThreadController::OnGoReceived();
}

/****************************************************************************/
/*!
 *  \brief This method is called when the base class receives Stop signal.
 *
 *         See detailed description in the Base Class's header
 *
 *  \return
 */
/****************************************************************************/
void ServiceDeviceController::OnStopReceived(){

}

/****************************************************************************/
/**
 * \brief Power will fail shortly.
 *
 * Power will fail shortly.
 */
/****************************************************************************/
void ServiceDeviceController::OnPowerFail(const Global::PowerFailStages PowerFailStage){
    /// \todo implement
    Q_UNUSED(PowerFailStage);
}

/****************************************************************************/
/**
 * \brief Get UnInitialized devices.
 *
 */
/****************************************************************************/
void ServiceDeviceController::GetUnInitializedDevices(QList<quint32> &)
{
}

/****************************************************************************/
/**
 * \brief Send command
 *
 */
/****************************************************************************/
void ServiceDeviceController::SendCommand(Global::tRefType Ref, const Global::CommandShPtr_t &Cmd){
//    qDebug() << "ServiceDeviceController::SendCommand" << Ref;
    Threads::ThreadController::SendCommand(Ref, Cmd);
}

/****************************************************************************/
/**
 * \brief To get a new command Reference.
 *
 */
/****************************************************************************/
Global::tRefType ServiceDeviceController::GetNewCommandRef(){
    return Threads::ThreadController::GetNewCommandRef();
}

/****************************************************************************/
/**
 * \brief For handling acknowledgements.
 *
 */
/****************************************************************************/
void ServiceDeviceController::OnAcknowledge(Global::tRefType, const Global::AckOKNOK &){

}

/*****************************************************************************/
/**
 *  \brief     Normal operation mode start notification
 *
 *  \param     HdlInfo    = Return code, DCL_ERR_FCT_CALL_SUCCESS if successfull, otherwise error return code
 */
/****************************************************************************/
void ServiceDeviceController::ConnectDevices()
{
    qDebug()<<"---------------- ServiceDeviceController::ConnectDevices()";

  //  mp_DeviceProcessor->InitialDevice();

//    m_pXYZSet->Connect();
//    m_pDrawerSet->Connect();
//    m_pHoodSet->Connect();
//    m_pAgitationSet->Connect();
//    m_pRackTransferSet->Connect();
//    m_pOvenSet->Connect();
//    m_pHeatedCuvettesSet->Connect();

//    ConnectServiceSignals();

//    emit DeviceConfigure();
}

void ServiceDeviceController::InitDevices()
{
    qDebug()<<"ServiceDeviceController::InitDevices()";
    mp_DeviceProcessor->InitialDevice();
}

/****************************************************************************/
void ServiceDeviceController::ReturnMessagetoMain(const QString &Message)
{
    qDebug()<<"ServiceDeviceController::ReturnMessagetoMain()  --Message="<<Message;

    DeviceCommandProcessor::CmdReturnMessage* commandPtr(new DeviceCommandProcessor::CmdReturnMessage(Message));
    SendCommand(GetNewCommandRef(), Global::CommandShPtr_t(commandPtr));
}

/****************************************************************************/
void ServiceDeviceController::ReturnErrorMessagetoMain(const QString &Message)
{
    qDebug()<<"ServiceDeviceController::ReturnErrorMessagetoMain()  --Message="<<Message;

    DeviceCommandProcessor::CmdReturnMessage* commandPtr(new DeviceCommandProcessor::CmdReturnMessage(Message));
    commandPtr->m_MessageType = Service::GUIMSGTYPE_ERROR;
    SendCommand(GetNewCommandRef(), Global::CommandShPtr_t(commandPtr));
}

/****************************************************************************/
void ServiceDeviceController::ReturnCalibrationInitMessagetoMain(const QString &Message, bool OkStatus)
{

    qDebug()<<"ServiceDeviceController::ReturnCalibrationInitMessagetoMain()  --Message="<<Message;

    DeviceCommandProcessor::CmdReturnMessage* commandPtr(new DeviceCommandProcessor::CmdReturnMessage(Message));
    commandPtr->m_MessageType = Service::GUIMSGTYPE_INITCALIBRATION;
    commandPtr->m_CalibStatus = OkStatus;
    SendCommand(GetNewCommandRef(), Global::CommandShPtr_t(commandPtr));
}

/****************************************************************************/
void ServiceDeviceController::RefreshTestStatustoMain(const QString &Message, const Service::ModuleTestStatus &Status)
{

//    qDebug()<<"ServiceDeviceController::RefreshTestStatustoMain()  --Message="<<Message << " Status:"<<Status;

    DeviceCommandProcessor::CmdReturnMessage* commandPtr(new DeviceCommandProcessor::CmdReturnMessage(Message));
    commandPtr->m_MessageType = Service::GUIMSGTYPE_HEATINGSTATUS;
    commandPtr->m_Status = Status;
    SendCommand(GetNewCommandRef(), Global::CommandShPtr_t(commandPtr));
}

/****************************************************************************/
void ServiceDeviceController::ReturnManufacturingTestMsg(bool TestResult)
{
    qDebug()<<"ServiceDeviceController::ReturnManufacturingTestMsg TestResult="<<TestResult;

    DeviceCommandProcessor::CmdReturnMessage* commandPtr(new DeviceCommandProcessor::CmdReturnMessage(Service::CMessageString::MSG_MANUFACTURINGTEST_RESULT));
    commandPtr->m_ModuleTestResult = TestResult;
    commandPtr->m_MessageType = Service::GUIMSGTYPE_MANUFMSGMODE;
    SendCommand(GetNewCommandRef(), Global::CommandShPtr_t(commandPtr));
}

void ServiceDeviceController::OnReturnServiceRequestResult(QString ReqName, int ErrorCode, QStringList Results)
{
    qDebug()<<"ServiceDeviceController::OnReturnServiceRequestResult TestResult="<<ErrorCode;
    qDebug()<<"ReqName="<<ReqName;
    qDebug()<<Results;

    DeviceCommandProcessor::CmdReturnMessage* commandPtr(new DeviceCommandProcessor::CmdReturnMessage("Service Request Result"));
    commandPtr->m_ServErrorCode = ErrorCode;
    commandPtr->m_ServReqName = ReqName;
    commandPtr->m_ServResults = Results;
    commandPtr->m_MessageType = Service::GUIMSGTYPE_SERVICE_REQ_RESULT;

    SendCommand(GetNewCommandRef(), Global::CommandShPtr_t(commandPtr));
}


/****************************************************************************/
void ServiceDeviceController::OnSDC_AbortTest(Global::tRefType Ref, const DeviceCommandProcessor::CmdAbortTest &Cmd)
{
    SendAcknowledgeOK(Ref);
    //if(m_DeviceDataContainers)
    //if(true)
    //{
        emit SDC_AbortTest(Ref, Cmd.m_DevInstanceID);
    //}
    //else
    //{
        //ReturnErrorMessagetoMain(Service::MSG_DEVICEDATACONTAINERS_MISSING);
    //}
}


/****************************************************************************/
void ServiceDeviceController::OnCmdModuleManufacturingTest(Global::tRefType Ref, const DeviceCommandProcessor::CmdModuleManufacturingTest &Cmd)
{
    SendAcknowledgeOK(Ref);

    qDebug()<<"ServiceDeviceController::OnCmdModuleManufacturingTest CmdType="<<Cmd.m_CommandType;

    emit ModuleManufacturingTest(Cmd.m_CommandType, Cmd.m_AbortTestCaseId);
}

void ServiceDeviceController::OnCmdServiceRequest(Global::tRefType Ref, const DeviceCommandProcessor::CmdServiceTest &Cmd)
{
    SendAcknowledgeOK(Ref);

    qDebug()<<"ServiceDeviceController::OnCmdServiceRequest ReqName="<<Cmd.m_ReqName;

    emit ServiceRequest(Cmd.m_ReqName, Cmd.m_Params);
}

} //End Of namespace DeviceControl

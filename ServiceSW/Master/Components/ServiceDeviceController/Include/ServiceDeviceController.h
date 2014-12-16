/****************************************************************************/
/*! \file Include/ServiceDeviceController.h
 *
 *  \brief  Definition of class ServiceDeviceController
 *
 *  Version:    0.1
 *  Date:       2013-02-14
 *  Author:     Srivathsa HH
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
#ifndef SERVICEDEVICECONTROLLER_H
#define SERVICEDEVICECONTROLLER_H
//QT Headers
#include <QObject>
#include <stdio.h>

//Project Headers
#include <Global/Include/GlobalDefines.h>
#include <Threads/Include/ThreadController.h>
#include <Global/Include/Commands/AckOKNOK.h>
#include <DeviceCommandProcessor/Include/DeviceCommandProcessorThreadController.h>


#include <ServiceDeviceController/Include/Commands/CmdReturnMessage.h>
#include <ServiceDeviceController/Include/Commands/CmdGetDataContainers.h>

#include <ServiceDeviceController/Include/Commands/CmdAbortTest.h>
#include <ServiceDeviceController/Include/Commands/CmdModuleManufacturingTest.h>
#include <ServiceDeviceController/Include/Commands/CmdServiceTest.h>

#include <ServiceDeviceController/Include/DeviceProcessor/DeviceProcessor.h>

namespace DeviceControl {
/****************************************************************************/
/**
 * \brief This Class manages all device related commands and events.
 *
 */
/****************************************************************************/
class ServiceDeviceController : public DeviceCommandProcessor::DeviceCommandProcessorThreadController {
    Q_OBJECT

public:
    /**
     * \brief Constructor.
     *
     * \param[in]   TheHeartBeatSource     Source for Heart Beat.
     */
    /****************************************************************************/
    ServiceDeviceController(Global::gSourceType TheHeartBeatSource);
    /****************************************************************************/
    /**
     * \brief Destructor.
     */
    /****************************************************************************/
    virtual ~ServiceDeviceController();
    /****************************************************************************/
    /**
     * \brief Create and Initialize objects this class holds.
     */
    /****************************************************************************/
    virtual void CreateAndInitializeObjects();
    /****************************************************************************/
    /**
     * \brief Clean up and destroy objects this class holds.
     */
    /****************************************************************************/
    virtual void CleanupAndDestroyObjects();

    /****************************************************************************/
    /**
      * \brief Send command to external process.
      *
      * Send a command to the external process. Reference mapping does not
      * have to be done since the acknowledge we receive from the external
      * process corresponds with Ref.
      *
      * \param[in]   Ref                 Command reference.
      * \param[in]   Cmd                 Command to send.
      */
    /****************************************************************************/
    void SendCommand(Global::tRefType Ref, const Global::CommandShPtr_t &Cmd);    
    /****************************************************************************/
    /**
     * \brief Command of type CmdAbortTest received.
     *
     * \param[in]       Ref                 Reference of command.
     * \param[in]       Cmd                 Command.
     */
    /****************************************************************************/
    void OnSDC_AbortTest(Global::tRefType Ref, const DeviceCommandProcessor::CmdAbortTest &Cmd);

    /****************************************************************************/
    /**
     * \brief Returns a new Command Reference.
     *
     * \return tRefType    New command Reference
     */
    /****************************************************************************/
    Global::tRefType GetNewCommandRef();
    /****************************************************************************/
    /**
     * \brief To connect to all devices.
     */
    /****************************************************************************/
    virtual void ConnectDevices();
    /****************************************************************************/
    /**
     * \brief To initialize all devices.
     */
    /****************************************************************************/
    virtual void InitDevices();
    /****************************************************************************/
    /**
     * \brief To get UnInitialized devices.
     */
    /****************************************************************************/
    virtual void GetUnInitializedDevices(QList<quint32> &);
    /****************************************************************************/

    /****************************************************************************/
    /**
     * \brief Command of type CmdModuleManufacturingTest received.
     *
     * \iparam       Ref                 Reference of command.
     * \iparam       Cmd                 Command.
     */
    /****************************************************************************/
    void OnCmdModuleManufacturingTest(Global::tRefType Ref, const DeviceCommandProcessor::CmdModuleManufacturingTest &Cmd);

    /****************************************************************************/
    /**
     * \brief Command of type CmdServiceRequest received.
     *
     * \iparam       Ref                 Reference of command.
     * \iparam       Cmd                 Command.
     */
    /****************************************************************************/
    void OnCmdServiceRequest(Global::tRefType Ref, const DeviceCommandProcessor::CmdServiceTest &Cmd);

public slots:
    /**
     * \brief Returns a message to Main Thread Controller.
     *
     * \param[in]   Message    QString message to be sent.
     */
    /****************************************************************************/
    void ReturnMessagetoMain(const QString &Message);
    /****************************************************************************/
    /**
     * \brief Returns an Error message to Main Thread Controller.
     *
     * \param[in]   Message    QString message to be sent.
     */
    /****************************************************************************/
    void ReturnErrorMessagetoMain(const QString &Message);
    /****************************************************************************/
    /**
     * \brief Returns CalibrationInit Info message to Main Thread Controller.
     *
     * \iparam   Message    QString message to be sent.
     * \iparam   OkStatus   Calibration init status
     */
    /****************************************************************************/
    void ReturnCalibrationInitMessagetoMain(const QString &Message, bool OkStatus);

    /****************************************************************************/
    /**
     * \brief Refresh heating status to GUI.
     *
     * \iparam   Message    QString message to be sent.
     * \iparam   Status     Heating Status
     */
    /****************************************************************************/
    void RefreshTestStatustoMain(const QString &Message, const Service::ModuleTestStatus &Status);

    /****************************************************************************/
    /**
     * \brief Returns RFIDConsumables message to Main Thread Controller.
     *
     * \iparam   TestResult = Test result
     */
    /****************************************************************************/
    void ReturnManufacturingTestMsg(bool TestResult);

    /****************************************************************************/
    /**
     * \brief Returns RFIDConsumables message to Main Thread Controller.
     *
     * \iparam   ReqName = request name
     * \iparam   ErrorCode = error code
     * \iparam   Results = store result
     */
    /****************************************************************************/
    void OnReturnServiceRequestResult(QString ReqName, int ErrorCode, QStringList Results);

#if 0
    /****************************************************************************/
    /**
     * \brief Sets Process Settings Flag.
     *
     * \param[in]   Message    QString message to be sent.
     */
    /****************************************************************************/
    void SetProcessSettingsFlag(DataManager::CProcessSettings Container);
    /****************************************************************************/

#endif

signals:

    /****************************************************************************/
    /**
     * \brief when the data container initialized will be called
     */
    /****************************************************************************/
    void DataContainersInitialized();

    /****************************************************************************/
    /**
     * \brief SDC_AbortTest signal to abort test
     */
    /****************************************************************************/
    void SDC_AbortTest(Global::tRefType Ref, quint32 id);

    /****************************************************************************/
    /**
     * \brief SDC_HeatingTest signal to test heating
     */
    /****************************************************************************/
    void SDC_HeatingTest(Global::tRefType Ref, quint32 id,
                           quint8 HeaterIndex, quint8 CmdType);

    /****************************************************************************/
    /**
     * \brief SDC_RotaryValveTest signal to test rotary valve movement
     */
    /****************************************************************************/
    void SDC_RotaryValveTest(Global::tRefType Ref, quint32 id,
                           qint32 Position, quint8 CmdType);

    /****************************************************************************/
    /**
     * \brief SDC_LSensorDetectingTest signal to test level sensor detecting
     */
    /****************************************************************************/
    void SDC_LSensorDetectingTest(Global::tRefType Ref, quint32 id,
                           qint32 Position);

    /****************************************************************************/
    /**
     * \brief CalibrateDevice Signal
     * \iparam       CmdType         Command type
     */
    /****************************************************************************/
    void CalibrateDevice(Service::DeviceCalibrationCmdType CmdType);

    /****************************************************************************/
    /**
     * \brief ModuleManufacturingTest Signal
     * \iparam       CmdType         Module Test Name
     */
    /****************************************************************************/
    void ModuleManufacturingTest(Service::ModuleTestCaseID TestName, Service::ModuleTestCaseID AbortTestCaseId=Service::TEST_CASE_ID_UNUSED);

    /****************************************************************************/
    /**
     * \brief ModuleManufacturingTest Signal
     * \iparam       ReqName = Request name
     * \iparam       Params =  value of the request name
     */
    /****************************************************************************/
    void ServiceRequest(QString ReqName, QStringList Params);

protected:
    /**
     * \brief This method is called when the base received the  Go signal.
     *
     * This means that everything is fine and normal operation started.
     * We are running in our own thread.\n
     * We create all controllers used (the according method in derived classes
     * \ref CreateControllersAndThreads is also called), then initialize the controllers
     * and finally attach them to the corresponding threads and start them.\n
     * If something goes wrong, the master thread (and application) will be stopped.
     */
    /****************************************************************************/
    void OnGoReceived();
    /****************************************************************************/
    /**
     * \brief This method is called when the base class received the Stop signal.
     *
     * This means that normal operation will stop after processing this signal.
     * We are still running in our own thread.\n
     * We make some cleanup work (all used controllers) and finally destroy all
     * controllers and threads.
     */
    /****************************************************************************/
    void OnStopReceived();
    /****************************************************************************/
    /**
     * \brief This method is called when the power fail signal is received.
     *
     * This means that normal operation should stop and the thread controller should
     * save the current state and do cleanup work and destroy all objects it holds.
     *
     */
    /****************************************************************************/
    virtual void OnPowerFail(const Global::PowerFailStages PowerFailStage);
    /****************************************************************************/

private:
    /**
     * \brief To register commands handled by this thread controller.
     *
     */
    /****************************************************************************/
    void RegisterCommands();
    /****************************************************************************/
    /**
     * \brief To handle acknowledgements for commands originating from this object.
     *
     * \param[in]   tRefType     Reference of the command.
     * \param[in]   AckOKNOK     OK or NOK.
     */
    /****************************************************************************/
    void OnAcknowledge(Global::tRefType, const Global::AckOKNOK &);
    /****************************************************************************/
    /**
     * \brief To register commands handled by this thread controller.
     *
     */
    /****************************************************************************/
    void RequestForDataContainers();
    /****************************************************************************/
    /**
     * \brief To signals and slots associated with this object.
     *
     */
    /****************************************************************************/
    void ConnectSignalsnSlots();
    /****************************************************************************/

    DeviceProcessor *mp_DeviceProcessor; //!< DeviceProcessor Reference to make device calls.

    bool m_ProcessSettings;
};


} // end namespace DeviceControl
#endif // SERVICEDEVICECONTROLLER_H

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
#if 0
#include <ServiceDeviceController/Include/Commands/CmdGetDeviceCANID.h>
#include <ServiceDeviceController/Include/Commands/CmdGetDeviceFirmwareInfo.h>
#endif

#include <ServiceDeviceController/Include/Commands/CmdCalibrateDevice.h>
#include <ServiceDeviceController/Include/Commands/CmdAbortTest.h>
#include <ServiceDeviceController/Include/Commands/CmdHeatingTest.h>
#include <ServiceDeviceController/Include/Commands/CmdRotaryValveTest.h>
#include <ServiceDeviceController/Include/Commands/CmdLSensorDetectingTest.h>

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
     * \brief Command of type CmdHeatingTest received.
     *
     * \param[in]       Ref                 Reference of command.
     * \param[in]       Cmd                 Command.
     */
    /****************************************************************************/
    void OnSDC_HeatingTest(Global::tRefType Ref, const DeviceCommandProcessor::CmdHeatingTest &Cmd);
    /****************************************************************************/
    /**
     * \brief Command of type CmdRotaryValveTest received.
     *
     * \param[in]       Ref                 Reference of command.
     * \param[in]       Cmd                 Command.
     */
    /****************************************************************************/
    void OnSDC_RotaryValveTest(Global::tRefType Ref, const DeviceCommandProcessor::CmdRotaryValveTest &Cmd);
    /****************************************************************************/
    /**
     * \brief Command of type CmdLSensorDetectingTest received.
     *
     * \param[in]       Ref                 Reference of command.
     * \param[in]       Cmd                 Command.
     */
    /****************************************************************************/
    void OnSDC_LSensorDetectingTest(Global::tRefType Ref, const DeviceCommandProcessor::CmdLSensorDetectingTest &Cmd);
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
     * \brief Command of type CmdCalibrateDevice received.
     *
     * \iparam       Ref                 Reference of command.
     * \iparam       Cmd                 Command.
     */
    /****************************************************************************/
    void OnCmdCalibrateDevice(Global::tRefType Ref, const DeviceCommandProcessor::CmdCalibrateDevice &Cmd);

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

protected:
    /**
     * \brief This method is called when the base received the \ref Go signal.
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
     * \brief This method is called when the base class received the \ref Stop signal.
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
    bool m_Adjustment;
    bool m_DeviceDataContainers;
};


} // end namespace DeviceControl
#endif // SERVICEDEVICECONTROLLER_H

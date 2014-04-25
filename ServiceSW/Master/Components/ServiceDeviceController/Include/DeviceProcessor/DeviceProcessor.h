/****************************************************************************/
/*! \file DeviceProcessor.h
 *
 *  \brief Definition file for class DeviceProcessor.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-03-22
 *  $Author:    $ Srivathsa HH
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

#ifndef DEVICECOMMANDPROCESSOR_DEVICE_COMMANDPROCESSOR_H
#define DEVICECOMMANDPROCESSOR_DEVICE_COMMANDPROCESSOR_H


#include <Global/Include/GlobalDefines.h>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Core/Include/ServiceDefines.h"


class WrapperUtils;
class WrapperFmTempControl;
class WrapperFmStepperMotor;
class WrapperFmPressureControl;


namespace DeviceControl {

class ServiceDeviceController;
class IDeviceProcessing;

typedef enum {
    TEST_X_REF_RUN,
    TEST_YZ_REF_RUN,
    TEST_AGITATOR_REF_RUN,
    TEST_TRANSTN_REF_RUN,
    TEST_OVENLID_REF_RUN,
    TEST_DRAWER_REF_RUN,
    TEST_OTHER_RUNS
}TestState_t;

/****************************************************************************/
/**
 * \brief This class is a helper to do handle device processing related commands.
 *
 * This class has thread controller and Interface for the device command
 * processing. It communicates with the real hardware with signals and slots.
 * \warning This class is not thread safe!
 */
/****************************************************************************/

class DeviceProcessor : public QObject
{
    Q_OBJECT

public:
    /**
     * \brief Constructor.
     *
     * \param[in]   ServiceDeviceController     Reference of ServiceDeviceController.
     * \param[in]   IDeviceProcessing           Reference of IDeviceProcessing.
     */
    /****************************************************************************/
    DeviceProcessor(ServiceDeviceController &rDCP, IDeviceProcessing &iDevProc);
    /****************************************************************************/
    /**
     * \brief To initiate device connections
     */
    /****************************************************************************/
    void Connect();
    /****************************************************************************/
    /**
     * \brief  To initiatize devices
     */
    /****************************************************************************/
    void Initialize();
    /****************************************************************************/
    /**
     * \brief Get Pointer to DataManager Object
     * \return True if Device Initialization is completed.
     */
    /****************************************************************************/
    bool IsInitialized();
    /****************************************************************************/
    /****************************************************************************/
    /**
     * \brief Helper function to create wrappers
     */
    /****************************************************************************/
    void CreateWrappers();
public slots:
    /****************************************************************************/
    /**
     * \brief Handle Command of type CmdAbortTest received.
     *
     * \param[in]       Ref                 Reference of command.
     * \param[in]       Cmd                 Command.
     */
    /****************************************************************************/
    void OnAbortTest(Global::tRefType Ref, quint32 id);
    /****************************************************************************/
    /**
     * \brief Handle Command of type CmdHeatingTest received.
     *
     * \param[in]       Ref                 Reference of command.
     * \param[in]       Cmd                 Command.
     */
    /****************************************************************************/
    void OnHeatingTest(Global::tRefType Ref, quint32 id,
                         quint8 HeaterIndex, quint8 CmdType);
    /****************************************************************************/
    /**
     * \brief Handle Command of type CmdRotaryValveTest received.
     *
     * \param[in]       Ref                 Reference of command.
     * \param[in]       Cmd                 Command.
     */
    /****************************************************************************/
    void OnRotaryValveTest(Global::tRefType Ref, quint32 id,
                         qint32 Postion, quint8 CmdType);
    /****************************************************************************/
    /**
     * \brief Handle Command of type CmdLSensorDetectingTest received.
     *
     * \param[in]       Ref                 Reference of command.
     * \param[in]       Cmd                 Command.
     */
    /****************************************************************************/
    void OnLSensorDetectingTest(Global::tRefType Ref, quint32 id,
                         qint32 Position);

    /****************************************************************************/
    /**
     * \brief Calibrate Device
     *
     * \iparam       CmdType            Command type
     */
    /****************************************************************************/
    void OnCalibrateDevice(Service::DeviceCalibrationCmdType CmdType);

signals:

    /****************************************************************************/
    /**
     * \brief Signal for sending a return Message to MasterThread.
     *
     * \param[in]   ReturnString         QString ReturnString.
     */
    /****************************************************************************/
    void ReturnMessagetoMain(const QString ReturnString);
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
     * \brief Returns Calibration Init message to Main Thread Controller.
     *
     * \iparam   Message    QString message to be sent.
     * \iparam   OkStatus   Calibration status
     */
    /****************************************************************************/
    void ReturnCalibrationInitMessagetoMain(const QString &Message, bool OkStatus);
#if 0
    /****************************************************************************/
    /**
     * \brief Returns CANID message to Main Thread Controller.
     *
     * \param[in]   Message    QString message to be sent.
     */
    /****************************************************************************/
    void ReturnCANIDMessagetoMain(const QString &Message, QString canID, QString ExpectedCanID);
    /****************************************************************************/
    /**
     * \brief Returns Firmware Info message to Main Thread Controller.
     *
     * \param[in]   Message    QString message to be sent.
     */
    /****************************************************************************/
    void ReturnFirmwareMessagetoMain(const QString &Message, QString firmwareInfo);
    /****************************************************************************/
    /**
     * \brief Returns Rack RFID message to Main Thread Controller.
     *
     * \param[in]   Message    QString message to be sent.
     */

#endif
private:

    /****************************************************************************/
    /**
     * \brief To Test heating of rotary valve
     *
     * \param[in]       DeviceId            Device Instance ID
     */
    /****************************************************************************/
    qint32 TestRVHeating(quint32 DeviceId);
    /****************************************************************************/
    /**
     * \brief To Test heating of oven
     *
     * \param[in]       DeviceId            Device Instance ID
     */
    /****************************************************************************/
    qint32 TestOvenHeating(quint32 DeviceId);
    /****************************************************************************/
    /**
     * \brief To Test heating of level sensor
     *
     * \param[in]       DeviceId            Device Instance ID
     */
    /****************************************************************************/
    qint32 TestLSensorHeating(quint32 DeviceId);
    /****************************************************************************/
    /**
     * \brief To Test heating of tube1/tube2
     *
     * \param[in]       DeviceId            Device Instance ID
     * \param[in]       TubeIndex           Index of Tube
     */
    /****************************************************************************/
    qint32 TestTubeHeating(quint32 DeviceId, quint8 TubeIndex);
    /****************************************************************************/
    /**
     * \brief Move rotary valve to initial position
     *
     * \param[in]       DeviceId            Device Instance ID
     */
    /****************************************************************************/
    qint32 TestLSensorDetecting(quint32 DeviceId, qint32 Position);
    /****************************************************************************/
    /**
     * \brief To Test detecting of level sensor
     *
     * \param[in]       DeviceId            Device Instance ID
     ** \param[in]      Position            Tube position for sucking
     */
    /****************************************************************************/
    qint32 MoveRVToInitPos(quint32 DeviceId);
    qint32 MoveRVToTubePos(quint32 DeviceId, qint32 Pos);
    qint32 MoveRVToSealPos(quint32 DeviceId, qint32 Pos);
#if 0
    /**
     * \brief To convert device layer codes into meaningful strings.
     *
     * \param[in]       ReturnCode_t         ReturnCode from device layer
     * \return          QString              String to indicate sucess or failure
     */
    /****************************************************************************/
    QString ReturnStringForID(ReturnCode_t HdlInfo);
    /****************************************************************************/
    /**
     * \brief To check if device action was successful or not.
     *
     * \param[in]       ReturnCode_t         ReturnCode from device layer
     * \return          bool               bool indicate sucess or failure
     */
    /****************************************************************************/
    bool CheckHardwareInfo(ReturnCode_t HdlInfo);

#endif

    ServiceDeviceController     &m_rCDCP;               //!< ServiceDevice Controller Reference
    IDeviceProcessing           &m_rIdevProc;           //!< IDeviceProcessing reference to interact with device
    bool                        m_IsConfigured;         //!< Indicates if configuration is successfully completed
    TestState_t                 m_CurrentAction;
    bool                        m_ExhaustInit;
    bool                        m_WaterDeviceInit;
    bool                        m_HoodDeviceInit;
    bool                        m_DeviceLightInit;

#if 0
    WrapperDeviceXyz*           mp_xyzDeviceLeft;       //!< Wrapper for XYZ device to simplify device interaction
#endif

    bool                        m_UserAbort;
    WrapperUtils*               mp_Utils;
    // Temperature control modules
    WrapperFmTempControl*       mp_TempRV;
    WrapperFmTempControl*       mp_TempRetortSide;
    WrapperFmTempControl*       mp_TempRetortBottom;
    WrapperFmTempControl*       mp_TempOvenTop;
    WrapperFmTempControl*       mp_TempOvenBottom;
    WrapperFmTempControl*       mp_TempLSensor;
    WrapperFmTempControl*       mp_TempTube1;
    WrapperFmTempControl*       mp_TempTube2;
    // Stepper motor module
    WrapperFmStepperMotor*      mp_MotorRV;
    // Pressure control module
    WrapperFmPressureControl*   mp_PressPump;
};

} // end namespace DeviceCommandProcessor

#endif // COLORADODEVICECOMMANDPROCESSOR_DEVICE_COMMANDPROCESSOR_H

/****************************************************************************/
/*! \file ManufacturingTestHandler.h
 *
 *  \brief Definition file for class ManufacturingTestHandler.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2014-05-20
 *  $Author:    $ Sunny Qu
 *
 *  \b Company:
 *
 *       Leica Biosystems Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef MANUFACTURING_TEST_HANDLER_H
#define MANUFACTURING_TEST_HANDLER_H


#include <Global/Include/GlobalDefines.h>
#include <QProcess>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Core/Include/ServiceDefines.h"
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmDigitalInput.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmDigitalOutput.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmBaseModule.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmBootLoader.h>


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

typedef enum {
    RV_INITIALIZING = 0,
    RV_MOVE_TO_TUBE_POSITION,
    RV_MOVE_TO_SEALING_POSITION,
    LS_HEATING,
    RETORT_FILLING,
    RETORT_DRAINING,
    PUMP_CREATE_PRESSURE,
    PUMP_KEEP_PRESSURE,
    PUMP_RELEASE_PRESSURE,
    SYSTEM_FLUSH,
    WAIT_CONFIRM,
    WAIT_CONFIRM2,
    HIDE_MESSAGE,
    INFORM_DONE,
} TestCurStatus_t;

/****************************************************************************/
/**
 * \brief This class is a helper to do handle device processing related commands.
 *
 * This class has thread controller and Interface for the device command
 * processing. It communicates with the real hardware with signals and slots.
 * \warning This class is not thread safe!
 */
/****************************************************************************/

class ManufacturingTestHandler : public QObject
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
    ManufacturingTestHandler(IDeviceProcessing &iDevProc);

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
     * \param[in]       id                 Test case id.
     * \param[in]       AbortTestCaseId                It is only for abort
     */
    /****************************************************************************/
    void OnAbortTest(Global::tRefType Ref, quint32 id, quint32 AbortTestCaseIds=0);

    /****************************************************************************/
    /**
     * \brief Module Manufacturing Test for Himalaya Manufacturing Diagnostic
     *
     * \iparam       TestId            Module Test Case Id
     */
    /****************************************************************************/
    void PerformModuleManufacturingTest(Service::ModuleTestCaseID TestId, Service::ModuleTestCaseID AbortTestCaseId = Service::TEST_CASE_ID_UNUSED);


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
private:
    /****************************************************************************/
    /**
     * \brief To test Rotary Valve Initialization.
     */
    /****************************************************************************/
    qint32 TestRVInitialization( );

    /****************************************************************************/
    /**
     * \brief To test Rotary Valve Selecting function and Sealing Function.
     * \iparam   Id = Test case id
     */
    /****************************************************************************/
    qint32 TestRVSelectingAndSealing( Service::ModuleTestCaseID Id );


    /****************************************************************************/
    /**
     * \brief To move Rotary Valve to tube position.
     */
    /****************************************************************************/
    qint32 MoveRVToTubePos(qint32 Pos);

    /****************************************************************************/
    /**
     * \brief To move Rotary Valve to sealing position.
     */
    /****************************************************************************/
    qint32 MoveRVToSealPos(qint32 Pos);

    /****************************************************************************/
    /**
     * \brief To test Rotary Valve heating at station-test
     */
    /****************************************************************************/
    qint32 TestRVHeatingStation( );

    /****************************************************************************/
    /**
     * \brief To test Rotary Valve heating at end-test
     */
    /****************************************************************************/
    qint32 TestRVHeatingEnd( );

    /****************************************************************************/
    /**
     * \brief To heat level sensor for other test (i.e rotary valve heating end)
     */
    /****************************************************************************/
    qint32 HeatingLevelSensor();


    /****************************************************************************/
    /**
     * \brief To test heating for tubes (liquid, air).
     */
    /****************************************************************************/
    qint32 TestLAHeatingTube(Service::ModuleTestCaseID_t Id);

    /****************************************************************************/
    /**
     * \brief To Test Voltage and Current for Slave Board.
     */
    /****************************************************************************/
    qint32 TestMainControlASB(Service::ModuleTestCaseID_t Id);

    /****************************************************************************/
    /**
     * \brief To Test heating of oven
     *
     */
    /****************************************************************************/
    qint32 TestOvenHeating();

    /****************************************************************************/
    /**
     * \brief To Test heating of oven with water
     */
    /****************************************************************************/
    qint32 TestOvenHeatingWater( );


    /****************************************************************************/
    /**
     * \brief To Test detecting of cover sensor of Oven
     */
    /**************************************AbortExhaustFan**************************************/
    qint32 TestOvenCoverSensor();

    /****************************************************************************/
    /**
     * \brief To Test speaker of system
     */
    /****************************************************************************/
    qint32 TestSystemSpeaker();

    /****************************************************************************/
    /**
     * \brief To Test remote local alarm of system
     */
    /****************************************************************************/
    qint32 TestSystemAlarm();

    /****************************************************************************/
    /**
     * \brief To Test 110V/220V switch of system
     */
    /****************************************************************************/
    qint32 TestSystem110v220vSwitch();
    /****************************************************************************/
    /**
     * \brief To Test detecting of cover sensor of Oven
     */
    /****************************************************************************/
    qint32 TestLidLocker();

    /****************************************************************************/
    /**
     * \brief To Test heating of retort
     *
     */
    /****************************************************************************/
    qint32 TestRetortHeating();

    /****************************************************************************/
    /**
     * \brief To Test heating of retort's level sensor
     *
     */
    /****************************************************************************/
    qint32 TestRetortLevelSensorHeating();

    /****************************************************************************/
    /**
     * \brief To test dectecting of level sensor
     *
     */
    /****************************************************************************/
    qint32 TestRetortLevelSensorDetecting();

    /****************************************************************************/
    /**
     * \brief To Test heating of retort with water
     */
    /****************************************************************************/
    qint32 TestRetortHeatingWater( );
     /***************************************************************************/
     /** \brief To Test mains relay of system
     */
    /****************************************************************************/
    qint32 TestSystemMainsRelay();

    /****************************************************************************/
    /**
     * \brief Abort exhaust fan when call OnAbortTest
     */
    /****************************************************************************/
    qint32 TestSystemExhaustFan();

    /****************************************************************************/

    /****************************************************************************/
    /**
     * \brief To Test exhaust fan of systemAbortExhaustFan
     */
    /****************************************************************************/
    void AbortExhaustFanOperation();

    /****************************************************************************/
    /**
     * \brief To Test overflow of system
     */
    /****************************************************************************/
    qint32 TestSystemOverflow();

    /****************************************************************************/
    /**
     * \brief To test system sealing test
     */
    /****************************************************************************/
    qint32 TestSystemSealing(int CurStep);


    /****************************************************************************/
    /**
     * \brief To cleaning system
     */
    /****************************************************************************/
    qint32 CleaningSystem();


    /****************************************************************************/
    /**
     * \brief Create Pressure
     */
    /****************************************************************************/
    bool CreatePressure(int waitSecond, qreal targetPressure, qreal departure);

    /****************************************************************************/
    /**
     * \brief Update Firmware
     */
    /****************************************************************************/
    qint32 UpdateFirmware();

    /****************************************************************************/
    /**
     * \brief Get Slave Configuration Information
     */
    /****************************************************************************/
    void GetSlaveInformation();

    /****************************************************************************/
    /**
     * \brief Pressure sensor calibration
     */
    /****************************************************************************/
    void CalibratePressureSensor();

    /****************************************************************************/
    /**
     * \brief Set fail reason for test case
     * \param[in] Id                Test case id
     * \param[in] FailMsg           Fail message
     */
    /****************************************************************************/
    void SetFailReason(Service::ModuleTestCaseID Id, const QString& FailMsg);

    /****************************************************************************/
    /**
     * \brief RefreshTestStatustoMain
     * \param[in] TestCaseName      Test case name
     * \param[in] CurStatus         Current status
     * \param[in] param             paramater
     */
    /****************************************************************************/
    void EmitRefreshTestStatustoMain(const QString& TestCaseName, TestCurStatus_t CurStatus, qreal Param=0);



    IDeviceProcessing           &m_rIdevProc;           //!< IDeviceProcessing reference to interact with device
    bool                        m_IsConfigured;         //!< Indicates if configuration is successfully completed
    bool                        m_UserAbort;
    WrapperUtils*               mp_Utils;
    // Temperature control modules
    WrapperFmTempControl*       mp_TempOvenTop;
    WrapperFmTempControl*       mp_TempOvenBottom;

    WrapperFmTempControl*       mp_TempRetortSide;
    WrapperFmTempControl*       mp_TempRetortBttom;

    WrapperFmDigitalInput*      mp_TempRetortInputLid;

    WrapperFmDigitalInput*      mp_DigitalInputOven;
    WrapperFmDigitalOutput*     mp_DigitalOutputMainRelay;

    WrapperFmTempControl*       mp_TempTubeLiquid;
    WrapperFmTempControl*       mp_TempTubeAir;

    WrapperFmStepperMotor*      mp_MotorRV;
    WrapperFmTempControl*       mp_TempRV;
    WrapperFmPressureControl*   mp_PressPump;

    WrapperFmTempControl*       mp_TempLSensor;

    WrapperFmDigitalInput*      mp_DIRemoteAlarm;
    WrapperFmDigitalInput*      mp_DILocalAlarm;

    WrapperFmDigitalOutput*      mp_DORemoteAlarm;
    WrapperFmDigitalOutput*      mp_DOLocalAlarm;

    WrapperFmBaseModule*        mp_BaseModule3;
    WrapperFmBaseModule*        mp_BaseModule5;
    WrapperFmBaseModule*        mp_BaseModule15;


    QProcess*   mp_SpeakProc;

    bool        m_Continue;

};

} // end namespace DeviceCommandProcessor

#endif // MANUFACTURING_TEST_HANDLER_H

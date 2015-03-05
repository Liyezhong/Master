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
 *  (C) Copyright 2014 by Leica Biosystems Shanghai. All rights reserved.
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


/****************************************************************************/
/**
 * \brief Enum Test case status.
 */
/****************************************************************************/
typedef enum {
    RV_INITIALIZING = 0, //!< rotary valve initialize
    RV_MOVE_TO_TUBE_POSITION, //!< rotary valve position move to tube
    RV_MOVE_TO_SEALING_POSITION, //!<  rotary valve position move to sealing
    LS_HEATING, //!< level sensor heating
    RETORT_FILLING, //!< retort filling
    RETORT_DRAINING, //!< retort draining
    PUMP_CREATE_PRESSURE, //!< pump create pressure
    PUMP_KEEP_PRESSURE, //!< pump keep pressure
    PUMP_RELEASE_PRESSURE, //!< pump release pressure
    PUMP_CURRENT_PRESSURE, //!< pump current pressure
    SYSTEM_FLUSH, //!< flush system
    WAIT_CONFIRM, //!< waiting confirm
    WAIT_CONFIRM2, //!< waiting confirm
    HIDE_MESSAGE,  //!< hide message
    INFORM_DONE //!< inform done
} TestCurStatus_t; //!< test state enumeration

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
    friend class CTestServiceDeviceController;
public:
    /**
     * \brief Constructor.
     *
     * \param[in]   iDevProc           Reference of IDeviceProcessing.
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
     * \param[in]       id                  Test case id.
     * \param[in]       AbortTestCaseIds    It is only for abort
     */
    /****************************************************************************/
    void OnAbortTest(Global::tRefType Ref, quint32 id, quint32 AbortTestCaseIds=0);

    /****************************************************************************/
    /**
     * \brief Module Manufacturing Test for Himalaya Manufacturing Diagnostic
     *
     * \iparam       TestId            Module Test Case Id
     * \param[in]       AbortTestCaseId    It is only for abort
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

    /****************************************************************************/
    /**
     * \brief Returns service request message to Main Thread Controller.
     *
     * \iparam   ReqName = request name
     * \iparam   ErrorCode = error code
     * \iparam   Results = store result
     */
    /****************************************************************************/
    void ReturnServiceRequestResult(QString ReqName, int ErrorCode, QStringList Results);

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
    bool CreatePressure(int waitSecond, float targetPressure, float departure, const QString& TestCaseName);

    /****************************************************************************/
    /**
     * \brief Update Firmware
     */
    /****************************************************************************/
    qint32 UpdateFirmware();

    /****************************************************************************/
    /**
     * \brief Write board option to slave board
     * \param[in] p_Bootloader             point to boot loader handle
     * \param[in] SlaveType                slave type (3, 5, 15)
     */
    /****************************************************************************/
    bool WriteBoardOption(WrapperFmBootLoader *p_Bootloader, HimSlaveType_t SlaveType);

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
     * \brief Set Slave to stand by.
     */
    /****************************************************************************/
    void SetSlaveStandby();

    /****************************************************************************/
    /**
     *  \brief Auto set the heater switch type for ASB3 according to ASB5
     *  \iparam Id = test case Id
     */
    /****************************************************************************/
    qint32 AutoSetASB3HeaterSwitchType(Service::ModuleTestCaseID Id);

    /****************************************************************************/
    /**
     * \brief Set initialization value when starting slave.
     */
    /****************************************************************************/
    qint32 SystemSelfTest();


    /****************************************************************************/
    /**
     * \brief Reset operation time for some modules.
     */
    /****************************************************************************/
    qint32 ResetOperationTime();

    /****************************************************************************/
    /**
     *  \brief Reset operation time to DeviceLifeCycleRecord.xml.
     *  \iparam ModuleName = The name of module
     *  \iparam SubModuleName = The sub module name
     *  \iparam ParamName = The parameter name
     *  \return true or false
     */
    /****************************************************************************/
    bool ResetOperationTime2Ebox(const QString& ModuleName, const QString& SubModuleName, const QString& ParamName );

    /****************************************************************************/
    /**
     *  \brief Reset operation time to InstrumentHistoryRef.xml.
     *  \iparam ModuleName = The name of module
     *  \iparam SubModuleName = The sub module name
     *  \iparam ParamName = the default param name
     *  \return true or false
     */
    /****************************************************************************/
    bool ResetOperationTime2Ref(const QString& ModuleName, const QString& SubModuleName, const QString& ParamName);

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
    WrapperFmTempControl*       mp_TempRetortBottom;

    WrapperFmDigitalInput*      mp_DIRetortLid;

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

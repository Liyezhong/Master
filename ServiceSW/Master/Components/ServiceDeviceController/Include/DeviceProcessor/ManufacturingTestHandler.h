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
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Core/Include/ServiceDefines.h"
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmDigitalInput.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmDigitalOutput.h>
#include <ServiceDeviceController/Include/DeviceProcessor/Helper/WrapperFmBaseModule.h>


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
     * \param[in]       Cmd                 Command.
     */
    /****************************************************************************/
    void OnAbortTest(Global::tRefType Ref, quint32 id);

    /****************************************************************************/
    /**
     * \brief Module Manufacturing Test for Himalaya Manufacturing Diagnostic
     *
     * \iparam       TestId            Module Test Case Id
     */
    /****************************************************************************/
    void PerformModuleManufacturingTest(Service::ModuleTestCaseID TestId);


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
    /****************************************************************************/
    qint32 TestOvenCoverSensor();


    /****************************************************************************/
    /**
     * \brief Set fail reason for test case
     * \param[in] Id                Test case id
     * \param[in] FailMsg           Fail message
     */
    /****************************************************************************/
    void SetFailReason(Service::ModuleTestCaseID Id, const QString& FailMsg);


    IDeviceProcessing           &m_rIdevProc;           //!< IDeviceProcessing reference to interact with device
    bool                        m_IsConfigured;         //!< Indicates if configuration is successfully completed
    bool                        m_UserAbort;
    WrapperUtils*               mp_Utils;
    // Temperature control modules
    WrapperFmTempControl*       mp_TempOvenTop;
    WrapperFmTempControl*       mp_TempOvenBottom;

    WrapperFmDigitalInput*      mp_DigitalInpputOven;
    WrapperFmDigitalOutput*     mp_DigitalOutputMainRelay;

    WrapperFmTempControl*       mp_TempTubeLiquid;
    WrapperFmTempControl*       mp_TempTubeAir;

    WrapperFmStepperMotor*      mp_MotorRV;
    WrapperFmPressureControl*   mp_PressPump;

    WrapperFmBaseModule*        mp_BaseModule3;
    WrapperFmBaseModule*        mp_BaseModule5;
    WrapperFmBaseModule*        mp_BaseModule15;

};

} // end namespace DeviceCommandProcessor

#endif // MANUFACTURING_TEST_HANDLER_H

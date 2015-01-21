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
#include "ServiceDeviceController/Include/DeviceProcessor/ManufacturingTestHandler.h"
#include "ServiceDeviceController/Include/DeviceProcessor/ServiceTestHandler.h"

class WrapperUtils;
class WrapperFmTempControl;
class WrapperFmStepperMotor;
class WrapperFmPressureControl;


namespace DeviceControl {

class ServiceDeviceController;
class IDeviceProcessing;

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
     * \param[in]   iDevProc     IDeviceProcessing instance
     */
    /****************************************************************************/
    DeviceProcessor(IDeviceProcessing &iDevProc);
    /****************************************************************************/
    /**
     * \brief To initiate device connections
     */
    /****************************************************************************/
    void Connect();

    /****************************************************************************/
    /**
     * \brief To initiate device
     */
    /****************************************************************************/
    void InitialDevice();

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
     * \param[in]       id                  Id.
     */
    /****************************************************************************/
    void OnAbortTest(Global::tRefType Ref, quint32 id);

    /****************************************************************************/
    /**
     * \brief Module Manufacturing Test for Himalaya Manufacturing Diagnostic
     *
     * \iparam       TestName            Module Test Name
     * \iparam       AbortTestCaseId     id of the abort test case
     */
    /****************************************************************************/
    void OnModuleManufacturingTest(Service::ModuleTestCaseID TestName, Service::ModuleTestCaseID AbortTestCaseId=Service::TEST_CASE_ID_UNUSED);

    /****************************************************************************/
    /**
     * \brief service request function for Himalaya Manufacturing Diagnostic
     *
     * \iparam       ReqName           Request Name
     * \iparam       Params            parameter list
     */
    /****************************************************************************/
    void OnServiceRequest(QString ReqName, QStringList Params);

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
     * \iparam   ReqName = Request name
     * \iparam   ErrorCode = error code
     * \iparam   Results = store results
     */
    /****************************************************************************/
    void ReturnServiceRequestResult(QString ReqName, int ErrorCode, QStringList Results);

private:
    IDeviceProcessing           &m_rIdevProc;           //!< IDeviceProcessing reference to interact with device
    bool                        m_IsConfigured;         //!< Indicates if configuration is successfully completed

    bool                        m_UserAbort;
    WrapperUtils*               mp_Utils;

    ManufacturingTestHandler*   mp_ManufacturingTestHandler; //!< Manufacturing Test Handler helper class

    ServiceTestHandler*         mp_ServicecTestHandler;
};

} // end namespace DeviceCommandProcessor

#endif // COLORADODEVICECOMMANDPROCESSOR_DEVICE_COMMANDPROCESSOR_H

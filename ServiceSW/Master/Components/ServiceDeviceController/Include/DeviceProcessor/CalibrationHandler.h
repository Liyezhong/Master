/****************************************************************************/
/*! \file ServiceDeviceController/Include/DeviceProcessor/CalibrationHandler.h
 *
 *  \brief Definition file for class CalibrationHandler.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-12-24
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

#ifndef DEVICECOMMANDPROCESSOR_CALIBRATION_HANDLER_H
#define DEVICECOMMANDPROCESSOR_CALIBRATION_HANDLER_H


#include <Global/Include/GlobalDefines.h>
//#include "DeviceControl/Include/Global/DeviceControl.h"
#include "Core/Include/ServiceDefines.h"
#include <ServiceDataManager/Include/ServiceParameters.h>
//#include "ServiceUtils.h"

namespace DeviceControl {

class ServiceDeviceController;
class IDeviceProcessing;
class DeviceProcessor;

//!< Equation
#define STRINGX(X) ((X.at(0).toInt() + X.at(1).toInt())/2)

/****************************************************************************/
/**
 * \brief This class is a helper to do handle device processing related commands.
 *
 * This class has thread controller and Interface for the device command
 * processing. It communicates with the real hardware with signals and slots.
 * \warning This class is not thread safe!
 */
/****************************************************************************/

class CalibrationHandler : public QObject
{
    Q_OBJECT

public:
    /**
     * \brief Constructor.
     *
     */
    /****************************************************************************/
    CalibrationHandler(DeviceProcessor &DevProc);
    /****************************************************************************/

public slots:

    /**
     * \brief Calibrate Device
     *
     * \iparam       CmdType            Command type
     */
    /****************************************************************************/
    void OnCalibrateDevice(Service::DeviceCalibrationCmdType CmdType);
    /****************************************************************************/


signals:
    /****************************************************************************/
    /**
     * \brief Signal for sending a return Message to MasterThread.
     *
     * \iparam   ReturnString         QString ReturnString.
     */
    /****************************************************************************/
    void ReturnMessagetoMain(const QString ReturnString);
    /****************************************************************************/
    /**
     * \brief Returns an Error message to Main Thread Controller.
     *
     * \iparam   Message    QString message to be sent.
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
     * \brief Returns Calibration Init message to Main Thread Controller.
     *
     */
    /****************************************************************************/
    void ReconfigureDevices();
    /****************************************************************************/

private:

    void PerformOvenLidCalibrationInit();

    void PerformOvenLidCalibration();

    //WrapperDeviceOven*          mp_DeviceOven;          //!< Wrapper for Oven device to simplify device interaction
    void PerformPressureSensorCalibration();

    DeviceProcessor &m_rDeviceProcessor;    //!< DeviceProcessor

};

} // end namespace DeviceCommandProcessor

#endif // DEVICECOMMANDPROCESSOR_CALIBRATION_HANDLER_H

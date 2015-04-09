/****************************************************************************/
/*! \file Core/Include/CalibrationHandler.h
 *
 *  \brief CalibrationHandler class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-11-14
 *   $Author:  $ Srivathsa HH, Soumya. D
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#ifndef CORE_CALIBRATIONHANDLER_H
#define CORE_CALIBRATIONHANDLER_H

#include "Core/Include/ServiceGUIConnector.h"
#include "MainMenu/Include/MainWindow.h"
#include "MainMenu/Include/MenuGroup.h"
#include "Calibration/Include/PressureSensor.h"
#include "Calibration/Include/Touchscreen.h"
#include "Core/Include/ServiceDefines.h"


namespace Core {

/****************************************************************************/
/*!
 *  \brief This class implements device calibration functionality
 */
/****************************************************************************/
class CCalibrationHanlder : public QObject {
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam p_ServiceGUIConnector = serivce gui connector
     *  \iparam p_MainWindow = parent windows
     */
    /****************************************************************************/
    CCalibrationHanlder(Core::CServiceGUIConnector *p_ServiceGUIConnector, MainMenu::CMainWindow *p_MainWindow);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CCalibrationHanlder();

    /****************************************************************************/
    /*!
     *  \brief To load calibration gui componenets.
     */
    /****************************************************************************/
    void LoadCalibrationGUIComponenets();

    /****************************************************************************/
    /*!
     *  \brief To refresh calibration message to main
     *  \iparam Status = module test status
     */
    /****************************************************************************/
    void RefreshCalibrationMessagetoMain(const Service::ModuleTestStatus &Status);

private:
    Core::CServiceGUIConnector              *mp_ServiceConnector;           //!< Service GUI connector object
    QEventLoop                              m_LoopCalibrationStart;         //!< Loop for blocking Calibration commands.
    MainMenu::CMainWindow                   *mp_MainWindow;                 //!< The main window of the application

    // Calibration
    MainMenu::CMenuGroup                    *mp_CalibrationGroup;           //!< Menu group containing calibration dialogs
    Calibration::CPressureSensor            *mp_PressureSensor;             //!< Calibration for the pressure sensor.
    Calibration::CTouchscreen               *mp_Touchscreen;                //!< Starter for the touchscreen calibration

    /****************************************************************************/
    /*!
     *  \brief To preform calibration
     *  \iparam Title = title string
     *  \iparam GBox = GBox string
     *  \iparam Instr = index string
     *  \return true or false
     */
    /****************************************************************************/
    bool PerformCalibration(QString& Title, QString& GBox, QString& Instr);

    /****************************************************************************/
    /*!
     *  \brief To get calibration response
     *  \return true or false
     */
    /****************************************************************************/
    bool GetCalibrationResponse();

    /****************************************************************************/
    /*!
     *  \brief Service calibation
     *  \return true or false
     */
    /****************************************************************************/
    bool ServiceCalibation();

    /****************************************************************************/
    /*!
     *  \brief Manufacturing calibation
     *  \return true or flase
     */
    /****************************************************************************/
    bool ManufacturingCalibation();

    /****************************************************************************/
    /*!
     *  \brief Set calibration date to current config
     */
    /****************************************************************************/
    void SetCalibrationDate();

signals:
    /****************************************************************************/
    /**
     * \brief Signal emitted for ovenlid init calibration
     */
    /****************************************************************************/
    void OvenLidInitCalibrationRequest();
    /****************************************************************************/
    /**
     * \brief Signal emitted for PressureSensor calibration
     */
    /****************************************************************************/
    void PressureSensorCalibrationRequest();

    /****************************************************************************/
    /**
       * \brief Signal emitted to perform manufacturing tests
       * \iparam Test = Test name
       * \iparam AbortTestID = abort test id
       */
    /****************************************************************************/
    void PerformManufacturingTest(Service::ModuleTestCaseID Test, Service::ModuleTestCaseID AbortTestID=Service::TEST_CASE_ID_UNUSED);

    /*******************************************************************************/
    /*!
     *  \brief Signal emitted to shut down system
     *  \iparam NeedUpdate = flag of need update
     */
    /*******************************************************************************/
    void ShutdownSystem(bool NeedUpdate=true);

public slots:

    /****************************************************************************/
    /**
     * \brief slot for Pressure Sensor calibration
     */
    /****************************************************************************/
    void OnPressureSensorCalibration();

    /****************************************************************************/
    /*!
     *  \brief To Show message
     *  \iparam Message = message text
     */
    /****************************************************************************/
    void ShowMessage(const QString &Message);

    /****************************************************************************/
    /*!
     *  \brief To Show error message dialog
     *  \iparam Message = message text
     */
    /****************************************************************************/
    void ShowErrorMessage(const QString &Message);

    /****************************************************************************/
    /*!
     *  \brief To show calibration init message dialog
     *  \iparam Message = message text
     *  \iparam OkStatus = flag of ok
     */
    /****************************************************************************/
    void ShowCalibrationInitMessagetoMain(const QString &Message, bool OkStatus);

    /****************************************************************************/
    /*!
     *  \brief Enable calibration group
     *  \iparam Status = flag of status
     */
    /****************************************************************************/
    void EnableCalibrationGroup(bool Status);

private:
    int m_Result;   //!< store result.

};

} // end of namespace Core
#endif // CORE_CALIBRATIONHANDLER_H

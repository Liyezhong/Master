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
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
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

namespace Core {

/****************************************************************************/
/*!
 *  \brief This class implements device calibration functionality
 */
/****************************************************************************/
class CCalibrationHanlder : public QObject {
    Q_OBJECT

public:
    CCalibrationHanlder(Core::CServiceGUIConnector *p_ServiceGUIConnector, MainMenu::CMainWindow *p_MainWindow);
    ~CCalibrationHanlder();

    void LoadCalibrationGUIComponenets(bool TouchScreenCalibrationDone);

private:
    Core::CServiceGUIConnector              *mp_ServiceConnector;           //!< Service GUI connector object
    QEventLoop                              m_LoopCalibrationStart;         //!< Loop for blocking Calibration commands.
    MainMenu::CMainWindow                   *mp_MainWindow;                 //!< The main window of the application

    // Calibration
    MainMenu::CMenuGroup                    *mp_CalibrationGroup;           //!< Menu group containing calibration dialogs
    Calibration::CPressureSensor            *mp_PressureSensor;                    //!< Calibration for the oven lid.

    bool PerformCalibration(QString Title, QString GBox, QString Instr);

signals:

    /****************************************************************************/
    /**
     * \brief Signal emitted for OvenLid calibration
     */
    /****************************************************************************/
    void OvenLidCalibrationRequest();
    /****************************************************************************/
    /**
     * \brief Signal emitted for ovenlid init calibration
     */
    /****************************************************************************/
    void OvenLidInitCalibrationRequest();


public slots:

    /* Calibration Slots */
    void OnPressureSensorCalibration();

    /* Return Message Slots */
    void ShowMessage(const QString &Message);
    void ShowErrorMessage(const QString &Message);
    void ShowCalibrationInitMessagetoMain(const QString &Message, bool OkStatus);

    void EnableCalibrationGroup(bool Status);

};

} // end of namespace Core
#endif // CORE_CALIBRATIONHANDLER_H

/****************************************************************************/
/*! \file PressureSensorAutoCalibrate.h
 *
 *  \brief Declaration of Pressure Sensor Auto Calibrate.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2015-10-22
 *   $Author:  $ Sunny.Qu
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef DIAGNOSTICS_INITIALSYSTEM_PRESSURESENSORAUTOCALIBRATE_H
#define DIAGNOSTICS_INITIALSYSTEM_PRESSURESENSORAUTOCALIBRATE_H

#include "Diagnostics/Include/TestBase.h"
#include "MainMenu/Include/MainWindow.h"

namespace Diagnostics {

namespace InitialSystem {

/****************************************************************************/
/*!
*   \brief This class implements the functionality to initial AC voltage test
*/
/****************************************************************************/
class CPressureSensorAutoCalibrate : public Diagnostics::CTestBase
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam p_Parent = Parent widget
     */
    /****************************************************************************/
    CPressureSensorAutoCalibrate(QWidget *p_Parent=NULL);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CPressureSensorAutoCalibrate(void);

    /****************************************************************************/
    /*!
     *  \brief The function for test start run
     *  \return error code
     */
    /****************************************************************************/
    int Run(void);

protected:
    /****************************************************************************/
    /*!
     *  \brief To show/close waiting dialog
     *  \iparam ShowFlag = true to show and flase to close the dialog
     */
    /****************************************************************************/
    virtual void ShowWaitingMessage(bool ShowFlag=true);

    /****************************************************************************/
    /*!
     *  \brief To show the fail message dialog
     *  \iparam Error = the error type
     */
    /****************************************************************************/
    virtual void ShowFailMessage(int Error);

};

} // namespace InitialSystem

} // namespace Diagnostics

#endif // DIAGNOSTICS_INITIALSYSTEM_PRESSURESENSORAUTOCALIBRATE_H

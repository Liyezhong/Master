/****************************************************************************/
/*! \file Touchscreen.h
 *
 *  \brief Touchscreen class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-07-13
 *   $Author:  $ M.Scherer
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

#ifndef CALIBRATION_TOUCHSCREEN_H
#define CALIBRATION_TOUCHSCREEN_H

#include <QWidget>
#include <QEvent>
#include "MainMenu/Include/MainWindow.h"

namespace Calibration {

namespace Ui {
    class CTouchscreen;
}

/****************************************************************************/
/**
 * \brief This panel provides a button that starts the touchscreen
 *        calibration.
 */
/****************************************************************************/
class CTouchscreen : public QWidget
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam p_MainWindow = parent windows
     */
    /****************************************************************************/
    explicit CTouchscreen(MainMenu::CMainWindow *p_MainWindow = NULL);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    virtual ~CTouchscreen();

protected:

    /****************************************************************************/
    /*!
     *  \brief Event handler for change events
     *
     *  \iparam p_Event = Change event
     */
    /****************************************************************************/
    void changeEvent(QEvent *p_Event);

private:
    MainMenu::CMainWindow *mp_MainWindow;   //!< MainWindow Object
    Ui::CTouchscreen *mp_Ui;                //!< User interface

public slots:

    /****************************************************************************/
    /*!
     *  \brief Slot for touch screen calibration
     */
    /****************************************************************************/
    void TouchScreenCalibration();

signals:
    /*******************************************************************************/
    /*!
     *  \brief Signal emitted to shut down system
     */
    /*******************************************************************************/
    void ShutdownSystem(bool NeedUpdate=true);
};

} // end namespace Calibration

#endif // CALIBRATION_TOUCHSCREEN_H

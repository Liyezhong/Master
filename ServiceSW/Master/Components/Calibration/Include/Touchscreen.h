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
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
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
    explicit CTouchscreen(MainMenu::CMainWindow *p_MainWindow = NULL);
    virtual ~CTouchscreen();

protected:
    void changeEvent(QEvent *p_Event);

private:
    MainMenu::CMainWindow *mp_MainWindow;   //!< MainWindow Object
    Ui::CTouchscreen *mp_Ui;                //!< User interface

public slots:
    void TouchScreenCalibration();

};

} // end namespace Calibration

#endif // CALIBRATION_TOUCHSCREEN_H

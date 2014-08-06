
/****************************************************************************/
/*! \file Touchscreen.cpp
 *
 *  \brief Touchscreen implementation.
 *
 *  \b Description:
 *          This class implements widgets to perform touch screen calibration.
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

#include "Calibration/Include/Touchscreen.h"
#include "Main/Include/HimalayaServiceEventCodes.h"
#include "Global/Include/Utils.h"
#include "ui_Touchscreen.h"
#include <QDebug>
#include <QProcess>

namespace Calibration {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_MainWindow = Mainwindow object
 */
/****************************************************************************/
CTouchscreen::CTouchscreen(MainMenu::CMainWindow *p_MainWindow) : mp_MainWindow(p_MainWindow)
  , mp_Ui(new Ui::CTouchscreen)
{
    mp_Ui->setupUi(this);

    CONNECTSIGNALSLOTGUI(mp_Ui->startButton, clicked(), this, TouchScreenCalibration());
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CTouchscreen::~CTouchscreen()
{
    try {
        delete mp_Ui;
    }
    catch (...) {}
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CTouchscreen::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type()) {
    case QEvent::LanguageChange:
        mp_Ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/****************************************************************************/
/*!
 *  \brief Touch screen calibration
 */
/****************************************************************************/
void CTouchscreen::TouchScreenCalibration()
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_CALIBRATION_TOUCHSCREEN);

#if 0
    if (QFile::exists(qApp->applicationDirPath() + "/TouchscreenCalibration.sh") && QFile::exists(qApp->applicationDirPath() + "/himalaya_service")) {
        (void) system("chmod 755 TouchscreenCalibration.sh");
        emit ShutdownSystem();
        QStringList Params;

        if(mp_MainWindow->GetSaMUserMode() == QString("Service")) {
            Params.append("ts_Service");
        } else if(mp_MainWindow->GetSaMUserMode() == QString("Manufacturing")) {            
            Params.append("ts_Manufacturing");
        }
        (void) QProcess::startDetached("./TouchscreenCalibration.sh", Params);
    }
#else
    if(mp_MainWindow->GetSaMUserMode() == QString("Service")) {
        system("echo ts_Service >/tmp/.ts_calibrate");
    } else if(mp_MainWindow->GetSaMUserMode() == QString("Manufacturing")) {
        system("echo ts_Manufacturing > /tmp/.ts_calibrate");
    }

    emit ShutdownSystem(false);
#endif
}

} // end namespace Calibration

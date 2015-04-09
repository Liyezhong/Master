/****************************************************************************/
/*! \file Display.cpp
 *
 *  \brief Implementation file for class CDisplay.
 *
 *  \b Description:
 *          This class implements functions to view the status of Cover Hood.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-02-04
 *   $Author:  $ Soumya. D
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

#include "Diagnostics/Include/Display.h"
//#include "Main/Include/ColoradoServiceEventCodes.h"
#include "Global/Include/Utils.h"
#include "ui_Display.h"
#include <QDebug>

namespace Diagnostics {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CDisplay::CDisplay(QWidget *p_Parent) : QWidget(p_Parent),
    mp_Ui(new Ui::CDisplay)
{
    mp_Ui->setupUi(this);
    mp_Ui->instructionLabel->setText(QApplication::translate("Diagnostics::CDisplay",
                                     "Press Start button to start Basic Color Test.",
                                                             0, QApplication::UnicodeUTF8));
    // for service
    CONNECTSIGNALSLOTGUI(mp_Ui->startBtn, clicked(), this, OnBasicColorTest());
    // for factory
    CONNECTSIGNALSLOTGUI(mp_Ui->startBtn, clicked(), this, OnFactoryBasicColorTest());
    mp_ColorTestDialog = NULL;
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CDisplay::~CDisplay()
{
    try {
        if (mp_ColorTestDialog != NULL) {
            delete mp_ColorTestDialog;
        }
        delete mp_Ui;
    }
    catch (...) {
        // to please Lint
    }
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CDisplay::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type()) {
    case QEvent::LanguageChange:
        mp_Ui->retranslateUi(this);
        RetranslateUI();
        break;
    default:
        break;
    }
}

/****************************************************************************/
/*!
 *  \brief Slot is called for basic color test
 */
/****************************************************************************/
void CDisplay::OnBasicColorTest()
{
    emit BasicColorTest();
}

/****************************************************************************/
/*!
 *  \brief Slot is called for factory basic color test
 */
/****************************************************************************/
void CDisplay::OnFactoryBasicColorTest()
{
    QList<Service::ModuleTestCaseID> TestCaseList;
    Service::ModuleTestCaseID Id = Service::DISPLAY_BASIC_COLOR;
    TestCaseList.append(Id);
    emit BeginModuleTest(Service::DISPLAY, TestCaseList);
}

/****************************************************************************/
/*!
 *  \brief Basic color test
 */
/****************************************************************************/
void CDisplay::DoBasicColorTest()
{
    emit SetGUITabEnable(false);
    if (mp_ColorTestDialog) {
        delete mp_ColorTestDialog;
    }
    mp_ColorTestDialog = new CBasicColorTestDlg(this);
    (void)mp_ColorTestDialog->exec();

    emit SetGUITabEnable(true);
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CDisplay::RetranslateUI()
{
    mp_Ui->instructionLabel->setText(QApplication::translate("Diagnostics::CDisplay",
                                    "Press Start button to start Basic Color Test.", 0, QApplication::UnicodeUTF8));
}

} // end namespace Diagnostics

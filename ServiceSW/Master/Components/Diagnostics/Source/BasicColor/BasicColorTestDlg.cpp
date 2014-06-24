/****************************************************************************/
/*! \file BasicColorTestDlg.cpp
 *
 *  \brief  Basic color test dialog implementation
 *
 *  \b Description:
 *          This class implements dialog for color tests.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-04-18
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

#include "Diagnostics/Include/BasicColor/BasicColorTestDlg.h"
#include "Main/Include/HimalayaServiceEventCodes.h"
#include "Global/Include/Utils.h"
#include "ui_BasicColorTestDlg.h"
#include <QDebug>

namespace Diagnostics {

const int FIXED_DIALOG_WIDTH= 500;          ///< Basic color test dialog width
const int FIXED_DIALOG_HEIGHT = 350;        ///< Basic color test dialog height

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CBasicColorTestDlg::CBasicColorTestDlg(QWidget *p_Parent) : MainMenu::CDialogFrame(p_Parent)
  , mp_Ui(new Ui::CBasicColorTestDlg)
{
    m_ColorCount = 0;   
    mp_Ui->setupUi(GetContentFrame());
    SetDialogTitle(QApplication::translate("Diagnostics::CBasicColorTestDlg", "Basic Color Test",
                                           0, QApplication::UnicodeUTF8));
    mp_Ui->instructionLabel->setText(QApplication::translate("Diagnostics::CBasicColorTestDlg",
                             "Press Yes button if you see Black color.", 0, QApplication::UnicodeUTF8));
    mp_Ui->colorLabel->setFrameStyle((int)QFrame::WinPanel);
    mp_Ui->colorLabel->setStyleSheet(" QLabel { background-color:rgb(10,10,13); } ");
    resize(FIXED_DIALOG_WIDTH, FIXED_DIALOG_HEIGHT);
    m_ColorCount++;

    mp_MessageDlg = new MainMenu::CMessageDlg;
    mp_MessageDlg->setModal(true);

    CONNECTSIGNALSLOTGUI(mp_Ui->nextBtn, clicked(), this, BasicColorTest());
    CONNECTSIGNALSLOTGUI(mp_Ui->cancelBtn, clicked(), this, TestCancelButton());
    CONNECTSIGNALSLOTGUI(mp_Ui->abortBtn, clicked(), this, close());

}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CBasicColorTestDlg::~CBasicColorTestDlg()
{
    try {
        delete mp_Ui;
        delete mp_MessageDlg;
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
void CBasicColorTestDlg::changeEvent(QEvent *p_Event)
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
 *  \brief To perform basic color test
 */
/****************************************************************************/
void CBasicColorTestDlg::BasicColorTest()
{
    switch (m_ColorCount) {

    case 0:
        mp_Ui->instructionLabel->setText(QApplication::translate("MainMenu::CBasicColorTestDlg",
                                 "Press Yes button if you see Black color.", 0, QApplication::UnicodeUTF8));
        mp_Ui->colorLabel->setStyleSheet(" QLabel { background-color:rgb(10,10,13); } ");
        m_ColorCount++;
        break;

    case 1:
        mp_Ui->instructionLabel->setText(QApplication::translate("MainMenu::CBasicColorTestDlg",
                                 "Press Yes button if you see Blue color.", 0, QApplication::UnicodeUTF8));
        mp_Ui->colorLabel->setStyleSheet(" QLabel { background-color:rgb(21,72,137); } ");
        m_ColorCount++;
        break;

    case 2:
        mp_Ui->instructionLabel->setText(QApplication::translate("MainMenu::CBasicColorTestDlg",
                                 "Press Yes button if you see Grey color.", 0, QApplication::UnicodeUTF8));
        mp_Ui->colorLabel->setStyleSheet(" QLabel { background-color:rgb(145,150,154); } ");
        m_ColorCount++;
        break;

    case 3:
        mp_Ui->instructionLabel->setText(QApplication::translate("MainMenu::CBasicColorTestDlg",
                                 "Press Yes button if you see Green color.", 0, QApplication::UnicodeUTF8));
        mp_Ui->colorLabel->setStyleSheet(" QLabel { background-color:rgb(0,247,0); } ");
        m_ColorCount++;
        break;

    case 4:
        mp_Ui->instructionLabel->setText(QApplication::translate("MainMenu::CBasicColorTestDlg",
                                 "Press Yes button if you see Light Blue color.", 0, QApplication::UnicodeUTF8));
        mp_Ui->colorLabel->setStyleSheet(" QLabel { background-color:rgb(52,129,184); } ");
        m_ColorCount++;
        break;

    case 5:
        mp_Ui->instructionLabel->setText(QApplication::translate("MainMenu::CBasicColorTestDlg",
                                 "Press Yes button if you see Pink color.", 0, QApplication::UnicodeUTF8));
        mp_Ui->colorLabel->setStyleSheet(" QLabel { background-color:rgb(225,166,173); } ");
        m_ColorCount++;
        break;

    case 6:
        mp_Ui->instructionLabel->setText(QApplication::translate("MainMenu::CBasicColorTestDlg",
                                 "Press Yes button if you see Red color.", 0, QApplication::UnicodeUTF8));
        mp_Ui->colorLabel->setStyleSheet(" QLabel { background-color:rgb(193,18,28); } ");
        m_ColorCount++;
        break;

    case 7:
        mp_Ui->instructionLabel->setText(QApplication::translate("MainMenu::CBasicColorTestDlg",
                                 "Press Yes button if you see Yellow color.", 0, QApplication::UnicodeUTF8));
        mp_Ui->colorLabel->setStyleSheet(" QLabel { background-color:rgb(245,255,0); } ");
        m_ColorCount++;
        break;

    case 8:
        mp_Ui->instructionLabel->setText(QApplication::translate("MainMenu::CBasicColorTestDlg",
                                 "Press Yes button if you see White color.", 0, QApplication::UnicodeUTF8));
        mp_Ui->colorLabel->setStyleSheet(" QLabel { background-color:rgb(244,248,244); } ");
        m_ColorCount++;
        break;

    case 9:
        mp_Ui->colorLabel->hide();
        mp_Ui->instructionLabel->setText(QApplication::translate("MainMenu::CBasicColorTestDlg",
                                 "Basic Color Test Completed Successfully.", 0, QApplication::UnicodeUTF8));
        mp_Ui->nextBtn->setText(QApplication::translate("MainMenu::CBasicColorTestDlg", "Finish",
                                                        0, QApplication::UnicodeUTF8));
        mp_Ui->cancelBtn->hide();
        mp_Ui->groupBox->setGeometry(40, 40, 411, 150);

        //Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_DISPLAY_BASICCOLOR_TEST_SUCCESS);

        if(!connect(mp_Ui->nextBtn, SIGNAL(clicked()), this, SLOT(close()))) {
            qDebug() << "CBasicColorTestDlg: cannot connect 'clicked' signal";
        }
        m_ColorCount = 0;
        break;

    }
}

/****************************************************************************/
/*!
 *  \brief Calls when basic color test is cancelled
 */
/****************************************************************************/
void CBasicColorTestDlg::TestCancelButton()
{
    bool Flag = false;
    Flag = close();

    if (!Flag) {
        qDebug() << "CBasicColorTestDlg: cannot close the dialog";
    }
    mp_MessageDlg->SetTitle(QApplication::translate("MainMenu::CBasicColorTestDlg", "Basic Color Test",
                                                    0, QApplication::UnicodeUTF8));
    mp_MessageDlg->SetText(QApplication::translate("MainMenu::CBasicColorTestDlg", "Basic Color Test Failed",
                                                   0, QApplication::UnicodeUTF8));
    mp_MessageDlg->SetIcon(QMessageBox::Critical);
    mp_MessageDlg->SetButtonText(1, QApplication::translate("MainMenu::CBasicColorTestDlg", "Ok",
                                                            0, QApplication::UnicodeUTF8));
    mp_MessageDlg->HideButtons();
    mp_MessageDlg->Show();

    //Global::EventObject::Instance().RaiseEvent(EVENT_GUI_DIAGNOSTICS_DISPLAY_BASICCOLOR_TEST_SUCCESS);

    int Result = mp_MessageDlg->exec();

    if(Result) {
        m_ColorCount = 0;
    }
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CBasicColorTestDlg::RetranslateUI()
{
    SetDialogTitle(QApplication::translate("MainMenu::CBasicColorTestDlg", "Basic Color Test",
                                           0, QApplication::UnicodeUTF8));

    mp_Ui->instructionLabel->setText(QApplication::translate("MainMenu::CBasicColorTestDlg",
                                                             "Press Yes button if you see Black color.",
                                                             0, QApplication::UnicodeUTF8));

    mp_Ui->instructionLabel->setText(QApplication::translate("MainMenu::CBasicColorTestDlg",
                             "Press Yes button if you see Black color.", 0, QApplication::UnicodeUTF8));

    mp_Ui->instructionLabel->setText(QApplication::translate("MainMenu::CBasicColorTestDlg",
                             "Press Yes button if you see Blue color.", 0, QApplication::UnicodeUTF8));

    mp_Ui->instructionLabel->setText(QApplication::translate("MainMenu::CBasicColorTestDlg",
                             "Press Yes button if you see Grey color.", 0, QApplication::UnicodeUTF8));

    mp_Ui->instructionLabel->setText(QApplication::translate("MainMenu::CBasicColorTestDlg",
                             "Press Yes button if you see Green color.", 0, QApplication::UnicodeUTF8));

    mp_Ui->instructionLabel->setText(QApplication::translate("MainMenu::CBasicColorTestDlg",
                             "Press Yes button if you see Light Blue color.", 0, QApplication::UnicodeUTF8));

    mp_Ui->instructionLabel->setText(QApplication::translate("MainMenu::CBasicColorTestDlg",
                             "Press Yes button if you see Pink color.", 0, QApplication::UnicodeUTF8));

    mp_Ui->instructionLabel->setText(QApplication::translate("MainMenu::CBasicColorTestDlg",
                             "Press Yes button if you see Red color.", 0, QApplication::UnicodeUTF8));

    mp_Ui->instructionLabel->setText(QApplication::translate("MainMenu::CBasicColorTestDlg",
                             "Press Yes button if you see Yellow color.", 0, QApplication::UnicodeUTF8));

    mp_Ui->instructionLabel->setText(QApplication::translate("MainMenu::CBasicColorTestDlg",
                             "Press Yes button if you see White color.", 0, QApplication::UnicodeUTF8));

    mp_Ui->instructionLabel->setText(QApplication::translate("MainMenu::CBasicColorTestDlg",
                             "Basic Color Test Completed Successfully.", 0, QApplication::UnicodeUTF8));

    mp_Ui->nextBtn->setText(QApplication::translate("MainMenu::CBasicColorTestDlg",
                                                    "Finish", 0, QApplication::UnicodeUTF8));

    mp_MessageDlg->SetTitle(QApplication::translate("MainMenu::CBasicColorTestDlg", "Basic Color Test",
                                                    0, QApplication::UnicodeUTF8));

    mp_MessageDlg->SetText(QApplication::translate("MainMenu::CBasicColorTestDlg", "Basic Color Test Failed",
                                                   0, QApplication::UnicodeUTF8));

}

}   // end of namespace Diagnostics

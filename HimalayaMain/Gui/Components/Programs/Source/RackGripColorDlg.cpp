/****************************************************************************/
/*! \file RackGripColorDlg.cpp
 *
 *  \brief Implementation of file for class CRackGripColorDlg.
 *
 *  \b Description:
 *          This class implements a dailog for displaying and assigning of
 *          colors to programs.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-28
 *   $Author:  $ M.Scherer, C.Adaragunchi1234
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

#include "Global/Include/Utils.h"
#include "Global/Include/Exception.h"
#include "MainMenu/Include/MessageDlg.h"
#include "Programs/Include/RackGripColorDlg.h"
#include "ui_RackGripColorDlg.h"
#include <QDebug>
#include <QPainter>

namespace Programs {

//!< String list containing all rack colors available
QStringList CRackGripColorDlg::m_Colors = QStringList() << "yellow" << "pink" << "grey"
                                                        << "red" << "white" << "black"
                                                        << "darkblue" << "lightblue" << "green";

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Reference to Parent object
 *  \iparam p_MainWindow = Reference to MainWindow
 */
/****************************************************************************/
CRackGripColorDlg::CRackGripColorDlg(QWidget *p_Parent,MainMenu::CMainWindow *p_MainWindow) :
                                    MainMenu::CDialogFrame(p_Parent), mp_Ui(new Ui::CRackGripColorDlg),
                                    mp_MainWindow(p_MainWindow), mp_Program(NULL),
                                    m_ColorReplaced(false), m_ProcessRunning(false)
{
    mp_Ui->setupUi(GetContentFrame());
    SetDialogTitle(tr("Assign Rack Grip Color"));

    m_ButtonGroup.addButton(mp_Ui->pbYellow, 0);
    m_ButtonGroup.addButton(mp_Ui->pbPink, 1);
    m_ButtonGroup.addButton(mp_Ui->pbGrey, 2);
    m_ButtonGroup.addButton(mp_Ui->pbRed, 3);
    m_ButtonGroup.addButton(mp_Ui->pbWhite, 4);
    m_ButtonGroup.addButton(mp_Ui->pbBlack, 5);
    m_ButtonGroup.addButton(mp_Ui->pbDarkBlue, 6);
    m_ButtonGroup.addButton(mp_Ui->pbLightBlue, 7);
    m_ButtonGroup.addButton(mp_Ui->pbGreen, 8);

    CONNECTSIGNALSLOT(&m_ButtonGroup, buttonClicked(int), this, OnButtonGroup(int));
    CONNECTSIGNALSLOT(mp_Ui->btnCancel, clicked(), this, OnCancel());
    CONNECTSIGNALSLOT(mp_Ui->btnSave, clicked(), this, OnSave());
    CONNECTSIGNALSLOT(mp_MainWindow, ProcessStateChanged(), this, OnProcessStateChanged());
    mp_Program = new DataManager::CProgram();
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CRackGripColorDlg::~CRackGripColorDlg()
{    
    try {
        delete mp_Ui;
        delete mp_Program;
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
void CRackGripColorDlg::changeEvent(QEvent *p_Event)
{
    MainMenu::CDialogFrame::changeEvent(p_Event);
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
 *  \brief Saves the new rack color selection
 */
/****************************************************************************/
void CRackGripColorDlg::OnSave()
{
    m_Program[0]= *(mp_Program);
    if (m_ColorReplaced) {
        emit UpdateProgramColor(m_Program[1], true);
    }
    emit UpdateProgramColor(m_Program[0], false);
    accept();
}

/****************************************************************************/
/*!
 *  \brief Cancels the new rack color selection
 */
/****************************************************************************/
void CRackGripColorDlg::OnCancel()
{
    reject();
}

/****************************************************************************/
/*!
 *  \brief Updates the content of the color buttons
 */
/****************************************************************************/
void CRackGripColorDlg::UpdateButtons()
{
    QAbstractButton *p_Button;
    m_Programs.clear();

    for(qint32 i = 0; i < m_ProgramList.GetNumberOfPrograms(); i++) {
        DataManager::CProgram Program;
   /*  //vinay    if (m_ProgramList.GetProgram(i, Program) == true) {
            if (Program.GetColor() != "white") {
                m_Programs[Program.GetColor()] = Program;
            }
        }*/
    }
    foreach(p_Button, m_ButtonGroup.buttons()) {
        QPixmap Pixmap(45, 35);
        Pixmap.fill(Qt::transparent);
        QPainter Painter(&Pixmap);
        Painter.drawPixmap(0, 0, QPixmap(":/Rack/RackColor/RackColor_" +
                                         m_Colors[m_ButtonGroup.id(p_Button)] + ".png"));
        Painter.setPen(Qt::white);
    //vinay    Painter.drawText(12, 21, m_Programs[m_Colors[m_ButtonGroup.id(p_Button)]].GetShortName());
        p_Button->setIcon(QIcon(Pixmap));
    }
}

/****************************************************************************/
/*!
 *  \brief Changes the program color when one of the color buttons is clicked
 *
 *  \iparam Id = Number of the clicked color button
 */
/****************************************************************************/
void CRackGripColorDlg::OnButtonGroup(int Id)
{
    if (mp_Program == NULL) {
        return;
    }
   /* //vinay if (m_Programs[m_Colors[Id]].GetColor() == m_Colors[Id] && m_Programs[m_Colors[Id]].GetID()
            != mp_Program->GetID()) {
        MainMenu::CMessageDlg MessageDlg(this);
        MessageDlg.SetTitle(tr("Information Message"));
        MessageDlg.SetText(tr
                           ("Do you really want to replace the color of \"%1\"%2").arg(m_Programs[m_Colors[Id]].GetName()).arg("?"));
        MessageDlg.SetIcon(QMessageBox::Information);
        MessageDlg.SetButtonText(1, tr("Ok"));
        MessageDlg.SetButtonText(3, tr("Cancel"));
        MessageDlg.HideCenterButton();
        if (MessageDlg.exec() == (int)QDialog::Accepted) {
            m_Programs[m_Colors[Id]].SetColor("white");
            if (m_ProgramList.UpdateProgram(&m_Programs[m_Colors[Id]]) == false) {
                return;
            }
            m_Program[1] = m_Programs[m_Colors[Id]];
            m_ColorReplaced = true;
        }
        else {
            return;
        }
    }
    else {
        m_ColorReplaced = false;
    }
    if (mp_Program->GetColor() != m_Colors[Id]) {
        mp_Program->SetColor(m_Colors[Id]);
        if (m_ProgramList.UpdateProgram(mp_Program) == false) {
            return;
        }
    }
    UpdateButtons();*/
}

/****************************************************************************/
/*!
 *  \brief Sets the content of the dialog
 *
 *  \iparam p_ProgramList = Global program list
 *  \iparam p_Program = Program to edit
 *
 *  \return True if initialisation is successful else False
 */
/****************************************************************************/
bool CRackGripColorDlg::Init(DataManager::CDataProgramList *p_ProgramList, DataManager::CProgram *p_Program)
{
    m_ProgramList = *p_ProgramList;
    *mp_Program = *p_Program;
    m_ProgramClone = *p_Program;
    UpdateButtons();
    SetDialogTitle(tr("Assign Rack Grip Color"));
    return true;
}

/****************************************************************************/
/*!
 *  \brief This slot is called when Process state changes
 */
/****************************************************************************/
void CRackGripColorDlg::OnProcessStateChanged()
{
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    if(!m_ProcessRunning) {
        EnableColorButtons(true);
        mp_Ui->btnSave->setEnabled(true);
    }
    else {
        EnableColorButtons(false);
        mp_Ui->btnSave->setEnabled(false);
    }
}

/****************************************************************************/
/*!
 *  \brief Sets the text of Save button.
 *
 *  \iparam ButtonText - String containing the text for button.
 */
/****************************************************************************/
void CRackGripColorDlg::SetSaveButton(QString ButtonText)
{
    mp_Ui->btnSave->setText(ButtonText);
}

/****************************************************************************/
/*!
 *  \brief Enables or Disables Color buttons accordingly when the process
 *   state is changed.
 *
 *  \iparam Enable = Enable(true)/Disable(false)
 */
/****************************************************************************/
void CRackGripColorDlg::EnableColorButtons(bool Enable)
{
    mp_Ui->pbBlack->setEnabled(Enable);
    mp_Ui->pbDarkBlue->setEnabled(Enable);
    mp_Ui->pbGreen->setEnabled(Enable);
    mp_Ui->pbGrey->setEnabled(Enable);
    mp_Ui->pbLightBlue->setEnabled(Enable);
    mp_Ui->pbPink->setEnabled(Enable);
    mp_Ui->pbRed->setEnabled(Enable);
    mp_Ui->pbWhite->setEnabled(Enable);
    mp_Ui->pbYellow->setEnabled(Enable);
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CRackGripColorDlg::RetranslateUI()
{
   MainMenu::CDialogFrame::SetDialogTitle(QApplication::translate("Programs::CRackGripColorDlg",
   "Assign Rack Grip Color", 0, QApplication::UnicodeUTF8));
}

} // end namespace Programs

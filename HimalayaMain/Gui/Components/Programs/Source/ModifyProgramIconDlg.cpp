
/****************************************************************************/
/*! \file ModifyReagentGroupColorDlg.cpp
 *
 *  \brief ModifyReagentGroupColorDlg Implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-03-15
 *   $Author:  $ Swati Tiwari
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2012 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "Global/Include/Utils.h"
#include "Global/Include/Exception.h"
#include "Programs/Include/ModifyProgramIconDlg.h"
#include "ui_ModifyProgramIconDlg.h"
#include <QDebug>

namespace Programs {
/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CModifyProgramIconDlg::CModifyProgramIconDlg(QWidget *p_Parent, MainMenu::CMainWindow *p_MainWindow) :
    MainMenu::CDialogFrame(p_Parent), mp_Ui(new Ui::CModifyProgramIconDlg)
{
    mp_Ui->setupUi(GetContentFrame());
    m_ProcessRunning = false ;
    SetButtonGroup();

    QString strName("Icon");
    for (int i = 0; i < m_ButtonGroup.buttons().count(); i++)
    {
        strName.append(QString::number(i));
        m_IconName.push_back(strName);
        m_ButtonGroup.button(i)->setIcon(QIcon(":/HimalayaImages/Icons/Program/"+strName+".png"));
        m_ButtonGroup.button(i)->setIconSize(QSize(40, 30));
        strName = strName.left(4);
    }

    mp_Ui->btnCancel->setText("Cancel");
    mp_Ui->btnOk->setText("Ok");

    // Connecting Signal and Slots
    CONNECTSIGNALSLOT(&m_ButtonGroup, buttonClicked(int), this, OnIconGroup(int));
    CONNECTSIGNALSLOT(mp_Ui->btnCancel, clicked(), this, OnCancel());
    CONNECTSIGNALSLOT(mp_Ui->btnOk, clicked(), this, OnOk());
    m_MessageDlg.SetTitle(tr("Information Message"));
    m_MessageDlg.SetIcon(QMessageBox::Information);
    m_MessageDlg.SetButtonText(1, tr("Ok"));
    m_MessageDlg.HideButtons();
}

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \Function to create Map of PushButton and Creation of Button Group.
 */
/****************************************************************************/
void CModifyProgramIconDlg ::SetButtonGroup()
{
        m_ButtonGroup.addButton(mp_Ui->IconButton_1, 0);
        m_ButtonGroup.addButton(mp_Ui->IconButton_2, 1);
        m_ButtonGroup.addButton(mp_Ui->IconButton_3, 2);
        m_ButtonGroup.addButton(mp_Ui->IconButton_4, 3);
        m_ButtonGroup.addButton(mp_Ui->IconButton_5, 4);
        m_ButtonGroup.addButton(mp_Ui->IconButton_6, 5);

        m_ButtonGroup.addButton(mp_Ui->IconButton_7, 6);
        m_ButtonGroup.addButton(mp_Ui->IconButton_8, 7);
        m_ButtonGroup.addButton(mp_Ui->IconButton_9, 8);
        m_ButtonGroup.addButton(mp_Ui->IconButton_10, 9);
        m_ButtonGroup.addButton(mp_Ui->IconButton_11, 10);
        m_ButtonGroup.addButton(mp_Ui->IconButton_12, 11);
}
/****************************************************************************/
/*!
     *  \brief Destructor
     */
/****************************************************************************/
CModifyProgramIconDlg::~CModifyProgramIconDlg()
{
    delete mp_Ui;
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CModifyProgramIconDlg::changeEvent(QEvent *p_Event)
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
 *  \brief This slot is called when Cancel button in ModifyReagentDlg is pressed.
 */
/****************************************************************************/
void CModifyProgramIconDlg::OnCancel()
{
    emit CancelPressed();
    reject();
}

/****************************************************************************/
/*!
 *  \brief Saves the changes in the dialog
 */
/****************************************************************************/
void CModifyProgramIconDlg::OnOk()
{
    emit UpdateProgram(&m_Program);
    accept();
}
/****************************************************************************/
/*!
 *  \brief Selects the Icon in the Widget
 */
/****************************************************************************/
 void CModifyProgramIconDlg::OnIconGroup(int Id)
 {
     mp_Ui->btnOk->setEnabled(true);
     m_Program.SetIcon(m_IconName.value(Id));
 }

/****************************************************************************/
/*!
 *  \brief This slot is called when Process state changes
 */
/****************************************************************************/
void CModifyProgramIconDlg::OnProcessStateChanged()
{
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    if (!m_ProcessRunning) {
        //Edit Mode
        mp_Ui->btnOk->setEnabled(true);
        mp_Ui->btnCancel->setText(tr("Cancel"));
    }
    else {
        //View Mode
        mp_Ui->btnOk->setEnabled(false);
        mp_Ui->btnCancel->setText(tr("Close"));
    }
}

void CModifyProgramIconDlg::EnableAvailableIcon(DataManager::CDataProgramList* pProgramList)
{
    //enable all icons
    for (int i = 0; i < m_ButtonGroup.buttons().count(); i++)
    {
        m_ButtonGroup.button(i)->setEnabled(true);
        m_ButtonGroup.button(i)->setChecked(false);
    }

    //loop all used icons, diable it
    int count = pProgramList->GetNumberOfPrograms();
    for (int j = 0; j< count; j++)
    {
        if (m_IconName.contains(pProgramList->GetProgram(j)->GetIcon()))
        {
            int index = m_IconName.indexOf(pProgramList->GetProgram(j)->GetIcon());
            m_ButtonGroup.button(index)->setEnabled(false);
        }
    }
}

}

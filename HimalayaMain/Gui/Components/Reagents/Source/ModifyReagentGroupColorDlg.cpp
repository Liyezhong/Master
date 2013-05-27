/****************************************************************************/
/*! \file ModifyReagentGroupColorDlg.cpp
 *
 *  \brief ModifyReagentGroupColorDlg Implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-01-29
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
#include "Reagents/Include/ModifyReagentGroupColorDlg.h"
#include "ui_ModifyReagentGroupColorDlg.h"
#include <QDebug>

namespace Reagents {
/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CModifyReagentGroupColorDlg::CModifyReagentGroupColorDlg(QWidget *p_Parent, MainMenu::CMainWindow *p_MainWindow) :
    MainMenu::CDialogFrame(p_Parent), mp_Ui(new Ui::CModifyReagentGroupColorDlg)
{
    mp_Ui->setupUi(GetContentFrame());
    m_ProcessRunning = false ;
    SetButtonGroup();

    CONNECTSIGNALSLOT(&m_ButtonGroup, buttonClicked(int), this, OnButtonGroup(int));
    CONNECTSIGNALSLOT(mp_Ui->btnCancel, clicked(), this, OnCancel());
    CONNECTSIGNALSLOT(mp_Ui->btnOk, clicked(), this, OnOk());

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
void CModifyReagentGroupColorDlg ::SetButtonGroup()
{
        m_ButtonGroup.addButton(mp_Ui->pbwhite, 0);
        m_ButtonGroup.addButton(mp_Ui->pbyellow, 1);
        m_ButtonGroup.addButton(mp_Ui->pbpink, 2);
        m_ButtonGroup.addButton(mp_Ui->pbdarkred, 3);
        m_ButtonGroup.addButton(mp_Ui->pbgreen, 4);
        m_ButtonGroup.addButton(mp_Ui->pbgray, 5);
        m_ButtonGroup.addButton(mp_Ui->pbcarmine, 6);
        m_ButtonGroup.addButton(mp_Ui->pbred, 7);
        m_ButtonGroup.addButton(mp_Ui->pbdarkgreen, 8);
        m_ButtonGroup.addButton(mp_Ui->pbrackley,9);
        m_ButtonGroup.addButton(mp_Ui->pbblue,10);
        m_ButtonGroup.addButton(mp_Ui->pbdarkblue,11);

        m_QPushButtonMap[0] = mp_Ui->pbwhite;
        m_QPushButtonMap[1] = mp_Ui->pbyellow;
        m_QPushButtonMap[2] = mp_Ui->pbpink;
        m_QPushButtonMap[3] = mp_Ui->pbdarkred;
        m_QPushButtonMap[4] = mp_Ui->pbgreen;
        m_QPushButtonMap[5] = mp_Ui->pbgray;
        m_QPushButtonMap[6] = mp_Ui->pbcarmine;
        m_QPushButtonMap[7] = mp_Ui->pbred;
        m_QPushButtonMap[8] = mp_Ui->pbdarkgreen;
        m_QPushButtonMap[9] = mp_Ui->pbrackley;
        m_QPushButtonMap[10] = mp_Ui->pbblue;
        m_QPushButtonMap[11] = mp_Ui->pbdarkblue;

}

/****************************************************************************/
/*!
     *  \brief Destructor
     */
/****************************************************************************/
CModifyReagentGroupColorDlg::~CModifyReagentGroupColorDlg()
{
       delete mp_Ui;
}


/****************************************************************************/
/*!
     *  \Set pointer to CReagentGroupColorList
     */
/****************************************************************************/
void CModifyReagentGroupColorDlg:: SetReagentGroupColorListptr(DataManager::CReagentGroupColorList  *p_CReagentGroupColorList)
{
    mp_CReagentGroupColorList = p_CReagentGroupColorList;
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CModifyReagentGroupColorDlg::changeEvent(QEvent *p_Event)
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
void CModifyReagentGroupColorDlg::OnCancel()
{
    emit CancelPressed();
    reject();
}

/****************************************************************************/
/*!
 *  \brief Saves the changes in the dialog
 */
/****************************************************************************/
void CModifyReagentGroupColorDlg::OnOk()
{
    emit UpdateReagentGroup(m_ReagentGroup);
    accept();
}
/****************************************************************************/
/*!
 *  \brief This slot is called when Process state changes
 */
/****************************************************************************/
void CModifyReagentGroupColorDlg::OnProcessStateChanged()
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


/****************************************************************************/
/**
 * \brief Slot will be called when ButtonGroup is clicked
 */
/****************************************************************************/

void CModifyReagentGroupColorDlg:: OnButtonGroup(int Id)
{
     mp_Ui->btnOk->setEnabled(true);
     m_ReagentGroup.SetGroupColor(m_ColorNames.value(Id));
}

/****************************************************************************/
/**
 * \brief Slot will be called when ReagentGroupColorUpdated Signal emitted
 */
/****************************************************************************/
void CModifyReagentGroupColorDlg:: UpdateReagentGroupColor()
{
    m_ColorNames.clear();

    int Count = 0;
    QAbstractButton *p_Button;

      foreach(p_Button, m_ButtonGroup.buttons()) {
          QPixmap Pixmap;
         Pixmap.load(":/HimalayaImages/Color/Color_"+mp_CReagentGroupColorList->GetCReagentGroupColor(Count)->GetColorID()+ ".png");

         p_Button->setIconSize(QSize(50,50));
         p_Button->setIcon(QIcon(Pixmap));
         Count++;
      }

      for (qint32 ColorCount = 0; ColorCount < const_cast<DataManager::CReagentGroupColorList*>(mp_CReagentGroupColorList)->GetNumberOfReagentGroupColor(); ColorCount++) {
            QString ColorName = const_cast<DataManager::CReagentGroupColorList*>(mp_CReagentGroupColorList)->GetCReagentGroupColor(ColorCount)->GetColorValue();
             m_ColorNames << ColorName;
      }

}

/****************************************************************************/
/*!
 *  \brief Display the day run log files in the list
 *
 *  \iparam FileNames = List of file names
 */
/****************************************************************************/
void CModifyReagentGroupColorDlg::SetReagentGroupList(DataManager::CDataReagentGroupList &ReagentGroupList, const DataManager::CReagentGroup &ReagentGroup)
{   
    // copy the data to local
      m_ReagentGroup = ReagentGroup;
      mp_Ui->btnOk->setEnabled(false);

      for(int ButtonIndex = 0; ButtonIndex < m_ColorNames.count();ButtonIndex++ )
      {
        m_QPushButtonMap[ButtonIndex]->setEnabled(true);
        m_QPushButtonMap[ButtonIndex]->setChecked(false);
      }

       const DataManager::CReagentGroup * Temp_ReagentGroup;
       for(int Index = 0;Index<ReagentGroupList.GetNumberOfReagentGroups();Index++){
           Temp_ReagentGroup =ReagentGroupList.GetReagentGroup(Index);

           if(m_ColorNames.contains(Temp_ReagentGroup->GetGroupColor())){
               int index = m_ColorNames.indexOf(Temp_ReagentGroup->GetGroupColor());
               m_QPushButtonMap[index]->setEnabled(false);
           }
       }
}

}

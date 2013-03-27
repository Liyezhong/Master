/****************************************************************************/
/*! \file ModifyReagentDlg.cpp
 *
 *  \brief ModifyReagentDlg Implementation.
 *
 *   $Version: $ 0.2
 *   $Date:    $ 2011-08-22, 2013-01-17
 *   $Author:  $ M.Scherer, C.Adaragunchi, Swati Tiwari
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
#include "Reagents/Include/ModifyReagentDlg.h"
#include "ui_ModifyReagentDlg.h"
#include <QDebug>

namespace Reagents {

const QString REGEXP_NUMERIC_VALIDATOR  = "^[0-9]{1,5}$"; //!< Reg expression for the validator
/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 *  \iparam p_KeyBoard = Reference to Keyboard widget
 *  \iparam p_MainWindow = Reference to MainWindow
 *  \iparam p_DataConnector = Reference to DataConnector
 */
/****************************************************************************/
CModifyReagentDlg::CModifyReagentDlg(QWidget *p_Parent, KeyBoard::CKeyBoard *p_KeyBoard, MainMenu::CMainWindow *p_MainWindow, Core::CDataConnector *p_DataConnector) :
    MainMenu::CDialogFrame(p_Parent), mp_Ui(new Ui::CModifyReagentDlg), mp_MainWindow(p_MainWindow), mp_DataConnector(p_DataConnector)
{
    mp_Ui->setupUi(GetContentFrame());
    m_ReagentNameBtnClicked = false;
    m_CassetteValueBtnClicked = false;
    m_ProcessRunning = false ;
    mp_NewReagent = NULL;
    mp_ReagentGroupList = NULL;
    m_SelectionFlag = false;

    mp_TableWidget = new MainMenu::CBaseTable;
    mp_TableWidget->setModel(&m_ReagentGroupModel);

    //m_ReagentModel.SetReagentList(mp_ReagentGroupList, 2);

    mp_Ui->scrollTable->SetContent(mp_TableWidget);

    mp_TableWidget->SetVisibleRows(8);
    m_ReagentGroupModel.SetVisibleRowCount(8);
    mp_TableWidget->horizontalHeader()->show();
    ResizeHorizontalSection();

    CONNECTSIGNALSLOT(mp_TableWidget, pressed(QModelIndex), this, SelectionChanged(QModelIndex));
    CONNECTSIGNALSLOT(mp_Ui->btnCancel, clicked(), this, OnCancel());
    CONNECTSIGNALSLOT(mp_Ui->btnOk, clicked(), this, OnOk());
    CONNECTSIGNALSLOT(mp_Ui->buttonReagentName, clicked(), this, OnEditName());
    CONNECTSIGNALSLOT(mp_Ui->buttonValue, clicked(), this, OnEditCassetteValue());
    //CONNECTSIGNALSLOT(mp_MainWindow, ProcessStateChanged(), this, OnProcessStateChanged());
    mp_KeyBoardWidget = p_KeyBoard;

    // Init Message dialog
    m_MessageDlg.SetTitle(tr("Information Message"));
    m_MessageDlg.SetIcon(QMessageBox::Information);
    m_MessageDlg.SetButtonText(1, tr("Ok"));
    m_MessageDlg.HideButtons();
    m_RMSOption = Global::RMS_OFF;
}

/****************************************************************************/
/*!
 *  \brief Resizes the columns in the reagents table
 */
/****************************************************************************/
void CModifyReagentDlg::ResizeHorizontalSection()
{
    if (mp_TableWidget == NULL)
    {
        return;
    }
    mp_TableWidget->horizontalHeader()->resizeSection(0, 170);
    mp_TableWidget->horizontalHeader()->resizeSection(1, 100);
}

/****************************************************************************/
/*!
 *  \brief Called when the selection of the table is changed
 *
 *  \iparam Index = Currently selected cell
 */
/****************************************************************************/
void CModifyReagentDlg::SelectionChanged(QModelIndex Index)
{
     if (m_ButtonType == Reagents::NEW_BTN_CLICKED)
        m_SelectionFlag = true;

    QString Id = m_ReagentGroupModel.data(Index, (int)Qt::UserRole).toString();
    m_Reagent.SetGroupID(Id);
}


/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CModifyReagentDlg::~CModifyReagentDlg()
{
    try {        
        delete mp_Ui;
        delete mp_NewReagent;
    }
    catch (...) {
        // to please Lint.
    }
}

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/
void CModifyReagentDlg::changeEvent(QEvent *p_Event)
{
    QDialog::changeEvent(p_Event);
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
 *  \brief Sets the content of the dialog
 *
 *  \iparam p_Reagent = Reference of the reagent to be edited
 */
/****************************************************************************/
void CModifyReagentDlg::InitDialog(DataManager::CReagent const *p_Reagent,
                                   DataManager::CDataReagentGroupList const *p_ReagentGroupList, Global::RMSOptions_t Option)
{
    if (p_ReagentGroupList != NULL) {
        mp_ReagentGroupList = const_cast<DataManager::CDataReagentGroupList*>(p_ReagentGroupList);
        m_ReagentGroupModel.SetReagentGroupList(mp_ReagentGroupList, 2);
        ResizeHorizontalSection();
        // always select the first one
        //mp_TableWidget->selectRow(0);
    }
    m_RMSOption = Option;
    if (p_Reagent != NULL) {
        m_Reagent = *p_Reagent;

        // Check if Edit button was clicked in ReagentWidget
        if (m_ButtonType == Reagents::EDIT_BTN_CLICKED) {

            mp_Ui->buttonReagentName->setText(m_Reagent.GetReagentName());
            mp_Ui->buttonValue->setVisible(true);
            mp_Ui->labelRMSStaticName->setVisible(true);
            switch (Option) {
                case Global::RMS_CASSETTES:
                    mp_Ui->labelRMSStaticName->setText("Cassettes until change");
                    mp_Ui->buttonValue->setText(QString::number(m_Reagent.GetMaxCassettes()));
                    break;
                case Global::RMS_CYCLES:
                    mp_Ui->labelRMSStaticName->setText("Cycles until change");
                    mp_Ui->buttonValue->setText(QString::number(m_Reagent.GetMaxCycles()));
                    break;
                case Global::RMS_DAYS:
                    mp_Ui->labelRMSStaticName->setText("Days until change");
                    mp_Ui->buttonValue->setText(QString::number(m_Reagent.GetMaxDays()));
                    break;
                case Global::RMS_OFF:
                    mp_Ui->buttonValue->setVisible(false);
                    mp_Ui->labelRMSStaticName->setVisible(false);
                    break;
            }
            mp_TableWidget->selectRow(mp_ReagentGroupList->GetReagentGroupIndex(m_Reagent.GetGroupID()));
        }

    }
    else {
        NewReagent();
    }    

}

/****************************************************************************/
/*!
 *  \brief Sets the content of the dialog for Selected/Copied Reagent
 *  \iparam Reagent = Reagent object
 */
/****************************************************************************/
void CModifyReagentDlg::SetReagentContents(DataManager::CReagent &Reagent)
{

}

/****************************************************************************/
/*!
 *  \brief Sets the content of the dialog for New Reagent
 *
 */
/****************************************************************************/
void CModifyReagentDlg::NewReagent()
{
    mp_Ui->buttonReagentName->setEnabled(true);    
    mp_Ui->buttonReagentName->setText("--");
    mp_Ui->buttonValue->setEnabled(true);
    mp_Ui->buttonValue->setText("--");
   // mp_TableWidget->selectRow(0);

}

/****************************************************************************/
/*!
 *  \brief This function is called when Ok button on keyboard is clicked
 */
/****************************************************************************/
void CModifyReagentDlg::Update()
{
    OnOkClicked();
}

/****************************************************************************/
/*!
 *  \brief This function is called when ESC button on keyboard is clicked
 */
/****************************************************************************/
void CModifyReagentDlg::UpdateOnESC()
{
    m_ReagentNameBtnClicked = false;
}

/****************************************************************************/
/*!
 *  \brief Cancels the changes in the dialog
 */
/****************************************************************************/
void CModifyReagentDlg::OnCancel()
{
    emit CancelPressed();
    mp_TableWidget->reset();
    reject();
}

/****************************************************************************/
/*!
 *  \brief Saves the changes in the dialog
 */
/****************************************************************************/
void CModifyReagentDlg::OnOk()
{
    // Clone the DataManager ReagentList
     m_ReagentCloneList = *(mp_DataConnector->ReagentList);

    if (mp_Ui->buttonReagentName->text()=="--") {
        m_MessageDlg.SetText(tr("Please enter valid Reagent Long Name"));
        m_MessageDlg.SetButtonText(1, tr("Ok"));
        (void) m_MessageDlg.exec();
        return;
    }
    if (mp_Ui->buttonValue->text()=="--") {
        m_MessageDlg.SetText(tr("Please enter valid data"));
        m_MessageDlg.SetButtonText(1, tr("Ok"));
        (void) m_MessageDlg.exec();
        return;
    }

    // Check if Edit button was clicked in ReagentWidget
    if (m_ButtonType == Reagents::EDIT_BTN_CLICKED) {
        m_SelectionFlag = false;
        m_Reagent.SetReagentName(mp_Ui->buttonReagentName->text());
        switch (m_RMSOption) {
            case Global::RMS_CASSETTES:
                m_Reagent.SetMaxCassettes(mp_Ui->buttonValue->text().toInt());
                break;
            case Global::RMS_CYCLES:
                m_Reagent.SetMaxCycles(mp_Ui->buttonValue->text().toInt());
                break;
            case Global::RMS_DAYS:
			    m_Reagent.SetMaxDays(mp_Ui->buttonValue->text().toInt());
				break;
		}

        if (m_ReagentCloneList.UpdateReagent(&m_Reagent) == true) {
            emit UpdateReagent(m_Reagent);
        }
        else {
            ListOfErrors_t &ErrorList = m_ReagentCloneList.GetErrorList();
            QString ErrorString;
            DataManager::Helper::ErrorIDToString(ErrorList, ErrorString);
            m_MessageDlg.SetText(ErrorString);
            m_MessageDlg.SetButtonText(1, tr("Ok"));
            m_MessageDlg.HideButtons();
            (void) m_MessageDlg.exec();
        }
    }
    // Else New/Copy button is clicked in ReagentWidget
    else {
        // GetNextFreeReagentId for New/Copied Reagent.
        if(m_SelectionFlag != true)
        {
            m_MessageDlg.SetText(tr("Please Select ReagentGroup"));
            m_MessageDlg.SetButtonText(1, tr("Ok"));
            (void) m_MessageDlg.exec();
            return;
        }
        else
            m_SelectionFlag = false;

        QString Id = m_ReagentCloneList.GetNextFreeReagentID(true);
        m_Reagent.SetReagentID(Id);
        m_Reagent.SetReagentName(mp_Ui->buttonReagentName->text());
        m_Reagent.SetMaxCassettes(mp_Ui->buttonValue->text().toInt());

        if (m_ReagentCloneList.AddReagent(&m_Reagent)== true) {
            emit AddReagent(m_Reagent);
        }
        else {
            if (m_ButtonType == Reagents::NEW_BTN_CLICKED) {
                m_MessageDlg.SetText("Reagent add failed");
            }
            else {
                m_MessageDlg.SetText("Reagent copy failed");
            }
            m_MessageDlg.SetButtonText(1, tr("Ok"));
            m_MessageDlg.HideButtons();
           (void) m_MessageDlg.exec();
        }
    }
    accept();
}

/****************************************************************************/
/*!
 *  \brief Shows the on screen keyboard to edit the program name
 */
/****************************************************************************/
void CModifyReagentDlg::OnEditName()
{
    mp_KeyBoardWidget->Attach(this);
    m_ReagentNameBtnClicked = true;
    mp_KeyBoardWidget->SetKeyBoardDialogTitle(tr("Enter Reagent Name"));

    if (mp_Ui->buttonReagentName->text() != "--") {

        mp_KeyBoardWidget->SetLineEditContent(mp_Ui->buttonReagentName->text());
    }

    m_ValidationType = KeyBoard::VALIDATION_1;
    mp_KeyBoardWidget->SetValidationType(m_ValidationType);
    mp_KeyBoardWidget->SetPasswordMode(false);
    mp_KeyBoardWidget->SetMaxCharLength(32);
    mp_KeyBoardWidget->SetMinCharLength(2);
    mp_KeyBoardWidget->show();

}

/****************************************************************************/
/*!
 *  \brief Shows the on screen keyboard to edit the program name
 */
/****************************************************************************/
void CModifyReagentDlg::OnEditCassetteValue()
{
    mp_KeyBoardWidget->Attach(this);
    m_CassetteValueBtnClicked = true;
    switch (m_RMSOption) {
        case Global::RMS_CASSETTES:
            mp_KeyBoardWidget->SetKeyBoardDialogTitle(tr("Enter Cassette Value"));
            break;
        case Global::RMS_CYCLES:
            mp_KeyBoardWidget->SetKeyBoardDialogTitle(tr("Enter Cycle Value"));
            break;
        case Global::RMS_DAYS:
            mp_KeyBoardWidget->SetKeyBoardDialogTitle(tr("Enter Day Value"));
            break;
    }

    if (mp_Ui->buttonValue->text() != "--") {
        mp_KeyBoardWidget->SetLineEditContent(mp_Ui->buttonValue->text());
    }
    m_ValidationType = KeyBoard::VALIDATION_1;
    mp_KeyBoardWidget->SetValidationType(m_ValidationType);
    mp_KeyBoardWidget->SetPasswordMode(false);
    mp_KeyBoardWidget->SetMaxCharLength(32);
    mp_KeyBoardWidget->SetMinCharLength(2);
	mp_KeyBoardWidget->SetLineEditValidatorExpression(REGEXP_NUMERIC_VALIDATOR);
    mp_KeyBoardWidget->show();

}

/****************************************************************************/
/*!
 *  \brief This slot is called when OK button on Keyboard is clicked.
 */
/****************************************************************************/
void CModifyReagentDlg::OnOkClicked()
{
    QString LineEditString;
    mp_KeyBoardWidget->hide();
    if (m_ReagentNameBtnClicked){
        m_ReagentNameBtnClicked = false;
        LineEditString = mp_KeyBoardWidget->GetLineEditString();
        mp_Ui->buttonReagentName->setText(LineEditString);
    }
    else if (m_CassetteValueBtnClicked){
        m_CassetteValueBtnClicked = false;
        LineEditString = mp_KeyBoardWidget->GetLineEditString();
        mp_Ui->buttonValue->setText(LineEditString);
    }

    mp_KeyBoardWidget->Detach();
}


/****************************************************************************/
/*!
 *  \brief This slot is called when Process Running state changes
 */
/****************************************************************************/
void CModifyReagentDlg::OnProcessStateChanged()
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
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CModifyReagentDlg::RetranslateUI()
{
    mp_KeyBoardWidget->SetKeyBoardDialogTitle(QApplication::translate("Reagents::CModifyReagentDlg",
                                              "Enter Reagent Name", 0, QApplication::UnicodeUTF8));
}

} // end namespace Reagents

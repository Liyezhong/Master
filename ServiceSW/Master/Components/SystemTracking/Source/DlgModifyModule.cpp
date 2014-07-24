/****************************************************************************/
/*! \file DlgModifyModule.cpp
 *
 *  \brief Implementation of Modify module dialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-06-06
 *   $Author:  $ R.Wu
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

#include "SystemTracking/Include/DlgModifyModule.h"

#include "Global/Include/Utils.h"

#include "ui_DlgModifyModule.h"

namespace SystemTracking {

CDlgModifyModule::CDlgModifyModule(ServiceDataManager::CModule &Module,
                                   bool HasBoard,
                                   QWidget *p_Parent)
    : MainMenu::CDialogFrame(p_Parent)
    , mp_Module(&Module)
    , mp_Ui(new Ui::CDlgModifyModule)
{
    mp_Ui->setupUi(GetContentFrame());

    if (HasBoard)
    {
        mp_Ui->pushAutoDetect->show();
    }
    else
    {
        mp_Ui->pushAutoDetect->hide();
    }

    mp_MessageDlg = new MainMenu::CMessageDlg(this);
    mp_DayWheel = new MainMenu::CScrollWheel();
    mp_MonthWheel = new MainMenu::CScrollWheel();
    mp_YearWheel = new MainMenu::CScrollWheel();

    // Day
    for (int i = 1; i <= 31; i++) {
        mp_DayWheel->AddItem(QString("%1").arg(i, 2, 10, QChar('0')), i);
    }

    // Month
    for (int i = 1; i <= 12; i++) {
        mp_MonthWheel->AddItem(QString("%1").arg(i, 2, 10, QChar('0')), i);
    }

    // Year
    for (int i = 0; i <= 99; i++) {
        mp_YearWheel->AddItem(QString("%1").arg(i, 2, 10, QChar('0')), 2000 + i);
    }

    mp_Ui->widget->SetTitle(tr("Date"));
    mp_Ui->widget->Init(3);
    mp_Ui->widget->AddScrollWheel(mp_DayWheel, 0);
    mp_Ui->widget->SetSubtitle(tr("Day"), 0);
    mp_Ui->widget->AddSeparator(MainMenu::CWheelPanel::FULLSTOP, 0);
    mp_Ui->widget->AddScrollWheel(mp_MonthWheel, 1);
    mp_Ui->widget->SetSubtitle(tr("Month"), 1);
    mp_Ui->widget->AddSeparator(MainMenu::CWheelPanel::FULLSTOP, 1);
    mp_Ui->widget->AddScrollWheel(mp_YearWheel, 2);
    mp_Ui->widget->SetSubtitle(tr("Year"), 2);

    UpdateGUI();

    mp_KeyBoardWidget =
           new KeyBoard::CKeyBoard(KeyBoard::SIZE_1, KeyBoard::QWERTY_KEYBOARD);

    ConnectKeyBoardSignalSlots();

    (void)connect(mp_Ui->pushSerialNumber,
            SIGNAL(clicked()),
            this,
            SLOT(OnEditSerialNumber()));
    (void)connect(mp_Ui->pushAutoDetect,
            SIGNAL(clicked()),
            this,
            SLOT(OnAutoDetect()));
    (void)connect(mp_Ui->pushSave, SIGNAL(clicked()), this, SLOT(OnSave()));
    (void)connect(mp_Ui->pushClose, SIGNAL(clicked()), this, SLOT(close()));
}

CDlgModifyModule::~CDlgModifyModule(void)
{
    try {
        delete mp_KeyBoardWidget;
        delete mp_YearWheel;
        delete mp_MonthWheel;
        delete mp_DayWheel;
        delete mp_MessageDlg;
        delete mp_Ui;
    }
    catch (...) {
        // to please Lint
    }
}

void CDlgModifyModule::OnEditSerialNumber(void)
{
    mp_KeyBoardWidget->setModal(true);
    mp_KeyBoardWidget->SetKeyBoardDialogTitle(tr("ENTER SERIAL NUMBER"));
    mp_KeyBoardWidget->SetPasswordMode(false);
    mp_KeyBoardWidget->SetValidation(true);
    mp_KeyBoardWidget->SetLineEditValidatorExpression("^[0-9]{1,4}$"); //yuan@note: for SN;
    mp_KeyBoardWidget->DisplayNumericKeyBoard();
    mp_KeyBoardWidget->show();
}

void CDlgModifyModule::UpdateGUI(void)
{
    //QDate Date = QDate::fromString(mp_Module->GetDateOfProduction(), Qt::ISODate);
    QDate Date = QDate::currentDate();
    this->UpdateGUI(mp_Module->GetSerialNumber(), Date);

    this->SetDialogTitle(mp_Module->GetModuleName());
}

void CDlgModifyModule::UpdateGUI(const QString &SerialNumber,
                                 const QDate &DateOfProduction)
{
    mp_Ui->pushSerialNumber->setText(SerialNumber);
    if (SerialNumber == "N/A") {
        mp_Ui->pushSerialNumber->setEnabled(false);
    }

    mp_DayWheel->SetCurrentData(DateOfProduction.day());
    mp_MonthWheel->SetCurrentData(DateOfProduction.month());
    mp_YearWheel->SetCurrentData(DateOfProduction.year());
}

/****************************************************************************/
/*!
 *  \brief This slot is called when Save button is clicked.
 *  \return Returns zero if module does not exists.
 */
/****************************************************************************/
void CDlgModifyModule::OnSave(void)
{
    QDate Date(mp_YearWheel->GetCurrentData().toInt(),
               mp_MonthWheel->GetCurrentData().toInt(),
               mp_DayWheel->GetCurrentData().toInt());

    if (!Date.isValid())
    {
        mp_MessageDlg->SetTitle(tr("Date of Exchange"));
        mp_MessageDlg->SetButtonText(1, tr("OK"));
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetText(tr("Date is invalid."
                                  "Please select a valid date."));
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        mp_MessageDlg->show();

        return;
    }

    QDate Now = QDate::currentDate();
    if (Date > Now)
    {
        mp_MessageDlg->SetTitle(tr("Date of Exchange"));
        mp_MessageDlg->SetButtonText(1, tr("OK"));
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetText(tr("Date of Exchange shall not be later "
                                  "than %1").arg(Now.toString(Qt::ISODate)));
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        mp_MessageDlg->show();

        return;
    }

    QString SerialNumber = mp_Ui->pushSerialNumber->text();
    if(SerialNumber.isEmpty())
    {
        mp_MessageDlg->SetTitle(tr("Serial Number"));
        mp_MessageDlg->SetButtonText(1, tr("OK"));
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetText(tr("SerialNumber is Empty."
                                  "Please enter the serial number."));
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        mp_MessageDlg->show();

        return;
    }

    mp_Module->SetSerialNumber(SerialNumber);
    mp_Module->SetDateOfProduction(Date.toString(Qt::ISODate));

    emit UpdateModule(*mp_Module);

    (void)this->close();
}

/****************************************************************************/
/*!
 *  \brief Slot for removing the module
 */
/****************************************************************************/
void CDlgModifyModule::OnAutoDetect(void)
{
    emit AutoDetect(*mp_Module);
}

/****************************************************************************/
/*!
 *  \brief This slot is called when OK button on Keyboard is clicked.
 *
 *  \iparam EnteredString = Entered String from keyboard.
 */
/****************************************************************************/
void CDlgModifyModule::OnOkClicked(QString EnteredString)
{
    mp_KeyBoardWidget->hide();
    QString sn = mp_Ui->pushSerialNumber->text();
    int Index = sn.indexOf('/');
    if (Index != -1) {
        (void)sn.remove(Index + 1, sn.length() - Index);
        sn += EnteredString;
    }
    mp_Ui->pushSerialNumber->setText(sn.simplified());
}

/****************************************************************************/
/*!
 *  \brief This function is called when ESC button on keyboard is clicked
 */
/****************************************************************************/
void CDlgModifyModule::OnESCClicked(void)
{
    mp_KeyBoardWidget->hide();
}

/****************************************************************************/
/*!
 *  \brief Connects signals and slots of keyboard.
 */
/****************************************************************************/
void CDlgModifyModule::ConnectKeyBoardSignalSlots(void)
{
    // Connect signals and slots to keyboard.
    CONNECTSIGNALSLOT(mp_KeyBoardWidget, OkButtonClicked(QString), this, OnOkClicked(QString));
    CONNECTSIGNALSLOT(mp_KeyBoardWidget, EscButtonClicked(), this, OnESCClicked());
}

/****************************************************************************/
/*!
 *  \brief Disconnects signals and slots of keyboard.
 */
/****************************************************************************/
void CDlgModifyModule::DisconnectKeyBoardSignalSlots(void)
{
    // Disconnect signals and slots connected to keyboard.
    (void) disconnect(mp_KeyBoardWidget, SIGNAL(OkButtonClicked(QString)), this, SLOT(OnOkClicked(QString)));
    (void) disconnect(mp_KeyBoardWidget, SIGNAL(EscButtonClicked()), this, SLOT(OnESCClicked()));
}

} // end namespace SystemTracking



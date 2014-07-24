/****************************************************************************/
/*! \file DlgModifySubModule.cpp
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

#include "SystemTracking/Include/DlgModifySubModule.h"

#include "Global/Include/Utils.h"

#include "ui_DlgModifyModule.h"

namespace SystemTracking {

CDlgModifySubModule::CDlgModifySubModule(ServiceDataManager::CSubModule &SubModule,
                                   bool HasBoard,
                                   QWidget *p_Parent)
    : MainMenu::CDialogFrame(p_Parent)
    , mp_SubModule(&SubModule)
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

    if (mp_SubModule->GetSubModuleName() == "E Box") {
        mp_Ui->pushSerialNumber->setEnabled(true);
    }
    else {
        mp_Ui->pushSerialNumber->setEnabled(false);
    }
    mp_Ui->pushSerialNumber->setText("N/A");

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

CDlgModifySubModule::~CDlgModifySubModule()
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

void CDlgModifySubModule::OnEditSerialNumber(void)
{  
    mp_KeyBoardWidget->setModal(true);
    mp_KeyBoardWidget->SetKeyBoardDialogTitle(tr("ENTER SERIAL NUMBER"));
    mp_KeyBoardWidget->SetPasswordMode(false);
    mp_KeyBoardWidget->SetValidation(true);
    mp_KeyBoardWidget->SetLineEditValidatorExpression("^[0-9]{1,4}$"); //yuan@note: for SN;
    mp_KeyBoardWidget->DisplayNumericKeyBoard();
    mp_KeyBoardWidget->show();
}

void CDlgModifySubModule::UpdateGUI(void)
{
    QString ModuleName = mp_SubModule->GetSubModuleName();

    const QString SerialNumberName("SerialNumber");
    ServiceDataManager::Parameter_t *ParamSN =
            mp_SubModule->GetParameterInfo(SerialNumberName);
    if (0 == ParamSN)
    {
        qDebug() << "CDlgModifySubModule::UpdateGUI : "
                    "no parameter Serial Number in sub-module: "
                    << ModuleName;
        return;
    }

    const QString DateOfProductionName("DateOfExchange");
    ServiceDataManager::Parameter_t *ParamDOP =
            mp_SubModule->GetParameterInfo(DateOfProductionName);
    if (0 == ParamDOP)
    {
        qDebug() << "CDlgModifySubModule::UpdateGUI : "
                    "no parameter Date of Production in sub-module: "
                    << ModuleName;
        return;
    }

    //QDate Date =
            //QDate::fromString(ParamDOP->ParameterValue, Qt::ISODate);
    QDate Date = QDate::currentDate();

    this->UpdateGUI(ParamSN->ParameterValue, Date);

    this->SetDialogTitle(ModuleName);
}

void CDlgModifySubModule::UpdateGUI(const QString &SerialNumber,
                                    const QDate &DateOfProduction)
{
    if (SerialNumber != "N/A") {
        mp_Ui->pushSerialNumber->setText("N/A");
    }

    if (mp_SubModule && mp_SubModule->GetSubModuleName() == "E Box") {
        mp_Ui->pushSerialNumber->setText(SerialNumber);
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
void CDlgModifySubModule::OnSave(void)
{
    QDate Date(mp_YearWheel->GetCurrentData().toInt(),
               mp_MonthWheel->GetCurrentData().toInt(),
               mp_DayWheel->GetCurrentData().toInt());

    if (!Date.isValid())
    {
        mp_MessageDlg->SetTitle(tr("Date of Production"));
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
        mp_MessageDlg->SetTitle(tr("Date of Production"));
        mp_MessageDlg->SetButtonText(1, tr("OK"));
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetText(tr("Date of Production shall not be later "
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

    const QString SerialNumberName("SerialNumber");
    const QString DateOfProductionName("DateOfExchange");

    if (!mp_SubModule) {
        qDebug()<<"CDlgModifySubModule::OnSave():invalid sub module";
        return;
    }
    (void)mp_SubModule->UpdateParameterInfo(SerialNumberName, SerialNumber);
    (void)mp_SubModule->UpdateParameterInfo(DateOfProductionName,
                                      Date.toString(Qt::ISODate));

    emit UpdateSubModule(*mp_SubModule);

    (void)this->close();
}

/****************************************************************************/
/*!
 *  \brief Slot for removing the module
 */
/****************************************************************************/
void CDlgModifySubModule::OnAutoDetect(void)
{
    emit AutoDetect(*mp_SubModule);

    QString serialNumber = mp_SubModule->GetParameterInfo("SerialNumber")->ParameterValue;
    mp_Ui->pushSerialNumber->setText(serialNumber);

}

/****************************************************************************/
/*!
 *  \brief This slot is called when OK button on Keyboard is clicked.
 *
 *  \iparam EnteredString = Entered String from keyboard.
 */
/****************************************************************************/
void CDlgModifySubModule::OnOkClicked(QString& EnteredString)
{
    mp_KeyBoardWidget->hide();
    QString SerialNumber = mp_Ui->pushSerialNumber->text();
    int Index = SerialNumber.indexOf('/');
    if (Index != -1) {
        (void)SerialNumber.remove(Index + 1, SerialNumber.length() - Index);
        SerialNumber += EnteredString;
    }
    mp_Ui->pushSerialNumber->setText(SerialNumber.simplified());
}

/****************************************************************************/
/*!
 *  \brief This function is called when ESC button on keyboard is clicked
 */
/****************************************************************************/
void CDlgModifySubModule::OnESCClicked()
{
    mp_KeyBoardWidget->hide();
}

/****************************************************************************/
/*!
 *  \brief Connects signals and slots of keyboard.
 */
/****************************************************************************/
void CDlgModifySubModule::ConnectKeyBoardSignalSlots(void)
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
void CDlgModifySubModule::DisconnectKeyBoardSignalSlots(void)
{
    // Disconnect signals and slots connected to keyboard.
    (void) disconnect(mp_KeyBoardWidget, SIGNAL(OkButtonClicked(QString)), this, SLOT(OnOkClicked(QString)));
    (void) disconnect(mp_KeyBoardWidget, SIGNAL(EscButtonClicked()), this, SLOT(OnESCClicked()));
}

} // end namespace SystemTracking



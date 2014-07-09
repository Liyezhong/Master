/****************************************************************************/
/*! \file DiagnosticsManufacturing/Source/MainControlManufacturing.cpp
 *
 *  \brief CMainControl class implementation
 *
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-05-29
 *   $Author:  $ Dixiong Li
 *
 *  \b Company:
 *
 *       Leica Biosystems Shanghai.
 *
 *  (C) Copyright 2010 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "DiagnosticsManufacturing/Include/MainControlManufacturing.h"
#include "ui_MainControlManufacturing.h"
#include "Core/Include/SelectTestOptions.h"
#include <QDebug>
#include <QTableWidgetItem>
#include "ServiceDataManager/Include/TestCaseGuide.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

namespace DiagnosticsManufacturing {

const QString REGEXP_NUMERIC_VALIDATOR  = "^[0-9]{1,4}$"; //!< Reg expression for the validator
const int FIXED_LINEEDIT_WIDTH = 241;           ///< Fixed line edit width
const int SET_FIXED_TABLE_WIDTH = 500;          ///< Set table width
const int SET_FIXED_TABLE_HEIGHT = 280;         ///< Set table height
const int VISIBLE_TABLE_ROWS = 4;               ///< Visible table rows

/****************************************************************************/
/*!
 *  \brief Constructor
 *  \iparam p_DataConnector = Service GUI connector object
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CMainControl::CMainControl(Core::CServiceGUIConnector *p_DataConnector, MainMenu::CMainWindow *p_Parent)
    : mp_DataConnector(p_DataConnector)
    , mp_MainWindow(p_Parent)
    , mp_Ui(new Ui::CMainControlManufacturing)
    , mp_Module(NULL)
    , m_FinalTestResult("NA")
    , m_MainControlSNString("040/XXXX")
    , m_SystemSNString("")
    , m_TestFlag(true)
{
    mp_Ui->setupUi(this);

    mp_Ui->eboxSNEdit->installEventFilter(this);
    mp_Ui->eboxSNEdit->setFixedWidth(FIXED_LINEEDIT_WIDTH);

    mp_Ui->beginTestBtn->setEnabled(true);
    mp_Ui->eboxSNEdit->setText(m_MainControlSNString);

    mp_TestReporter = new CTestCaseReporter("MainControl");
    mp_MessageDlg   = new MainMenu::CMessageDlg(mp_MainWindow);
    mp_WaitDlg      = new MainMenu::CWaitDialog(mp_MainWindow);
    mp_WaitDlg->setModal(true);

    mp_TableWidget = new MainMenu::CBaseTable;

    mp_TableWidget->resize(SET_FIXED_TABLE_WIDTH, SET_FIXED_TABLE_HEIGHT);

    mp_TableWidget->horizontalHeader()->show();

    AddItem(1, Service::MAINCONTROL_ASB3);
    AddItem(2, Service::MAINCONTROL_ASB5);
    AddItem(3, Service::MAINCONTROL_ASB15);

    mp_TableWidget->setModel(&m_Model);
    mp_TableWidget->horizontalHeader()->resizeSection(0, 50);   // 0 => Index  50 => Size
    mp_TableWidget->horizontalHeader()->resizeSection(1, 50);   // 1 => Index  50 => Size
    mp_TableWidget->horizontalHeader()->resizeSection(2, 350);  // 2 => Index  400 => Size
    mp_TableWidget->horizontalHeader()->resizeSection(3, 45);   // 3 => Index  45 => Size

    mp_TableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    mp_TableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mp_Ui->widget->setMinimumSize(mp_TableWidget->width(), mp_TableWidget->height());
    mp_Ui->widget->SetContent(mp_TableWidget);
    mp_Ui->sendTestReportBtn->setEnabled(false);

//    mp_Ui->testSuccessLabel->setPixmap(QPixmap(QString::fromUtf8(":/Large/CheckBoxLarge/CheckBox-enabled-large.png")));

    mp_KeyBoardWidget = new KeyBoard::CKeyBoard(KeyBoard::SIZE_1, KeyBoard::QWERTY_KEYBOARD);

    if (mp_DataConnector->GetModuleListContainer()) {
        mp_Module = mp_DataConnector->GetModuleListContainer()->GetModule("Main Control");
    }

    CONNECTSIGNALSLOTGUI(mp_WaitDlg, rejected(), mp_TestReporter, StopSend());
    CONNECTSIGNALSLOTGUI(mp_Ui->beginTestBtn, clicked(), this, BeginTest());
    CONNECTSIGNALSLOTGUI(mp_Ui->sendTestReportBtn, clicked(), this, SendTestReport());
    CONNECTSIGNALSLOTGUI(mp_MainWindow, CurrentTabChanged(int), this, ResetTestStatus());
    CONNECTSIGNALSLOTGUI(this, UpdateModule(ServiceDataManager::CModule&), mp_DataConnector, SendModuleUpdate(ServiceDataManager::CModule&));
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CMainControl::~CMainControl()
{
     try {
        delete mp_KeyBoardWidget;
        delete mp_TableWidget;
        delete mp_Ui;
        delete mp_MessageDlg;
        delete mp_WaitDlg;
        delete mp_TestReporter;

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
void CMainControl::changeEvent(QEvent *p_Event)
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
 *  \brief  Method will be called,when an event triggered on the object
 *
 *  \iparam p_Object = object of the widget
 *  \iparam p_Event = Event object
 *
 *  \return true when event is triggered
 */
/****************************************************************************/
bool CMainControl::eventFilter(QObject *p_Object, QEvent *p_Event)
{
    if (p_Object == mp_Ui->eboxSNEdit && p_Event->type() == QEvent::MouseButtonPress)
    {
        ConnectKeyBoardSignalSlots();
        mp_KeyBoardWidget->setModal(true);
        mp_KeyBoardWidget->SetKeyBoardDialogTitle(QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                           "Enter Serial Number", 0, QApplication::UnicodeUTF8));
        mp_KeyBoardWidget->SetPasswordMode(false);
        mp_KeyBoardWidget->SetValidation(true);
        mp_KeyBoardWidget->SetMinCharLength(4);
        mp_KeyBoardWidget->SetMaxCharLength(4);
        mp_KeyBoardWidget->DisplayNumericKeyBoard();
        // ^ and $ is used for any character. * is used to enter multiple characters
        // [0-9] is used to allow user to enter only 0 to 9 digits
        mp_KeyBoardWidget->SetLineEditValidatorExpression(REGEXP_NUMERIC_VALIDATOR);
        mp_KeyBoardWidget->show();
        return true;
    }
    return false;
}

/****************************************************************************/
/*!
 *  \brief  To add data item to the table
 *  \iparam Index = Test case No.
 *  \iparam
 */
/****************************************************************************/
void CMainControl::AddItem(quint8 Index, Service::ModuleTestCaseID_t Id)
{
    QList<QStandardItem *> ItemList;

    QStandardItem *itemCheckFlag = new QStandardItem;
    itemCheckFlag->setData(Qt::Checked, (int)Qt::CheckStateRole);
    itemCheckFlag->setEditable(true);
    itemCheckFlag->setSelectable(true);
    itemCheckFlag->setCheckable(true);
    itemCheckFlag->setToolTip(DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id));
    ItemList << itemCheckFlag;

    ItemList << new QStandardItem(QString("%1").arg(Index));
    ItemList << new QStandardItem(DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id));

    QPixmap SetPixmap;
    QPixmap PixMap(QString(":/Large/CheckBoxLarge/CheckBox-enabled-large.png"));
    if (!PixMap.isNull())
        SetPixmap = (PixMap.scaled(QSize(45,45),Qt::KeepAspectRatio, Qt::FastTransformation));

    QStandardItem *item = new QStandardItem;
    item->setData(SetPixmap, (int) Qt::DecorationRole);
    ItemList << item;

    m_Model.setHorizontalHeaderLabels(QStringList() << ""
                                                    << QApplication::translate("DiagnosticsManufacturing::CMainControl", "Nr.", 0, QApplication::UnicodeUTF8)
                                                    << QApplication::translate("DiagnosticsManufacturing::CMainControl", "Tests", 0, QApplication::UnicodeUTF8)
                                                    << "");
    m_Model.appendRow(ItemList);
    mp_TestReporter->AddTestCaseId(Id);
}

/****************************************************************************/
/*!
 *  \brief This function is called when OK is clicked
 *  \iparam EnteredString = Stores line edit string
 */
/****************************************************************************/
void CMainControl::OnOkClicked(const QString& EnteredString)
{

    mp_KeyBoardWidget->hide();

    m_MainControlSNString.chop(4);
    m_MainControlSNString.append(EnteredString.simplified());
    mp_Ui->eboxSNEdit->setText(m_MainControlSNString);

    mp_Ui->beginTestBtn->setEnabled(true);
    DisconnectKeyBoardSignalSlots();

    if (mp_Module && Core::CSelectTestOptions::GetCurTestMode() == Core::MANUFACTURAL_ENDTEST) {
        mp_Module->SetSerialNumber(m_MainControlSNString);
        emit UpdateModule(*mp_Module);
    }
}

/****************************************************************************/
/*!
 *  \brief This function hides the keyboard when Esc is clicked
 */
/****************************************************************************/
void CMainControl::OnESCClicked()
{
    // Disconnect signals and slots connected to keyboard.
    DisconnectKeyBoardSignalSlots();
    mp_KeyBoardWidget->hide();
}

/****************************************************************************/
/*!
 *  \brief Connects signals and slots of keyboard.
 */
/****************************************************************************/
void CMainControl::ConnectKeyBoardSignalSlots()
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
void CMainControl::DisconnectKeyBoardSignalSlots()
{
    // Disconnect signals and slots connected to keyboard.
    (void) disconnect(mp_KeyBoardWidget, SIGNAL(OkButtonClicked(QString)), this, SLOT(OnOkClicked(QString)));
    (void) disconnect(mp_KeyBoardWidget, SIGNAL(EscButtonClicked()), this, SLOT(OnESCClicked()));

}

/****************************************************************************/
/*!
 *  \brief Slot called for agitator tests
 */
/****************************************************************************/
void CMainControl::BeginTest()
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MANUF_MAINCONTROL_TEST_REQUESTED);

    if (mp_Ui->eboxSNEdit->text().endsWith("XXXX")) {
        mp_MessageDlg->SetTitle(QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                        "Serial Number", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->SetButtonText(1, QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                                "Ok", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetText(QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                             "Please enter the serial number.", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        if (mp_MessageDlg->exec()) {
            mp_Ui->eboxSNEdit->setFocus();
            mp_Ui->eboxSNEdit->selectAll();
        }
        return;
    }

    qDebug()<<"CMainControl::BeginTest  ";
    QList<Service::ModuleTestCaseID> TestCaseList;
    for(int i=0; i<m_Model.rowCount(); i++) {
        QModelIndex ModelIndex = m_Model.index(i, 0);
        QStandardItem* item = m_Model.itemFromIndex(ModelIndex);
        Qt::CheckState State = item->checkState();
        qDebug()<<"checkable="<<item->checkState()<<" selected="<<item->isSelectable()<<" tooltip="<<item->toolTip();

        if (State == Qt::Checked) {

            Service::ModuleTestCaseID Id = DataManager::CTestCaseGuide::Instance().GetTestCaseId(item->toolTip());
            TestCaseList.append(Id);
        }
    }
    if (TestCaseList.count() == 0) {
        mp_MessageDlg->SetTitle(QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                        "Error", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->SetButtonText(1, QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                                "Ok", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetText(QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                       "Please select a test case.", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->SetIcon(QMessageBox::Critical);
        mp_MessageDlg->show();
    }
    else {
        EnableButton(false);
        mp_Ui->widget->setFocus();

        emit BeginModuleTest(Service::MAIN_CONTROL, TestCaseList);
        if (m_TestFlag) {
            mp_Ui->sendTestReportBtn->setEnabled(true);
        }

        qDebug()<<"CMainControl::BeginTest   --- emitted";
    }
//    ->HideAbort();
}

/****************************************************************************/
/*!
 *  \brief Slot called for agitator tests
 *  \iparam Id = Test case Id
 *  \iparam Result = Result of the test
 */
/****************************************************************************/
void CMainControl::SetTestResult(Service::ModuleTestCaseID Id, bool Result)
{
    QPixmap PixMapPass(QString(":/Large/CheckBoxLarge/CheckBox-Checked_large_green.png"));
    QPixmap PixMapFail(QString(":/Large/CheckBoxLarge/CheckBox-Crossed_large_red.png"));
    QPixmap SetPixMap;

    for(int i=0; i<m_Model.rowCount(); i++) {
        QStandardItem *item = m_Model.item(i, 0);
        if (item->toolTip() == DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id)) {
            if (Result) {
                if (!PixMapPass.isNull())
                    SetPixMap = (PixMapPass.scaled(45,45,Qt::KeepAspectRatio));
            }
            else {
                if (!PixMapFail.isNull())
                    SetPixMap = (PixMapFail.scaled(45,45,Qt::KeepAspectRatio));
            }
            (void) m_Model.setData(m_Model.index(i, 3), SetPixMap, (int) Qt::DecorationRole);
            break;
        }
    }
    m_TestFlag = true;
}

void CMainControl::EnableButton(bool EnableFlag)
{
    mp_Ui->beginTestBtn->setEnabled(EnableFlag);
}

/****************************************************************************/
/*!
 *  \brief Slot called for sending the test report to server
 */
/****************************************************************************/
void CMainControl::SendTestReport()
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MANUF_MAINCONTROL_SENDTESTREPORT_REQUESTED);

    QString systemSN;
    DataManager::CDeviceConfigurationInterface* DevConfigurationInterface = mp_DataConnector->GetDeviceConfigInterface();
    if (DevConfigurationInterface) {
        DataManager::CDeviceConfiguration* DeviceConfiguration = DevConfigurationInterface->GetDeviceConfiguration();
        if (DeviceConfiguration) {
            systemSN = DeviceConfiguration->GetValue("SERIALNUMBER");
        }
    }

    if (systemSN.startsWith("XXXX")) {
        mp_MessageDlg->SetTitle(QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                        "Serial Number", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->SetButtonText(1, QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                                "Ok", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetText(QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                             "Please enter the system serial number.", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        (void)mp_MessageDlg->exec();
        return;
    }

    mp_TestReporter->SetSerialNumber(systemSN);

    if (mp_TestReporter->GenReportFile()) {
        mp_WaitDlg->SetText(QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                    "Sending...", 0, QApplication::UnicodeUTF8));
        mp_WaitDlg->show();
        if (mp_TestReporter->SendReportFile()) {
            mp_WaitDlg->accept();
            mp_MessageDlg->SetTitle(QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                            "Send Report", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->SetButtonText(1, QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                                    "Ok", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->HideButtons();
            mp_MessageDlg->SetText(QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                           "Send test report ok.", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->SetIcon(QMessageBox::Information);
            (void)mp_MessageDlg->exec();
        }
        else {
            mp_WaitDlg->accept();
            mp_MessageDlg->SetTitle(QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                            "Send Report", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->SetButtonText(1, QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                                    "Ok", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->HideButtons();
            mp_MessageDlg->SetText(QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                           "Send test report failed.", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->SetIcon(QMessageBox::Critical);
            (void)mp_MessageDlg->exec();
        }
    }
    else {
        mp_MessageDlg->SetTitle(QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                        "Test Report", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->SetButtonText(1, QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                                "Ok", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetText(QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                       "Test report save failed.", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->SetIcon(QMessageBox::Critical);
        (void)mp_MessageDlg->exec();
    }
}

/****************************************************************************/
/*!
 *  \brief Slot called to reset the test status
 */
/****************************************************************************/
void CMainControl::ResetTestStatus()
{
#if 0
    mp_Ui->beginTestBtn->setEnabled(false);
    mp_Ui->heaterSNEdit->setText("006XXXXX");
    SetLineEditText(QString("006XXXXX"));
    QPixmap PixMap(QString(":/Large/CheckBoxLarge/CheckBox-enabled-large.png"));
    QPixmap SetPixMap;
    if (!PixMap.isNull())
        SetPixMap = (PixMap.scaled(45,45,Qt::KeepAspectRatio));
    (void) m_Model.setData(m_Model.index(0, 2), SetPixMap, (int) Qt::DecorationRole);
    (void) m_Model.setData(m_Model.index(1, 2), SetPixMap, (int) Qt::DecorationRole);
    mp_Ui->sendTestReportBtn->setEnabled(false);
//    mp_Ui->testSuccessLabel->setPixmap(QPixmap(QString::fromUtf8
//                                               (":/Large/CheckBoxLarge/CheckBox-enabled-large.png")));
#endif

}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CMainControl::RetranslateUI()
{
#if 0
    m_Model.setHorizontalHeaderLabels(QStringList() << QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                                               "Nr.", 0, QApplication::UnicodeUTF8)
                                                    << QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                                               "Tests", 0, QApplication::UnicodeUTF8)
                                                    << "");

    m_Model.clear();

    AddItem("1", QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                         "X1 Reference Run", 0, QApplication::UnicodeUTF8));
    AddItem("2", QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                         "X2 Reference Run", 0, QApplication::UnicodeUTF8));

    mp_TableWidget->horizontalHeader()->resizeSection(0, 50);   // 0 => Index  50 => Size
    mp_TableWidget->horizontalHeader()->resizeSection(1, 400);  // 1 => Index  400 => Size
    mp_TableWidget->horizontalHeader()->resizeSection(2, 45);   // 2 => Index  45 => Size

    mp_KeyBoardWidget->SetKeyBoardDialogTitle(QApplication::translate("DiagnosticsManufacturing::COven",
                                             "Enter Serial Number", 0, QApplication::UnicodeUTF8));

    mp_MessageDlg->SetTitle(QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                    "Serial Number", 0, QApplication::UnicodeUTF8));

    mp_MessageDlg->SetText(QApplication::translate("DiagnosticsManufacturing::CMainControl",
                           "Please enter the serial number.", 0, QApplication::UnicodeUTF8));

    mp_MessageDlg->SetTitle(QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                    "Test Report", 0, QApplication::UnicodeUTF8));

    mp_MessageDlg->SetText(QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                  "Test report saved successfully.", 0, QApplication::UnicodeUTF8));

    mp_MessageDlg->SetText(QApplication::translate("DiagnosticsManufacturing::CMainControl",
                                                   "Test report save failed.", 0, QApplication::UnicodeUTF8));
#endif
}

}  // end namespace DiagnosticsManufacturing


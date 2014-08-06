/****************************************************************************/
/*! \file DiagnosticsManufacturing/Source/LaSystemManufacturing.cpp
 *
 *  \brief CLaSystem class implementation
 *
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-05-30
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

#include "DiagnosticsManufacturing/Include/LaSystemManufacturing.h"
#include "DiagnosticsManufacturing/Include/TestCaseReporter.h"
#include "Core/Include/SelectTestOptions.h"
#include "ui_LaSystemManufacturing.h"
#include <QDebug>
#include <QTableWidgetItem>
#include "Core/Include/ServiceDefines.h"
#include "ServiceDataManager/Include/TestCaseGuide.h"
#include "Main/Include/HimalayaServiceEventCodes.h"
#include "Core/Include/CMessageString.h"

namespace DiagnosticsManufacturing {

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
CLaSystem::CLaSystem(Core::CServiceGUIConnector *p_DataConnector, MainMenu::CMainWindow *p_Parent)
    : mp_DataConnector(p_DataConnector)
    , mp_MainWindow(p_Parent)
    , mp_Ui(new Ui::CLaSystemManufacturing)
    , m_FinalTestResult("NA")
    , m_TestFlag(false)
{
    mp_Ui->setupUi(this);

    mp_Ui->beginTestBtn->setEnabled(true);

    mp_TestReporter = new CTestCaseReporter("LaSystem", mp_DataConnector, mp_MainWindow);
    mp_MessageDlg   = new MainMenu::CMessageDlg(mp_MainWindow);
    mp_WaitDlg      = new MainMenu::CWaitDialog(mp_MainWindow);
    mp_WaitDlg->setModal(true);

    mp_TableWidget = new MainMenu::CBaseTable;

    mp_TableWidget->resize(SET_FIXED_TABLE_WIDTH, SET_FIXED_TABLE_HEIGHT);

    mp_TableWidget->horizontalHeader()->show();

    AddItem(1, Service::LA_SYSTEM_HEATING_LIQUID_TUBE);
    AddItem(2, Service::LA_SYSTEM_HEATING_AIR_TUBE);

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

    CONNECTSIGNALSLOTGUI(mp_Ui->beginTestBtn, clicked(), this, BeginTest());
    CONNECTSIGNALSLOTGUI(mp_Ui->sendTestReportBtn, clicked(), this, SendTestReport());
    CONNECTSIGNALSLOTGUI(mp_MainWindow, CurrentTabChanged(int), this, ResetTestStatus());
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CLaSystem::~CLaSystem()
{
     try {
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
void CLaSystem::changeEvent(QEvent *p_Event)
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
 *  \brief  To add data item to the table
 *  \iparam Index = Test case No.
 *  \iparam
 */
/****************************************************************************/
void CLaSystem::AddItem(quint8 Index, Service::ModuleTestCaseID_t Id)
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
                                                    << Service::CMessageString::MSG_DIAGNOSTICS_TEST_NUMBER
                                                    << Service::CMessageString::MSG_DIAGNOSTICS_TEST_NAME
                                                    << "");
    m_Model.appendRow(ItemList);
    mp_TestReporter->AddTestCaseId(Id);
}

/****************************************************************************/
/*!
 *  \brief Slot called for agitator tests
 */
/****************************************************************************/
void CLaSystem::BeginTest()
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MANUF_LASYSTEM_TEST_REQUESTED);

    qDebug()<<"CLaSystem::BeginTest  ";
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
        mp_MessageDlg->SetTitle(Service::CMessageString::MSG_TITLE_ERROR);
        mp_MessageDlg->SetButtonText(1, Service::CMessageString::MSG_BUTTON_OK);
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetText(Service::CMessageString::MSG_DIAGNOSTICS_SELECT_TEST_CASE);
        mp_MessageDlg->SetIcon(QMessageBox::Critical);
        mp_MessageDlg->show();
    }
    else {
        EnableButton(false);
        mp_Ui->widget->setFocus();

        emit BeginModuleTest(Service::LA_SYSTEM, TestCaseList);
        if (m_TestFlag) {
            mp_Ui->sendTestReportBtn->setEnabled(true);
        }

        qDebug()<<"CLaSystem::BeginTest   --- emitted";
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
void CLaSystem::SetTestResult(Service::ModuleTestCaseID Id, bool Result)
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

void CLaSystem::EnableButton(bool EnableFlag)
{
    mp_Ui->beginTestBtn->setEnabled(EnableFlag);
}

/****************************************************************************/
/*!Service::CMessageString::MSG_DIAGNOSTICS_SELECT_TEST_CAService::CMessageString::MSG_DIAGNOSTICS_SELECT_TEST_CASESE
 *  \brief Slot called for sending the test report to server
 */
/****************************************************************************/
void CLaSystem::SendTestReport()
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MANUF_LASYSTEM_SENDTESTREPORT_REQUESTED);

    QString systemSN;
    DataManager::CDeviceConfigurationInterface* DevConfigurationInterface = mp_DataConnector->GetDeviceConfigInterface();
    if (DevConfigurationInterface) {
        DataManager::CDeviceConfiguration* DeviceConfiguration = DevConfigurationInterface->GetDeviceConfiguration();
        if (DeviceConfiguration) {
            systemSN = DeviceConfiguration->GetValue("SERIALNUMBER");
        }
    }

    if (systemSN.startsWith("XXXX")) {
        mp_MessageDlg->SetTitle(Service::CMessageString::MSG_TITLE_SERIAL_NUMBER);
        mp_MessageDlg->SetButtonText(1, Service::CMessageString::MSG_BUTTON_OK);
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetText(Service::CMessageString::MSG_DIAGNOSTICS_ENTER_SYSTEM_SN);
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        (void)mp_MessageDlg->exec();
        return;
    }

    mp_TestReporter->SetSerialNumber(systemSN);

    if (mp_TestReporter->GenReportFile()) {
        (void)mp_TestReporter->SendReportFile();
    }
    else {
        mp_MessageDlg->SetTitle(Service::CMessageString::MSG_TITLE_SEND_REPORT);
        mp_MessageDlg->SetButtonText(1, Service::CMessageString::MSG_BUTTON_OK);
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetText(Service::CMessageString::MSG_DIAGNOSTICS_GEN_REPORT_FILE_FAILED);
        mp_MessageDlg->SetIcon(QMessageBox::Critical);
        (void)mp_MessageDlg->exec();
    }
}

/****************************************************************************/
/*!
 *  \brief Slot called to reset the test status
 */
/****************************************************************************/
void CLaSystem::ResetTestStatus()
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
void CLaSystem::RetranslateUI()
{
#if 0
    m_Model.setHorizontalHeaderLabels(QStringList() << QApplication::translate("DiagnosticsManufacturing::CLaSystem",
                                                                               "Nr.", 0, QApplication::UnicodeUTF8)
                                                    << QApplication::translate("DiagnosticsManufacturing::CLaSystem",
                                                                               "Tests", 0, QApplication::UnicodeUTF8)
                                                    << "");

    m_Model.clear();

    AddItem("1", QApplication::translate("DiagnosticsManufacturing::CLaSystem",
                                         "X1 Reference Run", 0, QApplication::UnicodeUTF8));
    AddItem("2", QApplication::translate("DiagnosticsManufacturing::CLaSystem",
                                         "X2 Reference Run", 0, QApplication::UnicodeUTF8));

    mp_TableWidget->horizontalHeader()->resizeSection(0, 50);   // 0 => Index  50 => Size
    mp_TableWidget->horizontalHeader()->resizeSection(1, 400);  // 1 => Index  400 => Size
    mp_TableWidget->horizontalHeader()->resizeSection(2, 45);   // 2 => Index  45 => Size

    mp_KeyBoardWidget->SetKeyBoardDialogTitle(QApplication::translate("DiagnosticsManufacturing::CLaSystem",
                                             "Enter Serial Number", 0, QApplication::UnicodeUTF8));

    mp_MessageDlg->SetTitle(QApplication::translate("DiagnosticsManufacturing::CLaSystem",
                                                    "Serial Number", 0, QApplication::UnicodeUTF8));

    mp_MessageDlg->SetText(QApplication::translate("DiagnosticsManufacturing::CLaSystem",
                           "Please enter the serial number.", 0, QApplication::UnicodeUTF8));

    mp_MessageDlg->SetTitle(QApplication::translate("DiagnosticsManufacturing::CLaSystem",
                                                    "Test Report", 0, QApplication::UnicodeUTF8));

    mp_MessageDlg->SetText(QApplication::translate("DiagnosticsManufacturing::CLaSystem",
                                                  "Test report saved successfully.", 0, QApplication::UnicodeUTF8));

    mp_MessageDlg->SetText(QApplication::translate("DiagnosticsManufacturing::CLaSystem",
                                                   "Test report save failed.", 0, QApplication::UnicodeUTF8));
#endif
}

}  // end namespace DiagnosticsManufacturing

/****************************************************************************/
/*! \file DiagnosticsManufacturing/Source/CleanSystemManufacturing.cpp
 *
 *  \brief CCleanSystem class implementation
 *
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-06-23
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

#include "DiagnosticsManufacturing/Include/CleaningManufacturing.h"
#include "DiagnosticsManufacturing/Include/TestCaseReporter.h"
#include "Core/Include/SelectTestOptions.h"
#include "ui_CleaningManufacturing.h"
#include <QDebug>
#include <QTableWidgetItem>
#include "Core/Include/ServiceDefines.h"
#include "ServiceDataManager/Include/TestCaseGuide.h"
#include "Main/Include/HimalayaServiceEventCodes.h"

namespace DiagnosticsManufacturing {

const QString REGEXP_NUMERIC_VALIDATOR  = "^[0-9]{1,5}$"; //!< Reg expression for the validator
const int FIXED_LINEEDIT_WIDTH = 241;           ///< Fixed line edit width
const int SET_FIXED_TABLE_WIDTH = 500;          ///< Set table width
const int SET_FIXED_TABLE_HEIGHT = 200;         ///< Set table height
const int VISIBLE_TABLE_ROWS = 4;               ///< Visible table rows

/****************************************************************************/
/*!
 *  \brief Constructor
 *  \iparam p_DataConnector = Service GUI connector object
 *  \iparam p_Parent = Parent widget
 */
/****************************************************************************/
CCleaning::CCleaning(Core::CServiceGUIConnector *p_DataConnector, MainMenu::CMainWindow *p_Parent)
    : mp_DataConnector(p_DataConnector)
    , mp_MainWindow(p_Parent)
    , mp_Ui(new Ui::CCleaningManufacturing)
    , mp_MessageDlg(NULL)
    , m_FinalTestResult("NA")
{
    mp_Ui->setupUi(this);

    mp_Ui->beginTestBtn->setEnabled(true);

    mp_MessageDlg = new MainMenu::CMessageDlg(mp_MainWindow);

    mp_TableWidget = new MainMenu::CBaseTable;

    mp_TableWidget->resize(SET_FIXED_TABLE_WIDTH, SET_FIXED_TABLE_HEIGHT);

    mp_TableWidget->horizontalHeader()->show();

    AddItem(1, Service::CLEANING_SYSTEM_TEST);

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

    CONNECTSIGNALSLOTGUI(mp_Ui->beginTestBtn, clicked(), this, BeginTest());
    CONNECTSIGNALSLOTGUI(mp_Ui->sendTestReportBtn, clicked(), this, SendTestReport());
    CONNECTSIGNALSLOTGUI(mp_MainWindow, CurrentTabChanged(int), this, ResetTestStatus());
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CCleaning::~CCleaning()
{
     try {
        delete mp_TableWidget;
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
void CCleaning::changeEvent(QEvent *p_Event)
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
void CCleaning::AddItem(quint8 Index, Service::ModuleTestCaseID_t Id)
{
    QList<QStandardItem *> itemList;

    QStandardItem *itemCheckFlag = new QStandardItem;
    //itemCheckFlag->setData(Qt::Checked, (int)Qt::CheckStateRole);
    itemCheckFlag->setData(QVariant(Qt::Checked), (int)Qt::CheckStateRole); //PC-lint
    itemCheckFlag->setEditable(true);
    itemCheckFlag->setSelectable(true);
    itemCheckFlag->setCheckable(true);
    itemCheckFlag->setSizeHint(QSize(30,30));
    itemCheckFlag->setToolTip(DataManager::CTestCaseGuide::Instance().GetTestCaseName(Id));
    itemList << itemCheckFlag;

    itemList << new QStandardItem(QString("%1").arg(Index));
    itemList << new QStandardItem(DataManager::CTestCaseGuide::Instance().GetTestCaseDescription(Id));

    QPixmap SetPixmap;
    QPixmap PixMap(QString(":/Large/CheckBoxLarge/CheckBox-enabled-large.png"));
    if (!PixMap.isNull())
        SetPixmap = (PixMap.scaled(QSize(36,36),Qt::KeepAspectRatio, Qt::FastTransformation));

    QStandardItem *item = new QStandardItem;
    item->setData(SetPixmap, (int) Qt::DecorationRole);
    itemList << item;

    m_Model.setHorizontalHeaderLabels(QStringList() << ""
                                                    << QApplication::translate("DiagnosticsManufacturing::CCleaning", "Nr.", 0, QApplication::UnicodeUTF8)
                                                    << QApplication::translate("DiagnosticsManufacturing::CCleaning", "Tests", 0, QApplication::UnicodeUTF8)
                                                    << "");
    m_Model.appendRow(itemList);
}

/****************************************************************************/
/*!
 *  \brief Slot called for agitator tests
 */
/****************************************************************************/
void CCleaning::BeginTest()
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MANUF_CLEANING_SYSTEM_TEST_REQUESTED);

    qDebug()<<"CCleaning::BeginTest  ";
    QList<Service::ModuleTestCaseID> TestCaseList;
    for(int i=0; i<m_Model.rowCount(); i++) {
        QModelIndex ModelIndex = m_Model.index(i, 0);
        QStandardItem* item = m_Model.itemFromIndex(ModelIndex);
        Qt::CheckState State = item->checkState();
        //qDebug()<<"checkable="<<item->checkState()<<" selected="<<item->isSelectable()<<" tooltip="<<item->toolTip();
        qDebug()<<"checkable="<<(int)State<<" selected="<<item->isSelectable()<<" tooltip="<<item->toolTip(); //PC-lint

        if (State == Qt::Checked) {
            Service::ModuleTestCaseID Id = DataManager::CTestCaseGuide::Instance().GetTestCaseId(item->toolTip());
            TestCaseList.append(Id);
        }
    }
    if (TestCaseList.count() == 0) {
        mp_MessageDlg->SetTitle(QApplication::translate("DiagnosticsManufacturing::CCleaning",
                                                        "Error", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->SetButtonText(1, QApplication::translate("DiagnosticsManufacturing::CCleaning",
                                                                "Ok", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->HideButtons();
        mp_MessageDlg->SetText(QApplication::translate("DiagnosticsManufacturing::CCleaning",
                                                       "Please select a test case.", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->SetIcon(QMessageBox::Critical);
        mp_MessageDlg->show();
    }
    else {
        EnableButton(false);
        mp_Ui->widget->setFocus();

        emit BeginModuleTest(Service::CLEANING_SYSTEM, TestCaseList);

        qDebug()<<"CCleaning::BeginTest   --- emitted";
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
void CCleaning::SetTestResult(Service::ModuleTestCaseID Id, bool Result)
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

}

void CCleaning::EnableButton(bool EnableFlag)
{
    mp_Ui->beginTestBtn->setEnabled(EnableFlag);
}

/****************************************************************************/
/*!
 *  \brief Slot called for sending the test report to server
 */
/****************************************************************************/
void CCleaning::SendTestReport()
{
    Global::EventObject::Instance().RaiseEvent(EVENT_GUI_MANUF_CLEANING_SYSTEM_SENDTESTREPORT_REQUESTED);
    //else {
        /*
        CTestCaseReporter* p_TestReporter = new CTestCaseReporter(mp_MainWindow, "Cleaning", m_LineEditString);

        mp_MessageDlg->SetTitle(QApplication::translate("DiagnosticsManufacturing::CCleaning",
                                                    "Test Report", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->SetButtonText(1, QApplication::translate("DiagnosticsManufacturing::CLaSystem",
                                                            "Ok", 0, QApplication::UnicodeUTF8));
        mp_MessageDlg->HideButtons();
        if (p_TestReporter->GenReportFile()) {
            mp_MessageDlg->SetText(QApplication::translate("DiagnosticsManufacturing::CCleaning",
                                          "Test report saved successfully.", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->SetIcon(QMessageBox::Information);
        }
        else {
            mp_MessageDlg->SetText(QApplication::translate("DiagnosticsManufacturing::CCleaning",
                                                       "Test report save failed.", 0, QApplication::UnicodeUTF8));
            mp_MessageDlg->SetIcon(QMessageBox::Critical);
        }
        (void)mp_MessageDlg->exec();
        */
    //}

}

/****************************************************************************/
/*!
 *  \brief Slot called to reset the test status
 */
/****************************************************************************/
void CCleaning::ResetTestStatus()
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
void CCleaning::RetranslateUI()
{
#if 0
    m_Model.setHorizontalHeaderLabels(QStringList() << QApplication::translate("DiagnosticsManufacturing::CCleaning",
                                                                               "Nr.", 0, QApplication::UnicodeUTF8)
                                                    << QApplication::translate("DiagnosticsManufacturing::CCleaning",
                                                                               "Tests", 0, QApplication::UnicodeUTF8)
                                                    << "");

    m_Model.clear();

    AddItem("1", QApplication::translate("DiagnosticsManufacturing::CCleaning",
                                         "X1 Reference Run", 0, QApplication::UnicodeUTF8));
    AddItem("2", QApplication::translate("DiagnosticsManufacturing::CCleaning",
                                         "X2 Reference Run", 0, QApplication::UnicodeUTF8));

    mp_TableWidget->horizontalHeader()->resizeSection(0, 50);   // 0 => Index  50 => Size
    mp_TableWidget->horizontalHeader()->resizeSection(1, 400);  // 1 => Index  400 => Size
    mp_TableWidget->horizontalHeader()->resizeSection(2, 45);   // 2 => Index  45 => Size

    mp_KeyBoardWidget->SetKeyBoardDialogTitle(QApplication::translate("DiagnosticsManufacturing::CCleanSystem",
                                             "Enter Serial Number", 0, QApplication::UnicodeUTF8));

    mp_MessageDlg->SetTitle(QApplication::translate("DiagnosticsManufacturing::CCleaning",
                                                    "Serial Number", 0, QApplication::UnicodeUTF8));

    mp_MessageDlg->SetText(QApplication::translate("DiagnosticsManufacturing::CCleaning",
                           "Please enter the serial number.", 0, QApplication::UnicodeUTF8));

    mp_MessageDlg->SetTitle(QApplication::translate("DiagnosticsManufacturing::CCleaning",
                                                    "Test Report", 0, QApplication::UnicodeUTF8));

    mp_MessageDlg->SetText(QApplication::translate("DiagnosticsManufacturing::CCleaning",
                                                  "Test report saved successfully.", 0, QApplication::UnicodeUTF8));

    mp_MessageDlg->SetText(QApplication::translate("DiagnosticsManufacturing::CCleaning",
                                                   "Test report save failed.", 0, QApplication::UnicodeUTF8));
#endif
}

}  // end namespace DiagnosticsManufacturing



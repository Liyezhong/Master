#include "Settings/Include/BottleCheckStatusDlg.h"
#include "ui_BottleCheckStatusDlg.h"
#include "Core/Include/DataConnector.h"

namespace Settings {
    CBottleCheckStatusDlg::CBottleCheckStatusDlg(QWidget *parent, Core::CDataConnector* pDataConnect) :
        MainMenu::CDialogFrame(parent),
        ui(new Ui::CBottleCheckStatusDlg),
        m_Waiting(tr("Waiting")),m_Empty(tr("Empty")),m_Passed(tr("Passed")),
        m_Blockage(tr("Blockage")),m_Checking(tr("Checking"))

    {
        ui->setupUi(GetContentFrame());
        SetDialogTitle(tr("Bottle check"));

        mp_TableWidget = new MainMenu::CBaseTable;
        mp_TableWidget->setModel(&m_BottleCheckStatusModel);
        ui->bottleCheckStatusTable->SetContent(mp_TableWidget);
        m_BottleCheckStatusModel.SetRequiredContainers(pDataConnect->ReagentList, pDataConnect->DashboardStationList);
        mp_TableWidget->horizontalHeader()->show();
        mp_TableWidget->SetVisibleRows(7);
        m_BottleCheckStatusModel.SetVisibleRowCount(7);
        mp_TableWidget->horizontalHeader()->resizeSection(0, 80);
        mp_TableWidget->horizontalHeader()->resizeSection(1, 160);
        mp_TableWidget->horizontalHeader()->resizeSection(2, 80);
        mp_TableWidget->selectRow(0);
        CONNECTSIGNALSLOT(ui->btnClose, clicked(), this, OnClose());

        m_StationIDRowMap.insert("S1", 0);
        m_StationIDRowMap.insert("S2", 1);
        m_StationIDRowMap.insert("S3", 2);
        m_StationIDRowMap.insert("S4", 3);
        m_StationIDRowMap.insert("S5", 4);
        m_StationIDRowMap.insert("S6", 5);
        m_StationIDRowMap.insert("S7", 6);
        m_StationIDRowMap.insert("S8", 7);
        m_StationIDRowMap.insert("S9", 8);
        m_StationIDRowMap.insert("S10", 9);
        m_StationIDRowMap.insert("S11", 10);
        m_StationIDRowMap.insert("S12", 11);
        m_StationIDRowMap.insert("S13", 12);
        m_StationIDRowMap.insert("P1", 13);
        m_StationIDRowMap.insert("P2", 14);
        m_StationIDRowMap.insert("P3", 15);


        m_BottleCheckStatusMap.insert(DataManager::BOTTLECHECK_WAITING, m_Waiting);
        m_BottleCheckStatusMap.insert(DataManager::BOTTLECHECK_EMPTY, m_Empty);
        m_BottleCheckStatusMap.insert(DataManager::BOTTLECHECK_PASSED, m_Passed);
        m_BottleCheckStatusMap.insert(DataManager::BOTTLECHECK_BLOCKAGE, m_Blockage);
        m_BottleCheckStatusMap.insert(DataManager::BOTTLECHECK_CHECKING, m_Checking);
        m_BottleCheckStatusMap.insert(DataManager::BOTTLECHECK_WILLNOTPROCESS, " --- ");

        m_StationStatusMap.insert("S1", m_Waiting);
        m_StationStatusMap.insert("S2", m_Waiting);
        m_StationStatusMap.insert("S3", m_Waiting);
        m_StationStatusMap.insert("S4", m_Waiting);
        m_StationStatusMap.insert("S5", m_Waiting);
        m_StationStatusMap.insert("S6", m_Waiting);
        m_StationStatusMap.insert("S7", m_Waiting);
        m_StationStatusMap.insert("S8", m_Waiting);

        m_StationStatusMap.insert("S9", m_Waiting);
        m_StationStatusMap.insert("S10", m_Waiting);
        m_StationStatusMap.insert("S11", m_Waiting);
        m_StationStatusMap.insert("S12", m_Waiting);
        m_StationStatusMap.insert("S13", m_Waiting);
        m_StationStatusMap.insert("P1", m_Waiting);
        m_StationStatusMap.insert("P2", m_Waiting);
        m_StationStatusMap.insert("P3", m_Waiting);

        m_BottleCheckStatusModel.SetBottleCheckStatusMap(m_StationStatusMap);

        ui->btnClose->setEnabled(false);

    }

    CBottleCheckStatusDlg::~CBottleCheckStatusDlg()
    {
        delete ui;
        delete mp_TableWidget;
    }

    /****************************************************************************/
    /*!
     *  \brief Event handler for change events
     *
     *  \iparam p_Event = Change event
     */
    /****************************************************************************/

    void CBottleCheckStatusDlg::changeEvent(QEvent *p_Event)
    {
        QDialog::changeEvent(p_Event);
        switch (p_Event->type()) {
            case QEvent::LanguageChange:
                ui->retranslateUi(this);
                RetranslateUI();
                break;
            default:
                break;
        }
    }

    /****************************************************************************/
    /*!
     *  \brief Translates the strings in UI to the selected language
     */
    /****************************************************************************/
    void CBottleCheckStatusDlg::RetranslateUI()
    {
        (void) m_BottleCheckStatusModel.setHeaderData(0, Qt::Horizontal,QApplication::translate("Settings::CBottleCheckStatusModel",
                                                                                     "Station", 0, QApplication::UnicodeUTF8),0);

        (void) m_BottleCheckStatusModel.setHeaderData(1, Qt::Horizontal,QApplication::translate("Settings::CBottleCheckStatusModel",
                                                                                     "Reagent", 0, QApplication::UnicodeUTF8),0);

        (void) m_BottleCheckStatusModel.setHeaderData(2, Qt::Horizontal,QApplication::translate("Settings::CBottleCheckStatusModel",
                                                                                     "Status", 0, QApplication::UnicodeUTF8),0);
        m_Waiting = QApplication::translate("Settings::CBottleCheckStatusModel", "Waiting", 0, QApplication::UnicodeUTF8);
        m_Empty = QApplication::translate("Settings::CBottleCheckStatusModel", "Empty", 0, QApplication::UnicodeUTF8);
        m_Passed = QApplication::translate("Settings::CBottleCheckStatusModel", "Passed", 0, QApplication::UnicodeUTF8);
        m_Blockage = QApplication::translate("Settings::CBottleCheckStatusModel", "Blockage", 0, QApplication::UnicodeUTF8);
        m_Checking = QApplication::translate("Settings::CBottleCheckStatusModel", "Checking", 0, QApplication::UnicodeUTF8);

        m_BottleCheckStatusMap.clear();
        m_BottleCheckStatusMap.insert(DataManager::BOTTLECHECK_WAITING, m_Waiting);
        m_BottleCheckStatusMap.insert(DataManager::BOTTLECHECK_EMPTY, m_Empty);
        m_BottleCheckStatusMap.insert(DataManager::BOTTLECHECK_PASSED, m_Passed);
        m_BottleCheckStatusMap.insert(DataManager::BOTTLECHECK_BLOCKAGE, m_Blockage);
        m_BottleCheckStatusMap.insert(DataManager::BOTTLECHECK_CHECKING, m_Checking);
        m_BottleCheckStatusMap.insert(DataManager::BOTTLECHECK_WILLNOTPROCESS, " --- ");

        m_StationStatusMap.clear();
        m_StationStatusMap.insert("S1", m_Waiting);
        m_StationStatusMap.insert("S2", m_Waiting);
        m_StationStatusMap.insert("S3", m_Waiting);
        m_StationStatusMap.insert("S4", m_Waiting);
        m_StationStatusMap.insert("S5", m_Waiting);
        m_StationStatusMap.insert("S6", m_Waiting);
        m_StationStatusMap.insert("S7", m_Waiting);
        m_StationStatusMap.insert("S8", m_Waiting);

        m_StationStatusMap.insert("S9", m_Waiting);
        m_StationStatusMap.insert("S10", m_Waiting);
        m_StationStatusMap.insert("S11", m_Waiting);
        m_StationStatusMap.insert("S12", m_Waiting);
        m_StationStatusMap.insert("S13", m_Waiting);
        m_StationStatusMap.insert("P1", m_Waiting);
        m_StationStatusMap.insert("P2", m_Waiting);
        m_StationStatusMap.insert("P3", m_Waiting);

        m_BottleCheckStatusModel.SetBottleCheckStatusMap(m_StationStatusMap);
    }

    void CBottleCheckStatusDlg::OnClose()
    {
        this->close();
    }

    /****************************************************************************/
    /*!
     *  \brief Update current bottle check status for the specified station
     *  \param stationID = station ID
     *  \param bottleCheckStatusType = status type
     */
    /****************************************************************************/
    void CBottleCheckStatusDlg::UpdateCurrentStationStatus(const QString& stationID, DataManager::BottleCheckStatusType_t bottleCheckStatusType)
    {
        m_BottleCheckStatusModel.UpdateStatusData(stationID, m_BottleCheckStatusMap[bottleCheckStatusType],
                                                      m_StationIDRowMap[stationID]);
        if (DataManager::BOTTLECHECK_ALLCOMPLETE == bottleCheckStatusType || DataManager::BOTTLECHECK_FAILED == bottleCheckStatusType)
        {
            ui->btnClose->setEnabled(true);
        }
    }


}

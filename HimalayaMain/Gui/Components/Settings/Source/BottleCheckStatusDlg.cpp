#include "Settings/Include/BottleCheckStatusDlg.h"
#include "ui_BottleCheckStatusDlg.h"
#include "Core/Include/DataConnector.h"

namespace Settings {
    CBottleCheckStatusDlg::CBottleCheckStatusDlg(QWidget *parent, Core::CDataConnector* pDataConnect) :
        MainMenu::CDialogFrame(parent),
        ui(new Ui::CBottleCheckStatusDlg)
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
        CONNECTSIGNALSLOT(ui->btnClose, clicked(), this, OnClose());
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
    }

    void CBottleCheckStatusDlg::OnClose()
    {
        this->close();
    }

}

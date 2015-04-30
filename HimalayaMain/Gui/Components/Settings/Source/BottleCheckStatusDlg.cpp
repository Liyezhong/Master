#include "Settings/Include/BottleCheckStatusDlg.h"
#include "ui_BottleCheckStatusDlg.h"

namespace Settings {
    CBottleCheckStatusDlg::CBottleCheckStatusDlg(QWidget *parent) :
        MainMenu::CDialogFrame(parent),
        ui(new Ui::CBottleCheckStatusDlg)
    {
        ui->setupUi(GetContentFrame());
        SetDialogTitle(tr("Bottle check"));

        mp_TableWidget = new MainMenu::CBaseTable;
        m_BottleCheckStatusModel.SetVisibleRowCount(8);
        mp_TableWidget->setModel(&m_BottleCheckStatusModel);
        ui->bottleCheckStatusTable->SetContent(mp_TableWidget);

        mp_TableWidget->horizontalHeader()->show();
        mp_TableWidget->SetVisibleRows(8);
        m_BottleCheckStatusModel.SetVisibleRowCount(8);
        mp_TableWidget->horizontalHeader()->resizeSection(0, 80);
        mp_TableWidget->horizontalHeader()->resizeSection(1, 180);
        mp_TableWidget->horizontalHeader()->resizeSection(2, 80);
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

    }

}

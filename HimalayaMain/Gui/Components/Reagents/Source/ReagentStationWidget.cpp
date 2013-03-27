/****************************************************************************/
#include "Global/Include/Utils.h"
#include "Global/Include/Exception.h"
#include "MainMenu/Include/ScrollWheel.h"
#include "MainMenu/Include/SliderControl.h"
#include "Reagents/Include/ReagentStationWidget.h"
#include "Reagents/Include/ModifyReagentStatusDlg.h"
#include "Reagents/Build/ui_ReagentStationWidget.h"


#include <QDebug>

namespace Reagents {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_ReagentList = Reference to Global reagent list
 *  \iparam p_Parent = Reference to Parent object
 *  \iparam p_KeyBoard = Reference to Keyboard widget
 *  \iparam p_DataConnector = Reference to DataConnector
 */
/****************************************************************************/
CReagentStationWidget::CReagentStationWidget(QWidget *p_Parent):
                                           MainMenu::CPanelFrame(p_Parent),
                                           mp_Ui(new Ui::CReagentStationWidget),
                                           mp_DataConnector(NULL),
                                           mp_ReagentList(NULL),
                                           mp_Reagent(NULL)
{
    mp_Ui->setupUi(GetContentFrame());
    SetPanelTitle(tr("Station"));

    m_ShowMessageDialog = false;
    m_ProcessRunning = false;

    mp_TableWidget = new MainMenu::CBaseTable;
    mp_TableWidget->setModel(&m_ReagentStationModel);
    m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();

    mp_Ui->scrollTable->SetContent(mp_TableWidget);

    PopulateReagentList();
    CONNECTSIGNALSLOT(mp_TableWidget, pressed(QModelIndex), this, SelectionChanged(QModelIndex));
    CONNECTSIGNALSLOT(mp_Ui->btnEdit, clicked(), this, OnEdit());
    CONNECTSIGNALSLOT(this, UpdateReagentList(), &m_ReagentStationModel, UpdateReagentList());
    CONNECTSIGNALSLOT(mp_Ui->scrollTable,Scrolled(), this, OnContentScrolled());
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CReagentStationWidget::~CReagentStationWidget()
{
    try {
        delete mp_TableWidget;
        delete mp_Ui;

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
void CReagentStationWidget::changeEvent(QEvent *p_Event)
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
 *  \brief Sets the settings data object for this class
 *
 *  \iparam p_UserSettings = Global user settings data
 */
/****************************************************************************/
void CReagentStationWidget::SetUserSettings(DataManager::CUserSettings *p_UserSettings)
{
    mp_UserSettings = p_UserSettings;
}


/****************************************************************************/
/*!
 *  \brief Displays a dialog for the modification of a staining reagent
 */
/****************************************************************************/
void CReagentStationWidget::OnEdit()
{
    m_MessageDlg.SetText(tr("Select Reagent"));
    mp_ModifiyReagentStatusDlg->SetDialogTitle(tr("Select Reagent"));
    mp_ModifiyReagentStatusDlg->SetDashboardStation(mp_DashStation);
    mp_ModifiyReagentStatusDlg->move(96,70);
    mp_ModifiyReagentStatusDlg->show();
}

/****************************************************************************/
/*!
 *  \brief Fills the reagents list with content
 */
/****************************************************************************/
void CReagentStationWidget::PopulateReagentList()
{
    mp_TableWidget->SetVisibleRows(8);
    m_ReagentStationModel.SetVisibleRowCount(8);
    mp_TableWidget->horizontalHeader()->show();
    ResizeHorizontalSection();
}

/****************************************************************************/
/*!
 *  \brief Resizes the columns in the reagents table
 */
/****************************************************************************/
void CReagentStationWidget::ResizeHorizontalSection()
{
    if (mp_TableWidget == NULL)
    {
        return;
    }
    mp_TableWidget->horizontalHeader()->resizeSection(0, 150);
    mp_TableWidget->horizontalHeader()->resizeSection(1, 150);
}

/****************************************************************************/
/*!
 *  \brief Called when the selection of the table is changed
 *
 *  \iparam Index = Currently selected cell
 */
/****************************************************************************/
void CReagentStationWidget::SelectionChanged(QModelIndex Index)
{
    QString Id = m_ReagentStationModel.data(Index, (int)Qt::UserRole).toString();
    mp_DashStation = const_cast<DataManager::CDashboardStation*>(mp_DataConnector->DashboardStationList->GetDashboardStation(Id));

    if (mp_DashStation) {
        if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
             m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
                (!m_ProcessRunning)) {
            //Edit Mode
            mp_Ui->btnEdit->setEnabled(true);

        }
        else {
            mp_Ui->btnEdit->setEnabled(false);
        }
    }
}

/****************************************************************************/
/*!
 *  \brief This slot is called when User Role changes
 */
/****************************************************************************/
void CReagentStationWidget::OnUserRoleChanged()
{
    m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    m_UserRoleChanged = true;
}

/****************************************************************************/
/*!
 *  \brief This slot is called when Process state changes
 */
/****************************************************************************/
void CReagentStationWidget::OnProcessStateChanged()
{
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
         m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
            (!m_ProcessRunning)) {
        //Edit Mode
        bool m_IsLeicaReagent = m_SelectedReagent.IsLeicaReagent();
        if(m_IsLeicaReagent) {
            mp_Ui->btnEdit->setEnabled(false);
        }
        else {
            mp_Ui->btnEdit->setEnabled(true);
        }
    }
    else {
        mp_Ui->btnEdit->setEnabled(false);

        if(m_ProcessRunning && m_ShowMessageDialog){
            m_MessageDlg.SetTitle(tr("Information Message"));
            m_MessageDlg.SetIcon(QMessageBox::Information);
            m_MessageDlg.SetButtonText(1, tr("Ok"));
            m_MessageDlg.HideButtons();
            (void)m_MessageDlg.exec();
        }
    }
}

/*****************************************************************************/
/*!
  *  \brief Translates the strings in UI to the selected language
  */
/****************************************************************************/
void CReagentStationWidget::RetranslateUI()
{
    MainMenu::CPanelFrame::SetPanelTitle(QApplication::translate("Reagent Station::CReagentStatusWidget",
                                                                 "Station", 0, QApplication::UnicodeUTF8));
    (void) m_ReagentStationModel.setHeaderData(0, Qt::Horizontal,QApplication::translate("Core::CReagentStatusModel",
                                                                                 "Station", 0, QApplication::UnicodeUTF8),0);

    (void) m_ReagentStationModel.setHeaderData(1, Qt::Horizontal,QApplication::translate("Core::CReagentStatusModel",
                                                                                 "Reagent", 0, QApplication::UnicodeUTF8),0);

}

/****************************************************************************/
/*!
 *  \brief This event is called whenever widget is shown
 */
/****************************************************************************/
void CReagentStationWidget::showEvent(QShowEvent *)
{
    m_MessageDlg.SetText(tr("Staining Process has started, Editing is no longer possible."));
    m_ShowMessageDialog = true ;
    mp_TableWidget->clearSelection();
    mp_Ui->scrollTable->Reset();
    ResetButtons();
}

/****************************************************************************/
/*!
 *  \brief Used to set pointer to mainwindow, used to retreive user role and
 *         process state changed.
 */
/****************************************************************************/
void CReagentStationWidget::SetPtrToMainWindow(Core::CDataConnector *p_DataConnector,
                                         DataManager::CDataReagentList *p_ReagentList,
                                         MainMenu::CMainWindow *p_MainWindow,
                                         KeyBoard::CKeyBoard *p_KeyBoard)
{
    mp_MainWindow = p_MainWindow;
    mp_DataConnector = p_DataConnector;
    mp_ReagentList = p_ReagentList;
    mp_KeyBoard = p_KeyBoard;
    // set the reagent ist to the model
    m_ReagentStationModel.SetRequiredContainers(mp_ReagentList, mp_DataConnector->ReagentGroupList,
                                               mp_DataConnector->DashboardStationList, 2);
    ResizeHorizontalSection();
    mp_ModifiyReagentStatusDlg = new CModifyReagentStatusDlg(this, p_MainWindow, p_DataConnector);
    mp_ModifiyReagentStatusDlg->setModal(true);
    CONNECTSIGNALSIGNAL(mp_ModifiyReagentStatusDlg, UpdateStationChangeReagent(QString,QString),
                        this, UpdateStationChangeReagent(QString,QString));
    CONNECTSIGNALSIGNAL(mp_ModifiyReagentStatusDlg, UpdateStationSetAsEmpty(QString),
                        this, UpdateStationSetAsEmpty(QString));
}


/****************************************************************************/
/*!
 *  \brief This resets the buttons on the right of the screen
 */
/****************************************************************************/
void CReagentStationWidget::ResetButtons()
{
    mp_Ui->btnEdit->setEnabled(false);
}

void CReagentStationWidget:: StationReagentUpdated()
{
    m_ReagentStationModel.UpdateReagentList();
    m_ReagentStationModel.ResetAndUpdateModel();
}
void CReagentStationWidget:: OnContentScrolled()
{

}

}

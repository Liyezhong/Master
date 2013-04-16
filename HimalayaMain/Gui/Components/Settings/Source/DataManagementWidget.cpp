/****************************************************************************/
/*! \file DataManagementWidget.cpp
 *
 *  \brief DataManagementWidget implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-29
 *   $Author:  $ M.Scherer
 *
 *  \b Company:
 *
 *       Leica Biosystems Nussloch GmbH.
 *
 *  (C) Copyright 2010 by Leica Biosystems Nussloch GmbH. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "Settings/Include/DataManagementWidget.h"
#include "Global/Include/Utils.h"
#include "Global/Include/Exception.h"
#include "ui_DataManagementWidget.h"
#include <QEvent>
#include <QApplication>
namespace Settings {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CDataManagementWidget::CDataManagementWidget(QWidget *p_Parent) : MainMenu::CPanelFrame(p_Parent),mp_Ui(new Ui::CDataManagementWidget)
{
    mp_Ui->setupUi(GetContentFrame());
    // mp_DataManagement = new MainMenu::CDataManagement(this);
    //SetContent(mp_DataManagement->layout());
    SetPanelTitle(tr("Data Management"));
    //CONNECTSIGNALSIGNAL(mp_DataManagement, ExecSending(const QString, const QStringList &), this, ExecSending(const QString, const QStringList &));
    CONNECTSIGNALSLOT(mp_Ui->importButton,clicked(),this,ImportDialog());
    CONNECTSIGNALSLOT(mp_Ui->saveusbButton,clicked(),this,SaveToUSBDialog());
    CONNECTSIGNALSLOT(mp_Ui->serviceExportButton,clicked(),this,ServiceExportDialog());
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CDataManagementWidget::~CDataManagementWidget()
{
    try {
      //  delete mp_DataManagement;
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

void CDataManagementWidget::changeEvent(QEvent *p_Event)
{
    QWidget::changeEvent(p_Event);
    switch (p_Event->type()) {
        case QEvent::LanguageChange:
            RetranslateUI();
            break;
        default:
            break;
    }
}

void CDataManagementWidget:: ServiceExportDialog()
{
    QStringList Type;
    Type << "User";
    emit ExecSending("ServiceExport", Type);
}
void CDataManagementWidget:: SaveToUSBDialog()
{
    QStringList Type;
    Type << "User";
    emit ExecSending("UserExport", Type);
}
void CDataManagementWidget:: ImportDialog()
{
    QStringList Type;
    Type << "User";
    emit ExecSending("DataImport", Type);
}

/****************************************************************************/
/*!
 *  \brief Enables/Disables the button based on the user role/process
 *         running status
 */
/****************************************************************************/
void CDataManagementWidget::ResetButtons()
{
    m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
    m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();
    if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin && (!m_ProcessRunning))) {
        //Edit Mode
        mp_Ui->saveusbButton->setEnabled(true);
    }
    else {
        mp_Ui->saveusbButton->setEnabled(false);
    }
    if (( m_CurrentUserRole == MainMenu::CMainWindow::Service) && (!m_ProcessRunning)) {
        mp_Ui->serviceExportButton->setEnabled(true);
    }
    else {
        mp_Ui->serviceExportButton->setEnabled(false);
    }
    if ((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
         m_CurrentUserRole == MainMenu::CMainWindow::Service) && (!m_ProcessRunning)) {
        mp_Ui->importButton->setEnabled(true);
    }
    else {
        mp_Ui->importButton->setEnabled(false);
    }
}


/****************************************************************************/
/*!
 *  \brief Called when DataManagement widget is show
 */
/****************************************************************************/
void CDataManagementWidget::showEvent(QShowEvent *p_Event)
{
    Q_UNUSED(p_Event);
    ResetButtons();
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CDataManagementWidget::RetranslateUI()
{
   MainMenu::CPanelFrame::SetPanelTitle(QApplication::translate("Settings::CDataManagementWidget", "Data Management", 0, QApplication::UnicodeUTF8));
}

/****************************************************************************/
/*!
 *  \brief Used to set pointer to mainwindow, used to retreive user role and
 *         process state changed.
 */
/****************************************************************************/
void CDataManagementWidget::SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow)
{
    mp_MainWindow = p_MainWindow;
   // mp_DataManagement->SetPtrToMainWindow(mp_MainWindow);
}

} //end of namespace

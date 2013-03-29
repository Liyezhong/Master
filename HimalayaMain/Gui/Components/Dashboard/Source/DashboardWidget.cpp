/****************************************************************************/
/*! \file Dashboard/Source/DashboardWidget.cpp
 *
 *  \brief Implementation file for class DashboardWidget.
 *
 *  $Version:   $ 0.2
 *  $Date:      $ 2013-01-03, 08-Feb-2013
 *  $Author:    $ Swati Tiwari, Santhosha Kumar Raju
 *
 *  \b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2012 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#include "Global/Include/Utils.h"
#include "Dashboard/Include/DashboardWidget.h"
#include "ui_DashboardWidget.h"
#include <QDebug>

namespace Dashboard {

CDashboardWidget::CDashboardWidget(Core::CDataConnector *p_DataConnector,
                                   MainMenu::CMainWindow *p_Parent): MainMenu::CPanelFrame(p_Parent),
                                   mp_Ui(new Ui::CDashboardWidget),mp_MainWindow(p_Parent),
                                   mp_DataConnector(p_DataConnector),
                                   m_Operation(Dashboard::OP_PLAY),
                                   m_UserRoleChanged(false)
{
     mp_Ui->setupUi(GetContentFrame());
     SetPanelTitle(tr("Dashboard"));

     QPalette Palette = mp_Ui->dashboardView->palette();
     QColor BaseColor = Palette.color(QPalette::Base);
     BaseColor.setAlpha(0);
     Palette.setColor(QPalette::Base, BaseColor);
     mp_Ui->dashboardView->setPalette(Palette);

     mp_DashboardScene = new CDashboardScene(mp_DataConnector, this, mp_MainWindow);
     mp_Ui->dashboardView->setScene(mp_DashboardScene);

     mp_Separator = new QFrame();
     mp_Separator->setParent(this);  // Set Parent of this Frame as the Dashboard Widget.
     DrawSeparatorLine();

     m_btnGroup.addButton(mp_Ui->playButton, Dashboard::firstButton);
     m_btnGroup.addButton(mp_Ui->abortButton, Dashboard::secondButton);

     m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();
     mp_MessageDlg = new MainMenu::CMessageDlg();

     AddItemsToComboBox();


     CONNECTSIGNALSLOT(mp_MainWindow, UserRoleChanged(), this, OnUserRoleChanged());
     CONNECTSIGNALSLOT(mp_Ui->pgmsComboBox, currentIndexChanged(int), mp_Ui->pgmsComboBox, handleSelectionChanged(int));

     CONNECTSIGNALSLOT(&m_btnGroup, buttonClicked(int), this, OnButtonClicked(int));
     CONNECTSIGNALSLOT(this, ProgramAction(const QString&, DataManager::ProgramActionType_t),
                       mp_DataConnector, SendProgramAction(const QString&, DataManager::ProgramActionType_t));

}

CDashboardWidget::~CDashboardWidget()
{
    try {
        delete mp_Separator;
        delete mp_DashboardScene;
        delete mp_Ui;
    } catch(...) {

    }
}

void CDashboardWidget::DrawSeparatorLine()
{

    mp_Separator->setFrameShape(QFrame::HLine);
    mp_Separator->setLineWidth(2);
    mp_Separator->setFrameShadow(QFrame::Plain);
    mp_Separator->setGeometry(4,460,685,2);
    mp_Separator->show();
}

void CDashboardWidget::AddItemsToComboBox()
{
    mp_Ui->pgmsComboBox->insertItem(0, QIcon(":/HimalayaImages/Icons/MISC/Icon_Leica.png"), "LeicaPrgm1");
    mp_Ui->pgmsComboBox->insertItem(1, QIcon(":/HimalayaImages/Icons/MISC/Icon_Leica.png"), "LeicaPrgm2");
    mp_Ui->pgmsComboBox->insertItem(2, QIcon(":/HimalayaImages/Icons/MISC/Icon_Leica.png"), "LeicaPrgm3");
    mp_Ui->pgmsComboBox->insertItem(3, QIcon(":/HimalayaImages/Icons/MISC/TickOk.png"), "UserPrgm1");
    mp_Ui->pgmsComboBox->insertItem(4, QIcon(":/HimalayaImages/Icons/MISC/TickOk.png"), "UserPrgm2");
    mp_Ui->pgmsComboBox->insertItem(5, QIcon(":/HimalayaImages/Icons/MISC/TickOk.png"), "UserPrgm3");
}

/****************************************************************************/
/*!
 *  \brief This slot is called when User Role changes
 */
/****************************************************************************/
void CDashboardWidget::OnUserRoleChanged()
{
    m_CurrentUserRole = MainMenu::CMainWindow::GetCurrentUserRole();
    m_UserRoleChanged = true;
}


void CDashboardWidget::OnButtonClicked(int whichBtn)
{
    if ( whichBtn == Dashboard::firstButton ) {
        switch(m_Operation)
        {
            case Dashboard::OP_PLAY:
            {
                qDebug() << " I am at Play";
                mp_Ui->playButton->setIcon(QIcon(":/HimalayaImages/Icons/Dashboard/Operation/Operation_Pause.png"));
                m_Operation = Dashboard::OP_PAUSE;
                CheckPreConditionsToRunProgram();
            }
            break;
            case Dashboard::OP_PAUSE:
            {
                qDebug() << " I am at Pause";

                mp_Ui->playButton->setIcon(QIcon(":/HimalayaImages/Icons/Dashboard/Operation/Operation_Start_Resume.png"));
                m_Operation = Dashboard::OP_PLAY;
                CheckPreConditionsToPauseProgram();

            }
            break;
        }
    }
    else if (whichBtn == Dashboard::secondButton)
    {
        qDebug() << " I am at Abort";
        CheckPreConditionsToAbortProgram();
    }

}

void CDashboardWidget::OnRMSValueChanged(Global::RMSOptions_t state)
{
    m_RMSState = state;
}

void CDashboardWidget::CheckPreConditionsToRunProgram()
{
    if((m_CurrentUserRole == MainMenu::CMainWindow::Admin ||
        m_CurrentUserRole == MainMenu::CMainWindow::Service) &&
            Global::RMS_OFF != m_RMSState)
    {
        qDebug() << "RMS IS ON && User = Admin | Service";
        mp_MessageDlg->SetIcon(QMessageBox::Warning);
        mp_MessageDlg->SetTitle(tr("Warning"));
        mp_MessageDlg->SetText(tr("Do you want to Start the Program with Expired Reagents."));
        mp_MessageDlg->SetButtonText(3, tr("Yes"));
        mp_MessageDlg->SetButtonText(2, tr("No"));
        mp_MessageDlg->SetButtonText(1, tr("Cancel"));
        mp_MessageDlg->Show();
        CONNECTSIGNALSLOT(mp_MessageDlg, ButtonLeftClicked(), this, OnPressYes());
        CONNECTSIGNALSLOT(mp_MessageDlg, ButtonCenterClicked(), this, OnPressNo());
        CONNECTSIGNALSLOT(mp_MessageDlg, ButtonRightClicked(), this, OnPressCancel());

    }

}

void CDashboardWidget::CheckPreConditionsToPauseProgram()
{

}


void CDashboardWidget::CheckPreConditionsToAbortProgram()
{

}

void CDashboardWidget::RunProgram()
{
    // Send command to Master
}

void CDashboardWidget::PauseProgram()
{
    // Send command to Master
}

void CDashboardWidget::AbortProgram()
{
    // Send command to Master
}

void CDashboardWidget::OnPressYes()
{
    qDebug() << " Start the Program"; // to do
    QString ProgID;
    emit ProgramAction(ProgID, DataManager::PROGRAM_START);
}

void CDashboardWidget::OnPressNo()
{
    qDebug() << " On NO "; // to do
}

void CDashboardWidget::OnPressCancel()
{
    qDebug() << " On Cancel"; // to do
}

} // End of namespace Dashboard

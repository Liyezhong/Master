/****************************************************************************/
/*! \file ReagentWidget.cpp.cpp
 *
 *  \brief ReagentWidget Implementation.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-10-22
 *   $Author:  $ Abe Yang
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

#include "Dashboard/Include/DashboardWidget.h"
#include "ui_DashboardWidget.h"
#include "Core/Include/DataConnector.h"
#include "MainMenu/Include/MainWindow.h"
//#include "HimalayaDataContainer/Containers/Programs/Include/DataProgramList.h"

using namespace Dashboard;

CDashboardWidget::CDashboardWidget(Core::CDataConnector *p_DataConnector,
                                     MainMenu::CMainWindow *p_Parent) :
    ui(new Ui::CDashboardWidget),
    mp_DataConnector(p_DataConnector),
    mp_MainWindow(p_Parent)
{
    ui->setupUi(this);
    ui->containerPanelWidget->SetPtrToMainWindow(mp_MainWindow, mp_DataConnector);
    ui->programPanelWidget->SetPtrToMainWindow(mp_MainWindow, mp_DataConnector);
    mp_ProgramList = mp_DataConnector->ProgramList;
    CONNECTSIGNALSIGNAL(this, AddItemsToFavoritePanel(bool), ui->programPanelWidget, AddItemsToFavoritePanel(bool));
    CONNECTSIGNALSLOT(ui->programPanelWidget, PrepareSelectedProgramChecking(), this, PrepareSelectedProgramChecking());
}

CDashboardWidget::~CDashboardWidget()
{
    try {
            delete ui;
        }
        catch (...) {
            // to please Lint.
        }
}

void CDashboardWidget::OnUnselectProgram()
{
    ui->containerPanelWidget->OnUnselectProgram();
}

void CDashboardWidget::PrepareSelectedProgramChecking()
{
    //(void)this->IsParaffinInProgram(mp_ProgramList->GetProgram(m_NewSelectedProgramId));//to get m_ParaffinStepIndex
    //Notify Master, to get the time costed for paraffin Melting
    /*QString strTempProgramId(m_NewSelectedProgramId);
    if (m_NewSelectedProgramId.at(0) == 'C')
    {
      strTempProgramId.append("_");
      QString strReagentIDOfLastStep = m_pUserSetting->GetReagentIdOfLastStep();
      strTempProgramId.append(strReagentIDOfLastStep);
    }
    mp_DataConnector->SendProgramSelected(strTempProgramId, m_ParaffinStepIndex);*/

}


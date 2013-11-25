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

#include "Dashboard/Include/DashboardWidget2.h"
#include "ui_DashboardWidget2.h"
#include "Core/Include/DataConnector.h"
#include "MainMenu/Include/MainWindow.h"

namespace Dashboard {

CDashboardWidget2::CDashboardWidget2(Core::CDataConnector *p_DataConnector,
                                     MainMenu::CMainWindow *p_Parent) :
    ui(new Ui::CDashboardWidget2),
    mp_DataConnector(p_DataConnector),
    mp_MainWindow(p_Parent)
{
    ui->setupUi(this);
    ui->containerPanelWidget->SetPtrToMainWindow(mp_MainWindow, mp_DataConnector);
    ui->programPanelWidget->SetPtrToMainWindow(mp_MainWindow, mp_DataConnector);

    CONNECTSIGNALSIGNAL(this, AddItemsToFavoritePanel(bool), ui->programPanelWidget, AddItemsToFavoritePanel(bool));

}

CDashboardWidget2::~CDashboardWidget2()
{
    try {
            delete ui;
        }
        catch (...) {
            // to please Lint.
        }
}

void CDashboardWidget2::OnUnselectProgram()
{
    ui->containerPanelWidget->OnUnselectProgram();
}

}//end


/****************************************************************************/
/*! \file DashboardWidget.h
 *
 *  \brief Dashboard Widget Class Definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-01-03
 *   $Author:  $ Swati Tiwari
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

#ifndef DASHBOARDWIDGET_H
#define DASHBOARDWIDGET_H

#include "MainMenu/Include/PanelFrame.h"
#include "MainMenu/Include/MainWindow.h"
#include "Dashboard/Include/DashboardScene.h"
#include "Dashboard/Include/DashboardEndTimeWidget.h"
#include "Dashboard/Include/DashboardComboBox.h"
#include <QFrame>
#include <QGridLayout>

namespace Dashboard {

namespace Ui {
    class CDashboardWidget;
}

/****************************************************************************/
/**
 * \brief This is the main widget for Dash Board.
 */
/****************************************************************************/
class CDashboardWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT

private:
    Ui::CDashboardWidget *mp_Ui;                                //!< User interface
    MainMenu::CMainWindow *mp_Parent;                           //!< Reference to MainWindow.
    Core::CDataConnector *mp_DataConnector;                     //!< Global data container
    CDashboardScene *mp_DashboardScene;                         //!< Displays the station items
    QFrame *mp_Separator;                                       //!< Separator Line between View & Operation

public:
    explicit CDashboardWidget(Core::CDataConnector *p_DataConnector, MainMenu::CMainWindow *p_Parent = NULL);

    ~CDashboardWidget();

protected:
    void DrawSeparatorLine();
    void AddItemsToComboBox();

};

}
#endif // DASHBOARDWIDGET_H

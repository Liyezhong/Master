/****************************************************************************/
/*! \file DashboardDateTimeWidget.h
 *
 *  \brief DashboardDateTimeWidget definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 27-Mar-2013
 *   $Author:  $ Santhosha Kumar Raju
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

#ifndef DASHBOARD_DATETIMEWIDGET_H
#define DASHBOARD_DATETIMEWIDGET_H

#include  "DataManager/Containers/UserSettings/Include/UserSettings.h"
#include "MainMenu/Include/MainWindow.h"
#include "MainMenu/Include/DateTime.h"
#include "MainMenu/Include/PanelFrame.h"
#include <QShowEvent>

namespace Dashboard {

/****************************************************************************/
/**
 * \brief Frame for the date and time widget
 */
/****************************************************************************/
class CDashboardDateTimeWidget : public MainMenu::CPanelFrame {
    Q_OBJECT

public:
    explicit CDashboardDateTimeWidget(QWidget *p_Parent = 0);
    virtual ~CDashboardDateTimeWidget();
    //void SetUserSettings(DataManager::CUserSettings *p_UserSettings);
    //void SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow);
    MainMenu::CDateTime *GetContent();

protected:
    //void showEvent(QShowEvent *p_Event);
    //void changeEvent(QEvent *p_Event);

private:
    MainMenu::CDateTime *mp_DateTime;   //!< Content of this widget
    DataManager::CUserSettings *mp_UserSettings;  //!< Data object
    //MainMenu::CMainWindow *mp_MainWindow;           //!< Pointer to MainWindow
   // void RetranslateUI();
};

} // end namespace Dashboard

#endif // DASHBOARD_DATETIMEWIDGET_H

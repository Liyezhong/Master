/****************************************************************************/
/*! \file DashboardDateTimeWidget.cpp
 *
 *  \brief DashboardDateTimeWidget implementation.
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

#include "Global/Include/Utils.h"
#include "Global/Include/Exception.h"
#include "Dashboard/Include/DashboardDateTimeWidget.h"
#include <QDateTime>
#include <QApplication>
#include <QEvent>

namespace Dashboard {

/****************************************************************************/
/*!
 *  \brief Constructor
 *
 *  \iparam p_Parent = Parent object
 */
/****************************************************************************/
CDashboardDateTimeWidget::CDashboardDateTimeWidget(QWidget *p_Parent) : MainMenu::CPanelFrame(p_Parent), mp_UserSettings(NULL)
{
    mp_DateTime = new MainMenu::CDateTime();
    SetPanelTitle(tr("Date/Time"));
    SetContent(mp_DateTime->layout());
}

/****************************************************************************/
/*!
 *  \brief Destructor
 */
/****************************************************************************/
CDashboardDateTimeWidget::~CDashboardDateTimeWidget()
{
    try {
        delete mp_DateTime;
    }
    catch (...) {
        // to please Lint.
    }
}
#if 0

/****************************************************************************/
/*!
 *  \brief Event handler for change events
 *
 *  \iparam p_Event = Change event
 */
/****************************************************************************/

void CDashboardDateTimeWidget::changeEvent(QEvent *p_Event)
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

#endif
/****************************************************************************/
/*!
 *  \brief  Returns the content of this panel
 *
 *  \return A pointer to the content
 */
/****************************************************************************/
MainMenu::CDateTime *CDashboardDateTimeWidget::GetContent()
{
    return mp_DateTime;
}

#if 0
/****************************************************************************/
/*!
 *  \brief Sets the settings data object for this class
 *
 *  \iparam p_UserSettings = Global user settings data
 */
/****************************************************************************/
void CDashboardDateTimeWidget::SetUserSettings(DataManager::CUserSettings *p_UserSettings)
{
    mp_UserSettings = p_UserSettings;
}

/****************************************************************************/
/*!
 *  \brief Updates the widget content everytime it is displayed
 *
 *  \iparam p_Event = Show event
 */
/****************************************************************************/
void CDashboardDateTimeWidget::showEvent(QShowEvent *p_Event) {
    if((mp_DateTime != NULL) && (mp_UserSettings != NULL) && (p_Event != NULL) && !p_Event->spontaneous()) {
        // widget is quite before to be shown (again) Refresh contents.
        mp_DateTime->RefreshDateTime(mp_UserSettings->GetTimeFormat());
        mp_DateTime->ResetButtons();
    }
}

/****************************************************************************/
/*!
 *  \brief Translates the strings in UI to the selected language
 */
/****************************************************************************/
void CDashboardDateTimeWidget::RetranslateUI()
{
   MainMenu::CPanelFrame::SetPanelTitle(QApplication::translate("Settings::CDashboardDateTimeWidget", "Date/Time", 0, QApplication::UnicodeUTF8));
}

/****************************************************************************/
/*!
 *  \brief Used to set pointer to mainwindow, used to retreive user role and
 *         process state changed.
 */
/****************************************************************************/
void CDashboardDateTimeWidget::SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow)
{
    mp_MainWindow = p_MainWindow;
    mp_DateTime->SetPtrToMainWindow(mp_MainWindow);
}
#endif

} // end namespace Dashboard

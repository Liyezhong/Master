/****************************************************************************/
/*! \file DateTimeWidget.h
 *
 *  \brief DateTimeWidget definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2011-09-21
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

#ifndef SETTINGS_DATETIMEWIDGET_H
#define SETTINGS_DATETIMEWIDGET_H

#include  "DataManager/Containers/UserSettings/Include/UserSettings.h"
#include "MainMenu/Include/MainWindow.h"
#include "MainMenu/Include/DateTime.h"
#include "MainMenu/Include/PanelFrame.h"
#include <QShowEvent>

namespace Settings {

/****************************************************************************/
/**
 * \brief Frame for the date and time widget
 */
/****************************************************************************/
class CDateTimeWidget : public MainMenu::CPanelFrame {
    Q_OBJECT

public:
    explicit CDateTimeWidget(QWidget *p_Parent = 0);
    virtual ~CDateTimeWidget();
    void SetUserSettings(DataManager::CUserSettings *p_UserSettings);
    void SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow);
    MainMenu::CDateTime *GetContent();

protected:
    void showEvent(QShowEvent *p_Event);
    void changeEvent(QEvent *p_Event);

private:
    MainMenu::CDateTime *mp_DateTime;   //!< Content of this widget
    DataManager::CUserSettings *mp_UserSettings;  //!< Data object
    MainMenu::CMainWindow *mp_MainWindow;           //!< Pointer to MainWindow
    void RetranslateUI();
};

} // end namespace Settings

#endif // SETTINGS_DATETIMEWIDGET_H

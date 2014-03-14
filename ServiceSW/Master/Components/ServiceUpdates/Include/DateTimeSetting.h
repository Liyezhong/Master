/****************************************************************************/
/*! \file DateTimeSetting.h
 *
 *  \brief Declaration of Date/Time setting class.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-06-18
 *   $Author:  $ R.Wu
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#ifndef SERVICEUPDATES_DATETIMESETTING_H
#define SERVICEUPDATES_DATETIMESETTING_H

#include "MainMenu/Include/PanelFrame.h"

#include <QDateTime>

#include "DataManager/Containers/UserSettings/Include/UserSettings.h"


namespace ServiceUpdates {

namespace Ui {
class CDateTimeSetting;
}

class CDateTimeSetting : public MainMenu::CPanelFrame
{
    Q_OBJECT
    
public:
    explicit CDateTimeSetting(QWidget *parent = 0);

    ~CDateTimeSetting(void);

    void SetUserSettings(DataManager::CUserSettings *p_UserSettings);

Q_SIGNALS:
    void DateTimeChanged(QDateTime);

protected:
    void changeEvent(QEvent *p_Event);
    
private:
    void RetranslateUI(void);

    Ui::CDateTimeSetting       *ui;

    DataManager::CUserSettings *mp_UserSettings;
};


} // namespace ServiceUpdates
#endif // SERVICEUPDATES_DATETIMESETTING_H

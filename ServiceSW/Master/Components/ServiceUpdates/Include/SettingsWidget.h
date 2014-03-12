/****************************************************************************/
/*! \file SettingsWidget.h
 *
 *  \brief Declaration of Settings widget class.
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

#ifndef SERVICEUPDATES_SETTINGSWIDGET_H
#define SERVICEUPDATES_SETTINGSWIDGET_H

#include <QWidget>
#include <QDateTime>
#include <QString>
#include <QStringList>

#include "Core/Include/ServiceGUIConnector.h"


namespace ServiceUpdates {

namespace Ui {
    class CSettingsWidget;
}

/****************************************************************************/
/**
 * \brief
 */
/****************************************************************************/
class CSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CSettingsWidget(Core::CServiceGUIConnector &DataConnector,
                             QWidget *parent = 0);

    ~CSettingsWidget(void);

    void UpdateSupportedGUILanguages(void);

    const QStringList &GetLanguageList(void)
    {
        return m_LanguageList;
    }

Q_SIGNALS:
    void DateTimeChanged(QDateTime);

    void UserSettingsChanged(DataManager::CUserSettings &Settings);

private Q_SLOTS:
    void UpdateGUI(void);

    void TabIndexChanged(int);

private:
    Ui::CSettingsWidget *mp_Ui;

    QStringList          m_LanguageList;

    Core::CServiceGUIConnector *mp_DateConnector;
};

} // end namespace ServiceUpdates

#endif //SERVICEUPDATES_SETTINGSWIDGET_H

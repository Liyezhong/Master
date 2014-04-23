/****************************************************************************/
/*! \file SettingsWidget.h
 *
 *  \brief SettingsWidget definition.
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

#ifndef SETTINGS_SETTINGSWIDGET_H
#define SETTINGS_SETTINGSWIDGET_H

#include "Core/Include/DataConnector.h"
#include "MainMenu/Include/MainWindow.h"
#include "KeyBoard/Include/KeyBoard.h"

namespace Settings {

namespace Ui {
    class CSettingsWidget;
}

/****************************************************************************/
/**
 * \brief This is the main widget of the settings module
 */
/****************************************************************************/
class CSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CSettingsWidget(Core::CDataConnector *p_Data, MainMenu::CMainWindow *p_Parent,
                             KeyBoard::CKeyBoard *p_Keyboard);
    virtual ~CSettingsWidget();

protected:
    void changeEvent(QEvent *p_Event);

private:
    Ui::CSettingsWidget *mp_Ui;             //!< User interface
    Core::CDataConnector *mp_Data;          //!< Data object
    MainMenu::CMainWindow *mp_MainWindow;   //!< Used to inform Main window when settings change.
    KeyBoard::CKeyBoard *mp_KeyBoardWidget; //!< Keyboard object
    void RetranslateUI();

private slots:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of PropagateDateTime
     */
    /****************************************************************************/
    void PropagateDateTime();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of UserSettingsUpdated
     */
    /****************************************************************************/
    void UserSettingsUpdated();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of PanelSelected
     */
    /****************************************************************************/
    void PanelSelected(int Index);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of UpdateLanguages
     */
    /****************************************************************************/
    void UpdateLanguages();

signals:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of signal SettingsChanged
     */
    /****************************************************************************/
    void SettingsChanged(DataManager::CUserSettings &Settings);
};

} // end namespace Settings

#endif // SETTINGS_SETTINGSWIDGET_H


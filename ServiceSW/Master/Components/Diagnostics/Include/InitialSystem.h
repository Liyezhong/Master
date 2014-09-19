/****************************************************************************/
/*! \file InitialSystem.h
 *
 *  \brief Declaration of Initial system check.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-09-10
 *   $Author:  $ Dixiong Li
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

#ifndef DIAGNOSTICS_INITIALSYSTEM_H
#define DIAGNOSTICS_INITIALSYSTEM_H

#include "MainMenu/Include/DialogFrame.h"
#include "Core/Include/ServiceGUIConnector.h"
#include "Core/Include/ServiceDefines.h"

namespace Diagnostics {

namespace Ui {
class CInitialSystem;
}

namespace InitialSystem {
class CInitialSystemCheck;
}

class CInitialSystem : public MainMenu::CDialogFrame
{
    Q_OBJECT

public:
    explicit CInitialSystem(Core::CServiceGUIConnector *p_DataConnector = NULL, QWidget *parent = 0);
    ~CInitialSystem();

protected:
    void changeEvent(QEvent *p_Event);

signals:
    void RetortPreHeating();

private Q_SLOTS:
    void StartCheck(void);
    void OnRefreshStatus(Service::InitialSystemTestType Type, int Ret);   
    void OnRefreshHeatingStatus(Service::InitialSystemTestType Type);

    void UpdateHeatingStatus();

private:
    void UpdateOvenHeatingStatus();
    void UpdateLHeatingStatus();
    void UpdateRVHeatingStatus();
    void UpdateRetortStatus();

private:
    //Core::CServiceGUIConnector *mp_DataConnector;   //!< Data Connector object
    MainMenu::CMessageDlg      *mp_WaitDlg;
    Ui::CInitialSystem         *mp_Ui;
    QTimer                     *mp_StartTimer;
    QTimer                     *mp_HeatingTimer;

    InitialSystem::CInitialSystemCheck *mp_InitialSystemCheck;

    bool                       m_IsHeatingTimerStart;
    bool                       m_OvenHeating;
    bool                       m_LiquidHeating;
    bool                       m_RVHeating;
    bool                       m_RetortHeating;

};


} // namespace Diagnostics
#endif // DIAGNOSTICS_INITIALSYSTEM_H


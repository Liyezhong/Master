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

private Q_SLOTS:
    void StartCheck(void);
    void OnRefreshStatus(Service::InitialSystemTestType Type, int Ret);

    void UpdateOvenTestStatus();
    void UpdateLiquidTestStatus();
    void UpdateRVTestStatus();
    void UpdateRetortTestStatus();

private:
    //Core::CServiceGUIConnector *mp_DataConnector;   //!< Data Connector object
    Ui::CInitialSystem         *mp_Ui;
    QTimer*                    mp_StartTimer;
    QPixmap                    m_PixmapCheck;
    QPixmap                    m_PixmapPass;
    QPixmap                    m_PixmapFail;

    InitialSystem::CInitialSystemCheck *mp_InitialSystemCheck;

};


} // namespace Diagnostics
#endif // DIAGNOSTICS_INITIALSYSTEM_H


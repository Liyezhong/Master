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

/****************************************************************************/
/**
 * \brief This dialog is start to Initial system check.
 */
/****************************************************************************/
class CInitialSystem : public MainMenu::CDialogFrame
{
    Q_OBJECT

public:
    /****************************************************************************/
    /*!
     *  \brief Constructor
     *  \iparam p_DataConnector = Service GUI connector object
     *  \iparam parent = Parent widget
     */
    /****************************************************************************/
    explicit CInitialSystem(Core::CServiceGUIConnector *p_DataConnector = NULL, QWidget *parent = 0);

    /****************************************************************************/
    /*!
     *  \brief Destructor
     */
    /****************************************************************************/
    ~CInitialSystem();

protected:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function changeEvent
     *  \param p_Event = QEvent type parameter
     *  \return from changeEvent
     */
    /****************************************************************************/
    void changeEvent(QEvent *p_Event);

signals:
    /****************************************************************************/
    /*!
     *  \brief Signal emitted for pre heating retort
     */
    /****************************************************************************/
    void RetortPreHeating();

private Q_SLOTS:
    /****************************************************************************/
    /*!
     *  \brief Slot for initial system check start
     */
    /****************************************************************************/
    void StartCheck(void);

    /****************************************************************************/
    /*!
     *  \brief Slot for refresh pre test status
     *  \iparam Type = The pre test module type
     *  \Ret  = The pre test result
     */
    /****************************************************************************/
    void OnRefreshStatus(Service::InitialSystemTestType Type, int Ret);

    /****************************************************************************/
    /*!
     *  \brief Slot for refresh pre heating test status
     *  \iparam Type = the pre heating module type
     */
    /****************************************************************************/
    void OnRefreshHeatingStatus(Service::InitialSystemTestType Type);

    /****************************************************************************/
    /*!
     *  \brief Slot for refresh heating test status
     */
    /****************************************************************************/
    void UpdateHeatingStatus();

private:
    /****************************************************************************/
    /*!
     *  \brief To refresh oven heating status
     */
    /****************************************************************************/
    void UpdateOvenHeatingStatus();

    /****************************************************************************/
    /*!
     *  \brief To refresh Liquid heating tube status
     */
    /****************************************************************************/
    void UpdateLHeatingStatus();

    /****************************************************************************/
    /*!
     *  \brief To refresh Rotary valve heating status
     */
    /****************************************************************************/
    void UpdateRVHeatingStatus();

    /****************************************************************************/
    /*!
     *  \brief To refresh retort heating status
     */
    /****************************************************************************/
    void UpdateRetortStatus();

private:
    //Core::CServiceGUIConnector *mp_DataConnector;   //!< Data Connector object
    MainMenu::CMessageDlg      *mp_WaitDlg;                     //!< Displayed when busy
    Ui::CInitialSystem         *mp_Ui;                          //!< User Interface
    QTimer                     *mp_StartTimer;                  //!< Timeout for start timer
    QTimer                     *mp_HeatingTimer;                //!< Timeout for refresh heating status

    InitialSystem::CInitialSystemCheck *mp_InitialSystemCheck;  //!< To run all initial system check

    bool                       m_IsHeatingTimerStart;           //!< The flag for heatingTimer is start
    bool                       m_OvenHeating;                   //!< The flag for oven heating
    bool                       m_LiquidHeating;                 //!< The flag for liquid heating tube
    bool                       m_RVHeating;                     //!< The flag for rotary valve heating
    bool                       m_RetortHeating;                 //!< the flag for retort heating

};


} // namespace Diagnostics
#endif // DIAGNOSTICS_INITIALSYSTEM_H


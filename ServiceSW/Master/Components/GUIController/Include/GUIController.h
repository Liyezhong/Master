/****************************************************************************/
/*! \file Include/GUIController.h
 *
 *  \brief  Definition of class SoftSwitchManager
 *
 *  Version:    0.1
 *  Date:       2013-02-14
 *  Author:     Srivathsa HH
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
#ifndef GUICONTROLLER_H
#define GUICONTROLLER_H
//QT Headers
#include <QObject>
#include <stdio.h>

//Project Headers
#include <Global/Include/GlobalDefines.h>
#include <Threads/Include/ThreadController.h>
#include <Global/Include/Commands/AckOKNOK.h>


namespace GUIControl {
/****************************************************************************/
/**
 * \brief This Class shall be responsible to manage GUI related
 *        events and updation of GUI
 */
/****************************************************************************/
class GUIController : public Threads::ThreadController {
    Q_OBJECT

public:
    GUIController(Global::gSourceType TheHeartBeatSource);
    virtual ~GUIController();
    virtual void CreateAndInitializeObjects();
    virtual void CleanupAndDestroyObjects();

    /****************************************************************************/
    /**
      * \brief Send command to external process.
      *
      * Send a command to the external process. Reference mapping does not
      * have to be done since the acknowledge we receive from the external
      * process corresponds with Ref.
      *
      * \param[in]   Ref                 Command reference.
      * \param[in]   Cmd                 Command to send.
      */
    /****************************************************************************/
    void SendCommand(Global::tRefType Ref, const Global::CommandShPtr_t &Cmd);
    Global::tRefType GetNewCommandRef();

protected:
    void OnGoReceived();
    void OnStopReceived();
    virtual void OnPowerFail();

private:
    void RegisterCommands();
    void OnAcknowledge(Global::tRefType, const Global::AckOKNOK &);

};


} // end namespace GUIControl
#endif // GUICONTROLLER_H

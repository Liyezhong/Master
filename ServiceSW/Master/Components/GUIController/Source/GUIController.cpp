/****************************************************************************/
/*! \file GUIController.cpp
 *
 *  \brief GUIController Implementation
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2013-02-14
 *   $Author:  $ Srivathsa HH
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
//QT Headers
#include <QMetaType>
#include <QDebug>
//Std lib headers

//Project Headers
#include <GUIController/Include/GUIController.h>
#include <Core/Include/Startup.h>
#include <Global/Include/Utils.h>

#include "Application/Include/Application.h"

namespace GUIControl {

/****************************************************************************/
/*!
 *  \brief    Constructor for the GUIController
 *
 * \param[in]   TheHeartBeatSource                 heart beat source type.
 *
 ****************************************************************************/
GUIController::GUIController(Global::gSourceType TheHeartBeatSource)
    : Threads::ThreadController(TheHeartBeatSource, "GUIController"){
}


/****************************************************************************/
/*!
 *  \brief    Destructor for the SoftSwitchManagerThreadController
 *
 *  \return
 *
 ****************************************************************************/
GUIController::~GUIController(){
}

/****************************************************************************/
/*!
 *  \brief    API function to call before EventHandler can be used
 *
 *      This function does everything to initialize the EventHandler. It
 *      shall be called before EventHandler can be used.
 *
 ****************************************************************************/
void GUIController::CreateAndInitializeObjects(){
    // now register commands
    RegisterCommands();
}

/****************************************************************************/
/*!
 *  \brief Destroy all used objects and prepare for shutdown.
 *
 *         See detailed description in the Base Class's header
 *
 *  \return
 */
/****************************************************************************/
void GUIController::CleanupAndDestroyObjects(){
}

/****************************************************************************/
/*!
 *  \brief Register commands handled by thread Controller
 *
 */
/****************************************************************************/
void GUIController::RegisterCommands(){
    RegisterAcknowledgeForProcessing<Global::AckOKNOK, GUIController>
            (&GUIController::OnAcknowledge, this);
}

/****************************************************************************/
/*!
 *  \brief This method is called when the base class receives Go signal.
 *
 *         See detailed description in the Base Class's header
 *
 *  \return
 */
/****************************************************************************/
void GUIController::OnGoReceived(){
    //On Go Received Construct GUI components here...
    qDebug()<<"GUIController::OnGoReceived()";

}

/****************************************************************************/
/*!
 *  \brief This method is called when the base class receives Stop signal.
 *
 *         See detailed description in the Base Class's header
 *
 *  \return
 */
/****************************************************************************/
void GUIController::OnStopReceived(){

}

/****************************************************************************/
/**
 * \brief Power will fail shortly.
 *
 * Power will fail shortly.
 */
/****************************************************************************/
void GUIController::OnPowerFail(){
    /// \todo implement
}

void GUIController::SendCommand(Global::tRefType Ref, const Global::CommandShPtr_t &Cmd){
    qDebug() << "SoftSwitchManagerThreadController::SendCommand" << Ref;
    Threads::ThreadController::SendCommand(Ref, Cmd);
}

Global::tRefType GUIController::GetNewCommandRef(){
    return Threads::ThreadController::GetNewCommandRef();
}

void GUIController::OnAcknowledge(Global::tRefType, const Global::AckOKNOK &){

}


} //End Of namespace GUIControl

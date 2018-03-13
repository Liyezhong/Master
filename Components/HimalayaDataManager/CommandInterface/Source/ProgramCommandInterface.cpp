/****************************************************************************/
/*! \file ProgramCommandInterface.cpp
 *
 *  \brief Program Command Interface  Implementation
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2012-07-30
 *  $Author:    $ N.Kamath
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
#include "HimalayaDataManager/CommandInterface/Include/ProgramCommandInterface.h"
#include "Global/Include/UITranslator.h"


namespace DataManager {
/****************************************************************************/
/*!
 *  \brief Constructor
 *  \iparam p_DataManager = Datamanager object
 *  \iparam p_MasterThreadController = Master thread Cntrl
 *  \iparam p_DataContainer = DataContainer pointer
 */
/****************************************************************************/
CProgramCommandInterface::CProgramCommandInterface(CDataManager *p_DataManager,
                                                   Threads::MasterThreadController *p_MasterThreadController,
                                                   CDataContainer *p_DataContainer):
    CCommandInterfaceBase(p_DataManager, p_MasterThreadController, p_DataContainer)
{
    RegisterCommands();
}

/****************************************************************************/
/**
 * \brief Register Commands related to Program Container
 */
/****************************************************************************/
void CProgramCommandInterface::RegisterCommands() {
    if (mp_MasterThreadController) {
        mp_MasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdProgramUpdate, DataManager::CProgramCommandInterface>
                (&CProgramCommandInterface::UpdateProgram, this);
        mp_MasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdNewProgram, DataManager::CProgramCommandInterface>
                (&CProgramCommandInterface::AddProgram, this);
        mp_MasterThreadController->RegisterCommandForProcessing<MsgClasses::CmdProgramDeleteItem, DataManager::CProgramCommandInterface>
                (&CProgramCommandInterface::DeleteProgram, this);
    }
}

/****************************************************************************/
/**
 * \brief Function which handles CmdNewProgram
 *  \iparam Ref = Command reference
 *  \iparam Cmd = Command
 *  \param AckCommandChannel = Command channel to send acknowledge
 */
/****************************************************************************/
void CProgramCommandInterface::AddProgram(Global::tRefType Ref, const MsgClasses::CmdNewProgram &Cmd, Threads::CommandChannel &AckCommandChannel)
{
    //Create program DataStream. This DataStream will contain program for which color has been replaced
    QByteArray ProgramData(const_cast<QByteArray &>(Cmd.GetProgramData()));
    QDataStream ProgramDataStream(&ProgramData, QIODevice::ReadWrite);
    ProgramDataStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));

    CProgram Program;
    //Streamout DataStream to program object
    ProgramDataStream >> Program;
    Program.SetID(static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ProgramList->GetNextFreeProgID(true));
    bool Result = true;
    static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ProgramList->ResetLastErrors();
    Result = static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ProgramList->AddProgram(&Program);
    if (Result)
    {
        Result = static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ProgramList->Write();
    }
    if (!Result) {
        // If error occurs , get errors and send errors to GUI
        ListOfErrors_t &ErrorList = static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ProgramList->GetErrorList();
        QString ErrorString;
        //Error List was not generated, we inform user that update was not possible
        if (ErrorList.isEmpty()) {
            ErrorString = Global::UITranslator::TranslatorInstance().
                                                Translate(Global::TranslatableString(EVENT_DM_PROG_ADD_FAILED_INTERNAL_ERR,
                                                                                     Global::tTranslatableStringList()));
            Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_ADD_FAILED_INTERNAL_ERR);
        }
        else {
        DataManager::Helper::ErrorIDToString(ErrorList, ErrorString);
        }
        // If error occurs , get errors and send errors to GUI
        //ErrorString = "sdfsdfsdfsdfsafd";
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_ADD_FAILED_INTERNAL_ERR);
        if(mp_MasterThreadController){
            mp_MasterThreadController->SendAcknowledgeNOK(Ref, AckCommandChannel, ErrorString, Global::GUIMSGTYPE_INFO);
        }
        return;
    }
    else {
        (void) ProgramDataStream.device()->reset();
        ProgramDataStream << Program ;
         if(mp_MasterThreadController){
             SendAckAndUpdateGUI(Ref, AckCommandChannel, Global::CommandShPtr_t(
                                     new MsgClasses::CmdNewProgram(10000, ProgramDataStream)));
         }
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_ADD_NEW_PROGRAM,Global::tTranslatableStringList()<<Program.GetName());
    }
}

/****************************************************************************/
/**
 * \brief Function which handles CmdProgramDeleteItem
 *  \iparam Ref = Command reference
 *  \iparam Cmd = Command
 *  \param AckCommandChannel = Command channel to send acknowledge
 */
/****************************************************************************/
void CProgramCommandInterface::DeleteProgram(Global::tRefType Ref, const MsgClasses::CmdProgramDeleteItem &Cmd, Threads::CommandChannel &AckCommandChannel)
{
    bool Result = true;
    QString ProgramName = static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ProgramList->GetProgram(Cmd.GetItemId())->GetName();
    Result = static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ProgramList->DeleteProgram(Cmd.GetItemId());
    if (!Result) {
        // If error occurs , get errors and send errors to GUI
        ListOfErrors_t &ErrorList = static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ProgramList->GetErrorList();
        QString ErrorString;
        //Error List was not generated, we inform user that update was not possible
        if (ErrorList.isEmpty()) {
            ErrorString = Global::UITranslator::TranslatorInstance().
                                                Translate(Global::TranslatableString(EVENT_DM_PROG_DEL_FAILED_INTERNAL_ERR,
                                                                                     Global::tTranslatableStringList()));
            Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_DEL_FAILED_INTERNAL_ERR);
        }
        else {
            DataManager::Helper::ErrorIDToString(ErrorList, ErrorString);
        }
        if(mp_MasterThreadController){
            mp_MasterThreadController->SendAcknowledgeNOK(Ref, AckCommandChannel, ErrorString, Global::GUIMSGTYPE_INFO);
        }
        qDebug()<<"\n\n Delete Program Failed";
    }
    else {
        if(mp_MasterThreadController){
            SendAckAndUpdateGUI(Ref, AckCommandChannel, Global::CommandShPtr_t(
                                    new MsgClasses::CmdProgramDeleteItem(10000, Cmd.GetItemId())));
            Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_REMMOVE_PROGRAM,Global::tTranslatableStringList()<<ProgramName);
        }
    }
    (void)static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ProgramList->Write();


}

/****************************************************************************/
/**
 * \brief Function which handles CmdProgramUpdate
 *  \iparam Ref = Command reference
 *  \iparam Cmd = Command
 *  \param AckCommandChannel = Command channel to send acknowledge
 */
/****************************************************************************/
void CProgramCommandInterface::UpdateProgram(Global::tRefType Ref, const MsgClasses::CmdProgramUpdate &Cmd, Threads::CommandChannel &AckCommandChannel)
{
    //Program for which white color is assigned
    CProgram Program;
    //Program with modified color.This program uses the color previously assigned to above program
//    CProgram ProgramWithNewColor;

    QByteArray ProgramData(const_cast<QByteArray &>(Cmd.GetProgramData()));
    QDataStream ProgramDataStream(&ProgramData, QIODevice::ReadWrite);
    ProgramDataStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));
    (void)ProgramDataStream.device()->reset();
    ProgramDataStream >> Program;


    bool Result = true;
    (static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ProgramList)->ResetLastErrors();
    Result = (static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ProgramList)->UpdateProgram(&Program);
    if (Result)
    {
        Result = static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ProgramList->Write();
    }
    if (!Result) {
        // If error occurs , get errors and send errors to GUI
        ListOfErrors_t &ErrorList = static_cast<DataManager::CDataContainer*>(mp_DataContainer)->ProgramList->GetErrorList();
        QString ErrorString;
        //Error List was not generated, we inform user that update was not possible
        if (ErrorList.isEmpty()) {
            ErrorString = Global::UITranslator::TranslatorInstance().
                                                Translate(Global::TranslatableString(EVENT_DM_PROG_ADD_FAILED_INTERNAL_ERR,
                                                                                     Global::tTranslatableStringList()));
            Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_ADD_FAILED_INTERNAL_ERR);
        }
        else {
        DataManager::Helper::ErrorIDToString(ErrorList, ErrorString);
        }
        // If error occurs , get errors and send errors to GUI
        //ErrorString = "sdfsdfsdfsdfsafd";
        Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_ADD_FAILED_INTERNAL_ERR);
        if(mp_MasterThreadController){
            mp_MasterThreadController->SendAcknowledgeNOK(Ref, AckCommandChannel, ErrorString, Global::GUIMSGTYPE_INFO);
        }
        return;
    }
    else
    {
        (void)ProgramDataStream.device()->reset();
        ProgramDataStream << Program ;
        if(mp_MasterThreadController){
            SendAckAndUpdateGUI(Ref, AckCommandChannel, Global::CommandShPtr_t(
                                    new MsgClasses::CmdProgramUpdate(10000, ProgramDataStream)));
            Global::EventObject::Instance().RaiseEvent(EVENT_DM_PROG_UPDATE_PROGRAM,Global::tTranslatableStringList()<<Program.GetName());
        }
    }
}

}// End of namespace DataManager



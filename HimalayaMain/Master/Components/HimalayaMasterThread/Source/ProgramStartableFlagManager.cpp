/****************************************************************************/
/*! \file HimalayaMasterThread/Source/ProgramStartableFlagManager.cpp
 *
 *  \brief Implementation file for class ProgramStartableFlagManager.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2010-11-05
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
//Project Headers
#include <HimalayaMasterThread/Include/ProgramStartableFlagManager.h>
#include <HimalayaMasterThread/Include/HimalayaMasterThreadController.h>
#include <NetCommands/Include/CmdStationsDefect.h>
#include <NetCommands/Include/CmdConfigurationFile.h>
#include <HimalayaDataContainer/Containers/Programs/Include/DataProgramList.h>
#include <Global/Include/EventObject.h>
#include <TestStubDataManager/CommandInterface/Include/UserSettingsCommandInterface.h>
#include <HimalayaDataManager/CommandInterface/Include/ProgramCommandInterface.h>
namespace Himalaya {

/****************************************************************************/
/*!
 *  \brief Constructor
 *  \iparam p_HimalayaMasterThreadController
 *
 */
/****************************************************************************/
ProgramStartableManager::ProgramStartableManager(HimalayaMasterThreadController *p_HimalayaMasterThreadController)
    :mp_HimalayaMasterThreadController(p_HimalayaMasterThreadController),
    mp_DataManager(NULL),
    mp_UserSettings(NULL),
    mp_UserSettingsInterface(NULL),
    mp_ProgramList(NULL) {
    //This object shall now live on himalaya master thread.
    this->setParent(p_HimalayaMasterThreadController);

}

/****************************************************************************/
/*!
 *  \brief Initializes the object
 *  \warning Catch expception thrown by CHECKPTR
 */
/****************************************************************************/
void ProgramStartableManager::Init() {
    try {
    //Initialize all pointers that will be used
    CHECKPTR(mp_HimalayaMasterThreadController);
    mp_DataManager = dynamic_cast<DataManager::CDataManager*>((const_cast<DataManager::CDataManagerBase *>(mp_HimalayaMasterThreadController->GetDataManager())));
    CHECKPTR(mp_DataManager);
    mp_UserSettingsInterface = mp_DataManager->GetUserSettingsInterface();
    CHECKPTR(mp_UserSettingsInterface)
    mp_UserSettings = mp_UserSettingsInterface->GetUserSettings();
    CHECKPTR(mp_UserSettings);
    mp_ProgramList = mp_DataManager->GetProgramList();
    CHECKPTR(mp_ProgramList);
    ConnectSignals();
}
    catch (...) {

    }
}

/****************************************************************************/
/*!
 *  \brief Register the commands required to be handled
 */
/****************************************************************************/
void ProgramStartableManager::ConnectSignals() {
    CONNECTSIGNALSLOT(mp_DataManager->mp_SettingsCommandInterface, UserSettingsChanged(const bool), this , OnUserSettingsChanged(const bool));
    CONNECTSIGNALSLOT(mp_DataManager->mp_ProgramCommandInterface, StartableProgramEdited(const QString &), this , OnStartableProgramEdited(const QString &));
}


/****************************************************************************/
/*!
 *  \brief Manages Static startable flag if program step in a startable
 *         program is edited / or DI water step is removed
 *  \iparam ProgramID
 */
/****************************************************************************/
void ProgramStartableManager::OnStartableProgramEdited(const QString &ProgramID) {
    Q_UNUSED(ProgramID)
}

/****************************************************************************/
/*!
 *  \brief  Manages Dynamic startable flag based on the liquid level values
 *          read during initialization
 *  \iparam DefectiveStations     = List of defective stations ( Defect in the
 *                                  sense liquid level is out of range)
 */
/****************************************************************************/
void ProgramStartableManager::OnReadLiquidLevelComplete(QStringList DefectiveStations) {
    Q_UNUSED(DefectiveStations)
}

/****************************************************************************/
/*!
 *  \brief  Manage static Program Startable flag when User changes DI water
 *          type to Tap water type in Bathlayout screen.
 */
/****************************************************************************/
void ProgramStartableManager::OnUserSettingsChanged(const bool LanguangeChanged) {
    /*
        Global::WaterType CurrentWaterType = Global::StringToWaterType(mp_UserSettings->GetValue("Water_Type"));
        QStringList NonStartableProgramIDs;
        QStringList StartableProgramIDList =  mp_ProgramSequenceList->GetStartableProgramIDList();
        for (qint32 I = 0 ; I < StartableProgramIDList.size(); I++) {
            DataManager::CProgram *p_Program = const_cast<DataManager::CProgram*>(mp_ProgramList->GetProgram(StartableProgramIDList.at(I)));
            if (p_Program) {
                for (qint32 I = 0; I < p_Program->GetNumberOfSteps(); I++) {
                    const DataManager::CProgramStep *p_ProgramStep = p_Program->GetProgramStep(I);
                    //Check: Current step is Distilled water and Current Water Type is not distilled water
                    if ((QString::compare(p_ProgramStep->GetReagentID(), DISTILLED_WATER_STEP) == 0)
                            && QString::compare(p_ProgramStep->GetReagentID(), Global::WaterTypeToString(CurrentWaterType)) != 0) {
                        //Store Program Ids of Programs which use DI water
                        NonStartableProgramIDs << p_Program->GetID();
                        //We dont need to iterate further, check next startable program
                        continue;
                    }
                }
            }
        }
        //Now that we have Program IDs of Programs which are no longer startable update Program Sequence Container
        QStringListIterator It(NonStartableProgramIDs);
        while (It.hasNext()) {
            DataManager::CProgramSequence *p_ProgramSequence = const_cast<DataManager::CProgramSequence*>(mp_ProgramSequenceList->GetProgramSequenceStep(It.next()));
            if (p_ProgramSequence) {
                p_ProgramSequence->SetStartCapableStatus(false);
                p_ProgramSequence->SetStartCapableResult("2");
            }
        }
        if (NonStartableProgramIDs.size() >= 1) {
            //Log and inform EventHandler
            Global::EventObject::Instance().RaiseEvent(EVENT_DI_WATER_NOT_AVAILABLE);
            UpdateGUI();
        }
    */

    if (!LanguangeChanged || (0 == mp_DataManager))
    {
        return;
    }

    if (mp_DataManager->GetUserSettingsInterface()->GetUserSettings(false) != NULL) {
        // get the current language
        QLocale::Language CurrentLanguage =
                mp_DataManager->GetUserSettingsInterface()->GetUserSettings(false)->GetLanguage();

        // store the langauge name de_DE
        QString LanguageName(QLocale(CurrentLanguage).name());
        // remove the DE
        LanguageName.truncate(LanguageName.lastIndexOf('_'));
        qDebug() << "Language Name ############################" << LanguageName << "#########################";

        QString LanguageFileName = "Himalaya_" + LanguageName + ".qm";
        mp_HimalayaMasterThreadController->SendLanguageFileToGUI(LanguageFileName);
    }

}

/****************************************************************************/
/*!
 *  \brief  Manage Dynamic Program Startable flag when Station defect occurs.
 *  \iparam DefectiveStationList = List of defective stations.
 */
/****************************************************************************/
void ProgramStartableManager::StationDefectHandler(QStringList DefectiveStationList) {
    Q_UNUSED(DefectiveStationList)
/*
    QStringList NonStartableProgramIDs;
    if (mp_ProgramList && mp_ProgramSequenceList) {
        QStringList StartableProgramIDList = mp_ProgramSequenceList->GetStartableProgramIDList();
        for (qint32 I = 0 ; I < StartableProgramIDList.size(); I++) {
            DataManager::CProgram *p_Program = const_cast<DataManager::CProgram*>(mp_ProgramList->GetProgram(StartableProgramIDList.at(I)));
            if (p_Program) {
                QStringListIterator It(DefectiveStationList);
                while(It.hasNext()) {
                    if (p_Program->GetStationsUsedByProgram().contains(It.next())) {
                        //Store Program Ids of Programs which use Defective Stations
                        NonStartableProgramIDs << p_Program->GetID();
                        //we dont need to iterate over the entire defective station list
                        break;
                    }
                }
            }
        }
    }
    //Now that we have Program IDs of Programs which are no longer startable update Program Sequence Container
    QStringListIterator It(NonStartableProgramIDs);
    while (It.hasNext()) {
        DataManager::CProgramSequence *p_ProgramSequence = const_cast<DataManager::CProgramSequence*>(mp_ProgramSequenceList->GetProgramSequenceStep(It.next()));
        if (p_ProgramSequence) {
            p_ProgramSequence->SetStartCapableDynamicStatus(false);
            if (DefectiveStationList.at(0).at(0) == 'O') {
                //Oven Defect
                p_ProgramSequence->SetStartCapableResult("4");
            }
            else if (DefectiveStationList.at(0).at(0) == 'H') {
                // Heated Cuvette Defect
                p_ProgramSequence->SetStartCapableResult("5");
            }
            else if (DefectiveStationList.at(0).at(0) == 'I') {
                //Water Station Defect
                p_ProgramSequence->SetStartCapableResult("6");
            }
        }
    }
    Global::FmtArgs Args;
    QStringListIterator DefectiveStationIterator(DefectiveStationList);
    while (DefectiveStationIterator.hasNext()) {
        Args << DefectiveStationIterator.next();
    }
    qDebug()<<"List of Defective station "<<DefectiveStationList;
    //Log and inform EventHandler
    Global::EventObject::Instance().RaiseEvent(EVENT_STATIONS_DEFECT, Args, true);
    //For all the stations above send CmdStationDefect , and then finally the updated
    mp_HimalayaMasterThreadController->SendCommand(Global::CommandShPtr_t(new NetCommands::CmdStationsDefect(2000, DefectiveStationList)), mp_HimalayaMasterThreadController->GetCommandChannel(Himalaya::GUI));
    UpdateGUI();
*/
 }

/****************************************************************************/
/*!
 *  \brief  Sends GUI the Program Sequence Container
 */
/****************************************************************************/
void ProgramStartableManager::UpdateGUI() const {
/*
    QByteArray BA;
    QDataStream ProgramSequeneStream(&BA, QIODevice::ReadWrite);
    ProgramSequeneStream.setVersion(static_cast<int>(QDataStream::Qt_4_0));
    if (mp_HimalayaMasterThreadController) {
        ProgramSequeneStream << *mp_ProgramSequenceList;
        mp_ProgramSequenceList->Write();
        mp_HimalayaMasterThreadController->SendCommand(Global::CommandShPtr_t(new NetCommands::CmdConfigurationFile(5000, NetCommands::PROGRAM_SEQUENCE , ProgramSequeneStream)),
                                                       mp_HimalayaMasterThreadController->GetCommandChannel(Himalaya::GUI));
    }
*/
}

/****************************************************************************/
/*!
 *  \brief  Checks if a program is startable.
 *  \iparam ProgramID   = Program ID of the program to check
 *
 *  \return   True - if startable , else false.
 */
/****************************************************************************/
bool ProgramStartableManager::IsProgramStartable(const QString &ProgramID) const {
    Q_UNUSED(ProgramID)
    try {
/*        CHECKPTR(mp_ProgramSequenceList);
        QStringList StartableProgramIDList = mp_ProgramSequenceList->GetStartableProgramIDList();
        if (StartableProgramIDList.contains(ProgramID)) {
            return true;
        }
        else {
            return false;
        }
        */
        return true;
    }
    catch (...) {
        //Raise exception ->LOG
        return false;
    }
}



}//End of namespace Himalaya

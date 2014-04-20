/****************************************************************************/
/*! \file HimalayaUserSettings.h
 *
 *  \brief Definition file for class CHimalayaUserSettings.
 *
 *  $Version:   $ 0.2
 *  $Date:      $ 2010-08-23, 2012-04-23
 *  $Author:    $ J.Bugariu, Raju
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

#ifndef DATAMANAGER_HIMALAYA_USERSETTINGS_H
#define DATAMANAGER_HIMALAYA_USERSETTINGS_H

#include "DataManager/Containers/UserSettings/Include/UserSettings.h"


namespace DataManager {

/****************************************************************************/
/**
 * \brief Enum for RMS mode.
 */
/****************************************************************************/
enum RMSMode {
    RMS_UNDEFINED = 0,  ///< Undefind. Used for initialization.
    RMS_OFF,            ///< RMS off.
    RMS_CASSETTES,      ///< RMS by cassettes.
    RMS_CYCLES,         ///< RMS by cycles.
    RMS_DAYS            ///< RMS by days.
};

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function ERROR_TONE_PERIODIC_TIME
 *
 *  \return from ERROR_TONE_PERIODIC_TIME
 */
/****************************************************************************/
const QString ERROR_TONE_PERIODIC_TIME("ErrorTone_PeriodicTime");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function WARNING_TONE_PERIODIC
 *
 *  \return from WARNING_TONE_PERIODIC
 */
/****************************************************************************/
const QString WARNING_TONE_PERIODIC("WarningTone_Periodic");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function WARNING_TONE_PERIODIC_TIME
 *
 *  \return from WARNING_TONE_PERIODIC_TIME
 */
/****************************************************************************/
const QString WARNING_TONE_PERIODIC_TIME("WarningTone_PeriodicTime");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function INFORMATION_TONE_NUMBER
 *
 *  \return from INFORMATION_TONE_NUMBER
 */
/****************************************************************************/
const QString INFORMATION_TONE_NUMBER("InformationTone_Number");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function INFORMATION_TONE_LEVEL
 *
 *  \return from INFORMATION_TONE_LEVEL
 */
/****************************************************************************/
const QString INFORMATION_TONE_LEVEL("InformationTone_Level");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function INFORMATION_TONE_PERIODIC
 *
 *  \return from INFORMATION_TONE_PERIODIC
 */
/****************************************************************************/
const QString INFORMATION_TONE_PERIODIC("InformationTone_Periodic");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function INFORMATION_TONE_PERIODIC_TIME
 *
 *  \return from INFORMATION_TONE_PERIODIC_TIME
 */
/****************************************************************************/
const QString INFORMATION_TONE_PERIODIC_TIME("InformationTone_PeriodicTime");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function PARAFFIN_BATH_TEMPERATURE
 *
 *  \return from PARAFFIN_BATH_TEMPERATURE
 */
/****************************************************************************/
const QString PARAFFIN_BATH_TEMPERATURE("ParaffinBath_Temperature");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function PV_FREQUENCY
 *
 *  \return from PV_FREQUENCY
 */
/****************************************************************************/
const QString PV_FREQUENCY("PV_Frequency");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function RMS_PROCESSING_MODE
 *
 *  \return from RMS_PROCESSING_MODE
 */
/****************************************************************************/
const QString RMS_PROCESSING_MODE("RMS_ProcessingMode");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function RMS_CLEANING_MODE
 *
 *  \return from RMS_CLEANING_MODE
 */
/****************************************************************************/
const QString RMS_CLEANING_MODE("RMS_CleaningMode");

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function PERIODIC_ON
 *
 *  \return from PERIODIC_ON
 */
/****************************************************************************/
const QString PERIODIC_ON("on");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function PERIODIC_OFF
 *
 *  \return from PERIODIC_OFF
 */
/****************************************************************************/
const QString PERIODIC_OFF("off");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function RMS_CASSETTES_STRING
 *
 *  \return from RMS_CASSETTES_STRING
 */
/****************************************************************************/
const QString RMS_CASSETTES_STRING("cassettes");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function RMS_CYCLES_STRING
 *
 *  \return from RMS_CYCLES_STRING
 */
/****************************************************************************/
const QString RMS_CYCLES_STRING("cycles");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function RMS_DAYS_STRING
 *
 *  \return from RMS_DAYS_STRING
 */
/****************************************************************************/
const QString RMS_DAYS_STRING("days");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function RMS_OFF_STRING
 *
 *  \return from RMS_OFF_STRING
 */
/****************************************************************************/
const QString RMS_OFF_STRING("off");

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function CASSETTE_COUNT
 *
 *  \return from CASSETTE_COUNT
 */
/****************************************************************************/
const QString CASSETTE_COUNT("CassetteCount_Number");//need the format with "_"
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function REAGENTID_OF_LASTSTEP
 *
 *  \return from REAGENTID_OF_LASTSTEP
 */
/****************************************************************************/
const QString REAGENTID_OF_LASTSTEP("ReagentIdOfLastStep_ID");

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function SERVICE_OPERATION_HOURS
 *
 *  \return from SERVICE_OPERATION_HOURS
 */
/****************************************************************************/
const QString SERVICE_OPERATION_HOURS("Service_OperationHours");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function SERVICE_OPERATION_LAST_RESET_DATE
 *
 *  \return from SERVICE_OPERATION_LAST_RESET_DATE
 */
/****************************************************************************/
const QString SERVICE_OPERATION_LAST_RESET_DATE("Service_OperationLastResetDate");

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function SERVICE_ACTIVE_CARBON_HOURS
 *
 *  \return from SERVICE_ACTIVE_CARBON_HOURS
 */
/****************************************************************************/
const QString SERVICE_ACTIVE_CARBON_HOURS("Service_ActiveCarbonHours");
/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function SERVICE_ACTIVE_CARBON_LAST_RESET_DATE
 *
 *  \return from SERVICE_ACTIVE_CARBON_LAST_RESET_DATE
 */
/****************************************************************************/
const QString SERVICE_ACTIVE_CARBON_LAST_RESET_DATE("Service_ActiveCarbonLastResetDate");

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of function SERVICE_USE_EXHAUST_SYSTEM
 *
 *  \return from SERVICE_USE_EXHAUST_SYSTEM
 */
/****************************************************************************/
const QString SERVICE_USE_EXHAUST_SYSTEM("Service_UseExhaustSystem");



/****************************************************************************/
/**
 * \brief Class for reading / writing XML based configuration file for user settings.
 *
 * <b>This class is not thread safe.</b>
 */
/****************************************************************************/
class CHimalayaUserSettings: public CUserSettings
{
public:
    CHimalayaUserSettings(void);
    CHimalayaUserSettings(const CHimalayaUserSettings &);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function CHimalayaUserSettings
     *
     *  \return from CHimalayaUserSettings
     */
    /****************************************************************************/
    CHimalayaUserSettings(const CUserSettings &);

    ~CHimalayaUserSettings(void);

    int GetSoundNumberInformation(void) const;

    void SetSoundNumberInformation(int SoundNumberInformation);

    int GetSoundLevelInformation(void) const;

    void SetSoundLevelInformation(int SoundLevelInformation);

    bool GetSoundPeriodicError(void) const;

    bool GetSoundPeriodicWarning(void) const;

    void SetSoundPeriodicWarning(bool SoundPeriodicWarning);

    bool GetSoundPeriodicInformation(void) const;

    void SetSoundPeriodicInformation(bool SoundPeriodicInformation);

    int GetSoundPeriodicTimeError(void) const;

    void SetSoundPeriodicTimeError(int SoundPeriodicTimeError);

    int GetSoundPeriodicTimeWarning(void) const;

    void SetSoundPeriodicTimeWarning(int SoundPeriodicTimeWarning);

    int GetSoundPeriodicTimeInformation(void) const;

    void SetSoundPeriodicTimeInformation(int SoundPeriodicTimeInformation);

    int GetTemperatureParaffinBath(void);

    void SetTemperatureParaffinBath(int TemperatureParaffinBath);

    int GetFrequencyPV(void);

    void SetFrequencyPV(int FrequencyPV);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetCassetteCount
     *
     *  \param CassetteCount = int type parameter
     *
     *  \return from SetCassetteCount
     */
    /****************************************************************************/
    void SetCassetteCount(int CassetteCount);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetCassetteCount
     *
     *  \return from GetCassetteCount
     */
    /****************************************************************************/
    int GetCassetteCount();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetReagentIdOfLastStep
     *
     *  \param reagentID = QString type parameter
     *
     *  \return from SetReagentIdOfLastStep
     */
    /****************************************************************************/
    void SetReagentIdOfLastStep(QString& reagentID);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetReagentIdOfLastStep
     *
     *  \return from GetReagentIdOfLastStep
     */
    /****************************************************************************/
    QString GetReagentIdOfLastStep();

    Global::RMSOptions_t GetModeRMSProcessing(void);

    void SetModeRMSProcessing(Global::RMSOptions_t ModeRMSProcessing);

    Global::RMSOptions_t GetModeRMSCleaning(void);

    void SetModeRMSCleaning(Global::RMSOptions_t ModeRMSCleaning);

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetOperationHours
     *
     *  \param operationHours = int type parameter
     *
     *  \return from SetOperationHours
     */
    /****************************************************************************/
    void SetOperationHours(int operationHours);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetOperationHours
     *
     *  \return from GetOperationHours
     */
    /****************************************************************************/
    int GetOperationHours();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetActiveCarbonHours
     *
     *  \param operationHours = int type parameter
     *
     *  \return from SetActiveCarbonHours
     */
    /****************************************************************************/
    void SetActiveCarbonHours(int operationHours);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetActiveCarbonHours
     *
     *  \return from GetActiveCarbonHours
     */
    /****************************************************************************/
    int GetActiveCarbonHours();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetOperationLastResetDate
     *
     *  \param resetDate = QString type parameter
     *
     *  \return from SetOperationLastResetDate
     */
    /****************************************************************************/
    void SetOperationLastResetDate(QString& resetDate);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetOperationLastResetDate
     *
     *  \return from GetOperationLastResetDate
     */
    /****************************************************************************/
    QString GetOperationLastResetDate();
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetActiveCarbonLastResetDate
     *
     *  \param resetDate = QString type parameter
     *
     *  \return from SetActiveCarbonLastResetDate
     */
    /****************************************************************************/
    void SetActiveCarbonLastResetDate(QString& resetDate);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetActiveCarbonLastResetDate
     *
     *  \return from GetActiveCarbonLastResetDate
     */
    /****************************************************************************/
    QString GetActiveCarbonLastResetDate();

    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function SetUseExhaustSystem
     *
     *  \param checked = int type parameter
     *
     *  \return from SetUseExhaustSystem
     */
    /****************************************************************************/
    void SetUseExhaustSystem(int checked);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function GetUseExhaustSystem
     *
     *  \return from GetUseExhaustSystem
     */
    /****************************************************************************/
    int GetUseExhaustSystem();

    bool operator == (const CHimalayaUserSettings &);
}; // end class CUserSettings

} // end namespace DataManager

#endif // DATAMANAGER_CUSERSETTINGS_H

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

const QString ERROR_TONE_PERIODIC_TIME("ErrorTone_PeriodicTime");
const QString WARNING_TONE_PERIODIC("WarningTone_Periodic");
const QString WARNING_TONE_PERIODIC_TIME("WarningTone_PeriodicTime");
const QString INFORMATION_TONE_NUMBER("InformationTone_Number");
const QString INFORMATION_TONE_LEVEL("InformationTone_Level");
const QString INFORMATION_TONE_PERIODIC("InformationTone_Periodic");
const QString INFORMATION_TONE_PERIODIC_TIME("InformationTone_PeriodicTime");
const QString PARAFFIN_BATH_TEMPERATURE("ParaffinBath_Temperature");
const QString PV_FREQUENCY("PV_Frequency");
const QString RMS_PROCESSING_MODE("RMS_ProcessingMode");
const QString RMS_CLEANING_MODE("RMS_CleaningMode");

const QString PERIODIC_ON("on");
const QString PERIODIC_OFF("off");
const QString RMS_CASSETTES_STRING("cassettes");
const QString RMS_CYCLES_STRING("cycles");
const QString RMS_DAYS_STRING("days");
const QString RMS_OFF_STRING("off");

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

    RMSMode GetModeRMSProcessing(void);

    void SetModeRMSProcessing(RMSMode ModeRMSProcessing);

    RMSMode GetModeRMSCleaning(void);

    void SetModeRMSCleaning(RMSMode ModeRMSCleaning);

}; // end class CUserSettings

} // end namespace DataManager

#endif // DATAMANAGER_CUSERSETTINGS_H

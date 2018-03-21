// ================================================================================================
/*! @file Himalaya/HimalayaMain/Master/Components/Scheduler/Include/SchedulerMainThreadController.h
 *
 *  @brief
 *  Definition of class Scheduler::Schedule.
 *
 *  $Version:   $ 0.1
 *  $Date:      $ 2013-03-06
 *  $Author:    $ Abe Yang
 *
 *  @b Company:
 *
 *       Leica Microsystems Ltd. Shanghai.
 *
 *  (C) Copyright 2013 by Leica Microsystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 * ================================================================================================
*/
#ifndef SCHEDULER_MAINTHREADCONTROLLER_H
#define SCHEDULER_MAINTHREADCONTROLLER_H

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//                                                                                  From Qt-Library
#include <QQueue>
#include <QMap>
// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//                                                                                 Project Specific
#include "Threads/Include/ThreadController.h"
#include <Global/Include/Commands/Command.h>
#include "HimalayaDataContainer/Containers/DashboardStations/Commands/Include/CmdProgramAction.h"
#include "NetCommands/Include/CmdSystemAction.h"
#include "SchedulerMachine.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"
#include "DataManager/Helper/Include/Types.h"
#include "Scheduler/Include/ProgramStatusInfor.h"
#include "Scheduler/Include/HeatingStrategy.h"
#include "Global/Include/Commands/AckOKNOK.h"
#include "Scheduler/Include/SchedulerLogging.h"
#include "Scheduler/Include/EventScenarioErrorXMLInfo.h"

using namespace DeviceControl;

namespace MsgClasses
{
    class CmdKeepCassetteCount;
    class CmdProgramSelected;
    class CmdQuitAppShutdown;
    class CmdSavedServiceInfor;
    class CmdParaffinMeltPointChanged;
    class CmdBottleCheck;
    class CmdTakeOutSpecimenFinished;
}

namespace DataManager
{
    class CDataManager;
    class CReagent;
}

namespace Scheduler {

class SchedulerCommandProcessorBase;
class CSchedulerStateMachine;
class HeatingStrategy;
class CProgramSelfTest;
class CSchedulerStateHandler;

const qint64 TIME_FOR_FIX_TIME = 180;    ///< seconds for fix time
const quint64 EVENTID_ALARM_FOR_DRAIN = 610000100; ///< Event ID for alarm Draining

#define TIME_FOR_CLEANING_DRY_STEP            720    ///< seconds spending in dry step
#define TIME_FOR_HEATING_LEVEL_SENSOR         30     ///< seconds for heating level sensor
#define TIME_FOR_FILLING                      60     ///< seconds for filling
#define TIME_FOR_MOVE_SEAL                    3      ///< seconds for move seal
#define TIME_FOR_PRESSURE_CHECK               15     ///< seconds for pressure check before move tube
#define TIME_FOR_MOVE_TUBE                    5      ///< seconds for move tube
#define TIME_FOR_DRAIN                        60     ///< seconds for draing
#define TIME_FOR_MOVE_NEXT_TUBE               8      ///< seconds for move next tube
#define TIME_FOR_COOLING_DOWN                 1200   ///< seconds for cooling down

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of struct ProgramStepInfor
 */
/****************************************************************************/
typedef struct
{
    QString         ReagentGrpId;           //!< reagent group ID
    QString         StationID;              //!< the current StationID
    RVPosition_t    RvPos;                  //!< RV position
}BottlePosition_t;

/****************************************************************************/
/*!
 *  \brief  Definition/Declaration of struct ProgramStepInfor
 */
/****************************************************************************/
typedef struct
{
    CANObjectKeyLUT::CANObjectIdentifier_t FctModID;       ///<  Definition/Declaration of variable FctModID
    bool IsAvialable;       ///<  Definition/Declaration of variable IsAvialable
    bool IsWorking;       ///<  Definition/Declaration of variable IsWorking
    bool IsHealth;       ///<  Definition/Declaration of variable IsHealth
    qint64 StartTime;       ///<  Definition/Declaration of variable StartTime
    qint64 StopTime;       ///<  Definition/Declaration of variable StopTime
}FunctionModuleStatus_t;

/****************************************************************************/
/*!
 *  \brief  Definition of Heater Type
 */
/****************************************************************************/
typedef enum
{
    UNKNOWN,
    LEVELSENSOR,
    LATUBE1,
    LATUBE2,
    LATUBE1ABNORMAL,
    LATUBE2ABNORMAL,
    RETORT,
    RETORT_NOSIGNAL,
    OVEN,
    RV,
    ASB5,
    FAN
} HeaterType_t;

/****************************************************************************/
/*!
 * \brief Definition of power failure Type
 */
/****************************************************************************/
typedef enum
{
    POWERFAILURE_INIT,
    POWERFAILURE_RUN
} PowerFailureStep_t;

/****************************************************************************/
/*!
 *  \brief struct for Received command
 */
/****************************************************************************/
typedef struct
{
    DeviceControl::ReturnCode_t retCode;    //!< return Code
    QString                     cmdName;    //!< the command name
} RecvCommand_t;

/****************************************************************************/
/*!
 *  \brief struct for Slave module attributes
 */
/****************************************************************************/
typedef struct
{
    qreal CurrentMax5VDC;               //!< the current max 5VDC
    qreal Voltagerated24VDC;            //!< the voltage 24 VDC
    qreal VoltageTolerance24VDC;        //!< the voltage 24 VDC
} SlaveAttr_t;

/****************************************************************************/
/*!
 *  \brief enum for Cleaning Dry step
 */
/****************************************************************************/
typedef enum
{
    IDLE_HEATING_RV,
    IDLE_DRAIN_10S,
    IDLE_MOVE_RV_INITIALIZE,
    IDLE_MOVE_TUBE_2,
    IDLE_READY_OK
}IdleStepState_t;

/****************************************************************************/
/*!
 *  \brief struct for nondevice command
 */
/****************************************************************************/
typedef struct {
    QString Retort_name;
    ControlCommandType_t Cmd;
}NonDeviceCommand_t;

    /****************************************************************************/
    /**
     * @brief Controller class for the main scheduler thread.
     *
     */
    /****************************************************************************/
    class SchedulerMainThreadController : public Threads::ThreadController {
        Q_OBJECT

#ifdef GOOGLE_MOCK
		// add "friend class TestSchedulerController" for UT
        friend class TestSchedulerMainThreadController;
#endif

    private:
        QTimer m_TickTimer;       ///<  Definition/Declaration of variable m_TickTimer
        QMutex m_Mutex;       ///<  Definition/Declaration of variable m_Mutex
        QQueue<Global::CommandShPtr_t> m_SchedulerCmdQueue;       ///< Definition/Declaration of variable m_SchedulerCmdQueue
        QMutex m_MutexDeviceControlCmdQueue;                      ///< mutex for accessing m_DeviceControlCmdQueue
        QWaitCondition m_WaitCondition;                           ///< condition variable for DeviceControlCmdQueue
        QQueue<Scheduler::SchedulerCommandShPtr_t> m_DeviceControlCmdQueue;                     ///< Queue(Q2) for receive respond from SechedulerCommandProcessor

        QThread* m_SchedulerCommandProcessorThread;       ///<  Definition/Declaration of variable m_SchedulerCommandProcessorThread
        SchedulerCommandProcessorBase*  m_SchedulerCommandProcessor;       ///<  Definition/Declaration of variable m_SchedulerCommandProcessor
        //CSchedulerStateMachine* m_SchedulerMachine;       ///<  Definition/Declaration of variable m_SchedulerMachine
        QMap<QString, QSharedPointer<CSchedulerStateHandler>> m_SchedulerStateHandlerList;
        DataManager::CDataManager       *mp_DataManager;       ///<  Definition/Declaration of variable mp_DataManager
        qreal m_TempALLevelSensor;       ///<  Definition/Declaration of variable m_TempALLevelSensor
        qreal m_TempALTube1;       ///<  Definition/Declaration of variable m_TempALTube1
        qreal m_TempALTube2;       ///<  Definition/Declaration of variable m_TempALTube2
        qreal m_TempRV1;       ///<  Definition/Declaration of variable m_TempRV1
        qreal m_TempRV2;            ///<  Definition/Declaration of variable m_TempRV2
        quint16 m_OvenLidStatus;       ///<  Definition/Declaration of variable m_OvenLidStatus
        quint16 m_RetortLockStatus;       ///<  Definition/Declaration of variable m_RetortLockStatus

        quint32 m_EventKey;                                   ///< Current Event key
        ReturnCode_t m_CurErrEventID;                         ///< Current Event ID
        QSharedPointer<HeatingStrategy> mp_HeatingStrategy;   ///< Definition/Declaration of variable mp_HeatingStrategy
        Global::tRefType    m_RefCleanup;                     ///< Command reference of the cleanup command
        bool m_IsInSoakDelay;                                 ///< Delay in Soak
        bool m_IsPrecheckMoveRV;                            ///< precheck done move rv
        qint64 m_lastPVTime;                                  ///< Time for last PV operation
        qint8 m_ProcessingPV;                                 ///< flag to indicate P or V operation
        bool m_completionNotifierSent;                        ///< Flag to indication if program completion is sent to Gui.
        bool m_CleanAckSentGui;                                ///< flag to indicate if cleaning ack to gui or not
        CProgramStatusInfor m_ProgramStatusInfor;              ///< Program Status Infor
        BottlePosition_t    m_CurrentBottlePosition;          ///< the current BottlePosition for bottle check
        bool m_hasParaffin;                                   ///< the program has paraffin
        bool m_IsReleasePressureOfSoakFinish;                 ///< wether release pressure when soak finish
        QVector<SlaveAttr_t>  m_SlaveAttrList;                ///< Attribute list of Slave modules
        qint8   m_ReEnterFilling;                             ///< When restart filling, the sequence of re-entering filling
        qint64  m_TimeReEnterFilling;                         ///< Time when re-enter filling
        bool    m_CheckRemoteAlarmStatus;                     ///< flag to check m_CheckRemoteAlarmStatus
        bool    m_CheckLocalAlarmStatus;                      ///< flag to check m_CheckLocalAlarmStatus
        bool    m_DisableAlarm;                               ///< disable alarm or not
        qint8   m_LocalAlarmPreviousStatus;                   /// -1 - unknow status, 0 - connected, 1 - not connected
        qint8   m_RemoteAlarmPreviousStatus;                  /// -1 - unknow status, 0 - connected, 1 - not connected
        qint8   m_PssmStepFinSeq;                             ///< sequence of PSSM_STEP_FIN stage
        QSharedPointer<EventScenarioErrXMLInfo> m_pESEXMLInfo;///< Event-Scenario-Error parser
        QVector<QString> m_UnknownErrorLogVector;             ///< the unknow error log vector
        bool    m_InternalErrorRecv;                          ///< Internal error was received
        bool    m_CheckOvenCover;                             ///< check the oven cover
        bool    m_bWaitToPause;                               ///< Wait to be Paused
		bool    m_TransitionPeriod;                           ///< flag to indicate transition period
        PowerFailureStep_t m_PowerFailureStep;                ///< the power failure step
        bool    m_IsSendMsgForWaitHeatRV;                     ///< wether send message for waiting heating RV
        bool    m_IsErrorStateForHM;                          ///< enter the error state
        bool    m_ReportExhaustFanWarning;                    ///< flag to report exhaust fan warning
        bool    m_IsProcessing;                               ///< in processing
        QVector< QPair<QString, QString> >  m_DashboardStationList;     ///< The whole station List of dash board
        IdleStepState_t m_IdleState;                          ///< prepare for idle step state
        qint8   m_RVPositioinChSeqForIdle;                    ///< RVPositioin sequence for idle
        bool    m_WasPressureIdle;                            ///< has been pressured for 10 s
        bool    m_SentInfoForLockLidIdle;                     ///< has sent the information for locking retort lid
        qint64  m_PressureStartTime;                          ///< pressure start time for idle
        bool    m_IsTakeSpecimen;                             ///< wether take of specimen
        qint8   m_CountTheLogSenserData;                      ///< control the sensor data log
        quint8  m_CheckTheHardwareStatus;                     ///< Check the hardware status
        bool    m_IsFirstProcessingForDelay;                  ///< is first enter processing for delay time
        QString m_ReagentExpiredFlag;                         ///< Reagent expired flag
        QMap<DeviceControl::ReturnCode_t,quint8> m_RetCodeCounterList;      //!< Counter list for Error ReturnCode
        qint64  m_PauseStartTime;                             ///< Start time of pause command
        bool    m_bWaitToPauseCmdYes;                         ///< flag to indicate if PauseYes command is selected
        bool    m_IsResumeFromPause;                          ///< flag to indicate if we are resuming from pause state
        SchedulerStateMachine_t m_StateAtPause;               ///< The state when pause occurs

        bool    m_IsSelfTestDone;
        QSharedPointer<CProgramSelfTest> mp_ProgramSelfTest;  ///< for self-test

    private:
        SchedulerMainThreadController(const SchedulerMainThreadController&);                      ///< Not implemented.
        SchedulerMainThreadController& operator=(const SchedulerMainThreadController&);     ///< Not implemented.

        /****************************************************************************/
        /**
         * @brief Register commands.
         */
        /****************************************************************************/
        virtual void RegisterCommands();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function HardwareMonitor
         *
         *  \param StepID = const QString type parameter
         *
         *  \return from HardwareMonitor
         */
        /****************************************************************************/
        void HardwareMonitor(const QString& StepID);

        /****************************************************************************/
        /*!
         *  \brief Handle the whole work flow for Program Self-Test
         *  \param cmdName - command name
         *  \param retCode - return code
         */
        /****************************************************************************/
        void HandleSelfTestWorkFlow(const QString& cmdName, DeviceControl::ReturnCode_t retCode);

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function PeekNonDeviceCommand
         *
         *  \return from PeekNonDeviceCommand
         */
        /****************************************************************************/
        NonDeviceCommand_t PeekNonDeviceCommand();

         /****************************************************************************/
         /*!
          *  \brief Check no-signal error for temperature sensors
          *
          *  \param Scenario - current scenario
          *  \param HwTemp - current temperature of sensor
          *
          *  \return ture - no error, false - error was gotten
          */
         /****************************************************************************/
         bool CheckRetortTempSensorNoSignal(quint32 Scenario, qreal HWTemp);

         /****************************************************************************/
         /*!
          *  \brief Check no-signal error for temperature sensors
          *
          *  \param Scenario - current scenario
          *  \param HwTemp - current temperature of sensor
          *
          *  \return ture - no error, false - error was gotten
          */
         /****************************************************************************/
         bool CheckLevelSensorNoSignal(quint32 Scenario, qreal HWTemp);

         /****************************************************************************/
         /*!
          *  \brief Check if current over range error occurs in slave sensors
          *
          *  \param Scenario - current scenario
          *
          *  \return void
          */
         /****************************************************************************/
         void CheckSlaveAllSensor(quint32 Scenario, const HardwareMonitor_t& strctHWMonitor);
         /****************************************************************************/
         /*!
          *  \brief Enable pause button
          *
          *    \param bEnable - enabled or disabled
          *
          *  \return void
          */
         /****************************************************************************/
         void EnablePauseButton(bool bEnable);

         /****************************************************************************/
         /*!
          *  \brief HandleRmtLocAlarm
          *
          *  \param cmd alarm command
          *
          *  \return void
          */
         /****************************************************************************/
         void HandleRmtLocAlarm(quint32 cmd);

         /****************************************************************************/
         /*!
          *  \brief Construct slave module attribute list
          *
          *  \param QString moduleName - name of the specific slave module
          *
          *  \return bool, true - success, false - failure
          */
         /****************************************************************************/
         bool ConstructSlaveModuleAttrList(QString moduleName);

         /****************************************************************************/
         /*!
          *  \brief Get the stringID for ErrHandling and Service
          *
          *  \param quint32 - ErrorID
          *  \param Global::tTranslatableStringList - EventStringParList
          *  \param Global::tTranslatableStringList - EventRDStringParList
          */
         /****************************************************************************/
         void GetStringIDList(quint32 ErrorID,
                              Global::tTranslatableStringList &EventStringParList,
                              Global::tTranslatableStringList &EventRDStringParList);

         /****************************************************************************/
         /*!
          *  \brief Check active carbon filter is expired
          *
          */
         /****************************************************************************/
         void CheckCarbonFilterExpired();

         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot CheckLevelSensorTemperature
          *
          *  \param targetTemperature = qreal type parameter
          *
          *  \return return from CheckLevelSensorTemperature
          */
         /****************************************************************************/
         bool CheckLevelSensorTemperature(qreal targetTemperature);

         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot Pause
          */
         /****************************************************************************/
         void Pause();
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot MoveRVToInit
          */
         /****************************************************************************/
         void MoveRVToInit();

         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot DevProcDestroyed
          */
         /****************************************************************************/
         void DevProcDestroyed();
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot ReportGetServiceInfo
          *  \param ReturnCode = Return Code
          *  \param ModuleInfo = module information
          *  \param deviceName = which device got the service information
          */
         /****************************************************************************/
         void ReportGetServiceInfo(ReturnCode_t ReturnCode, const DataManager::CModule &ModuleInfo, const QString& deviceName);
         /****************************************************************************/
         /*!
          *  \brief  Definition/Declaration of slot ShutdownRetortHeater
          *  \param  setVal -- lifetime for carbon filter
          */
         /****************************************************************************/
         void ResetActiveCarbonFilterLifetime(quint32 setVal);

         /****************************************************************************/
         /*!
          *  \brief  Slot for HW error occuring;
          */
         /****************************************************************************/
         void OnSystemError();
         /****************************************************************************/
         /*!
          *  \brief  Slot for restart program from HW error;
          */
         /****************************************************************************/
         void OnBackToBusy();

         /****************************************************************************/
         /*!
          *  \brief  Slot for pretest done
          */
         /****************************************************************************/
         void OnPreTestDone();

         /****************************************************************************/
         /*!
          *  \brief  Slot for reporting error from DeviceControl
          *
          *  \iparam instanceID = Instance identifier of this function module instance
          *  \iparam usErrorGroup = Error group
          *  \iparam usErrorID = Error ID
          *  \iparam usErrorData = Additional error information
          *  \iparam timeStamp = Error time
          */
         /****************************************************************************/
         void OnReportError(quint32 instanceID, quint16 usErrorGroup, quint16 usErrorID, quint16 usErrorData, QDateTime timeStamp);

protected:

        /****************************************************************************/
        /**
         *  \brief Called when the base class received the Go signal.
         * This means that everything is fine and normal operation started.
         * We are running in our own thread now.
         */
        /****************************************************************************/
        virtual void OnGoReceived();

        /****************************************************************************/
        /**
         *  \brief Called when the base class received the Stop signal.
         * This means that normal operation will stop after processing this signal.
         * We are still running in our own thread.
         */
        /****************************************************************************/
        virtual void OnStopReceived();
        
        /****************************************************************************/
        /**
         *  \brief Called when power failure is to expect.
         *  \param  PowerFailStage
         */
        /****************************************************************************/
        virtual void OnPowerFail(const Global::PowerFailStages PowerFailStage);

         /****************************************************************************/
         /**
          * \brief Handler for acknowledge processing
          *
          * \iparam   Ref                 Acknowledge Command reference.
          * \iparam   Ack                 Command.
          */
         /****************************************************************************/
         void OnAcknowledge(Global::tRefType Ref, const Global::AckOKNOK &Ack);

         /****************************************************************************/
         /**
          *   \brief Definition/Declaration of function OnProgramAction
          *   \param Ref
          *   \param Cmd
          */
         /****************************************************************************/
        void OnProgramAction(Global::tRefType Ref, const MsgClasses::CmdProgramAction& Cmd);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function OnKeepCassetteCount
         *
         *  \param Ref = Global::tRefType type parameter
         *  \param Cmd =  MsgClasses::CmdKeepCassetteCount type parameter
         *
         *  \return from OnKeepCassetteCount
         */
        /****************************************************************************/
        void OnKeepCassetteCount(Global::tRefType Ref, const MsgClasses::CmdKeepCassetteCount & Cmd);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function OnProgramSelected
         *
         *  \param Ref = Global::tRefType type parameter
         *  \param Cmd =  MsgClasses::CmdProgramSelected type parameter
         *
         *  \return from OnProgramSelected
         */
        /****************************************************************************/
        void OnProgramSelected(Global::tRefType Ref, const MsgClasses::CmdProgramSelected& Cmd);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function OnQuitAppShutdown
         *
         *  \param Ref = Global::tRefType type parameter
         *  \param Cmd =  MsgClasses::CmdQuitAppShutdown type parameter
         *
         *  \return from OnQuitAppShutdown
         */
        /****************************************************************************/
        void OnQuitAppShutdown(Global::tRefType Ref, const MsgClasses::CmdQuitAppShutdown& Cmd);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function OnSavedServiceInfor
         *
         *  \param Ref = Global::tRefType type parameter
         *  \param Cmd =  MsgClasses::CmdSavedServiceInfor type parameter
         *
         *  \return from OnSavedServiceInfor
         */
        /****************************************************************************/
        void OnSavedServiceInfor(Global::tRefType Ref, const MsgClasses::CmdSavedServiceInfor & Cmd);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function OnParaffinMeltPointChanged
         *
         *  \param Ref = Global::tRefType type parameter
         *  \param Cmd =  MsgClasses::CmdParaffinMeltPointChanged type parameter
         *
         *  \return from OnParaffinMeltPointChanged
         */
        /****************************************************************************/
        void OnParaffinMeltPointChanged(Global::tRefType Ref, const MsgClasses::CmdParaffinMeltPointChanged & Cmd);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function OnBottleCheck
         *
         *  \param Ref = Global::tRefType type parameter
         *  \param Cmd =  MsgClasses::CmdBottleCheck type parameter
         *
         *  \return from OnBottleCheck
         */
        /****************************************************************************/
        void OnBottleCheck(Global::tRefType Ref, const MsgClasses::CmdBottleCheck & Cmd);
        /****************************************************************************/
        /*!
         *  \brief  Declaration of function OnTakeOutSpecimenFinished
         *
         *  \param Ref = Global::tRefType type parameter
         *  \param Cmd =  MsgClasses::CmdTakeOutSpecimenFinished type parameter
         *
         *  \return from OnTakeOutSpecimenFinished
         */
        /****************************************************************************/
        void OnTakeOutSpecimenFinished(Global::tRefType Ref, const MsgClasses::CmdTakeOutSpecimenFinished & Cmd);

    public:
        /****************************************************************************/
        /**
         * @brief Constructor.
         *
         * @iparam   TheHeartBeatSource    Logging source to be used.
         */
        /****************************************************************************/
        SchedulerMainThreadController(
            Global::gSourceType TheHeartBeatSource);

        /****************************************************************************/
        /**
         * @brief log debug message.
         *
         * @param   message
         */
        /****************************************************************************/
        inline void LogDebug(QString message)
        {
            qDebug() << message;
#if 1
            Global::EventObject::Instance().RaiseEvent(Global::EVENT_GLOBAL_STRING_ID_DEBUG_MESSAGE,Global::tTranslatableStringList()<<message);
#endif
        }
        /****************************************************************************/
        /**
         * @brief   return current Event key
         *
         * @return  quint32
         */
        /****************************************************************************/
        quint32 GetEventKey(){ return m_EventKey; }

        /****************************************************************************/
        /**
         * @brief   return current Event ID when an error occurs
         *
         * @return  ReturnCode_t
         */
        /****************************************************************************/
        ReturnCode_t GetCurErrEventID(){ return m_CurErrEventID; }
        /****************************************************************************/
        /**
         * @brief raise error to event handler
         *
         *  \iparam    EventKey
         *  \iparam    EventID
         *  \iparam    Scenario
         *  \iparam    ActionResult
         *  \iparam    Active
         *  \return    bool for RaiseError
         */
         /****************************************************************************/
        /*lint -e641 */
        bool RaiseError(const quint32 EventKey, ReturnCode_t EventID, const quint32 Scenario,
                                  const bool ActionResult, const bool Active = true);

        /****************************************************************************/
        /**
         * @brief raise event to event handler
         *
         *  \iparam    Event  event id
         *  \iparam    Parameters paramter list
         *  \iparam    Active flag to check active behavior
         */
         /****************************************************************************/
        inline void RaiseEvent(const quint32 Event, const QStringList& Parameters = QStringList(), bool Active = true)
        {

            Global::tTranslatableStringList ParList;
            for(QStringList::ConstIterator ite = Parameters.constBegin(); ite != Parameters.constEnd(); ite++)
            {
                ParList << *ite;
            }
            Global::EventObject::Instance().RaiseEvent(Event,ParList,Active);
        }

        /****************************************************************************/
        /*!
         *  \brief Send program acknowledge to GUI
         *
         */
        /****************************************************************************/
        void SendProgramAcknowledge(DataManager::ProgramAcknownedgeType_t ackType);

        /****************************************************************************/
        /**
         * \brief   Set transition period
         *  \param  flag - flag
         *  \return void
         */
         /****************************************************************************/
        void SetTransitionPeriod(bool flag) { m_TransitionPeriod = flag; }
        /****************************************************************************/
        /**
         * @brief Destructor.
         */
        /****************************************************************************/
        virtual ~SchedulerMainThreadController();

        /****************************************************************************/
        /**
         *  \brief Create and configure your objects.
         */
        /****************************************************************************/
        virtual void CreateAndInitializeObjects();

        /****************************************************************************/
        /**
         *  \brief Cleanup and destroy your objects.
         */
        /****************************************************************************/
        virtual void CleanupAndDestroyObjects();

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function OnActionCommandReceived
         *
         *  \param Ref = Global::tRefType type parameter
         *  \param Cmd =  NetCommands::CmdSystemAction type parameter
         *
         *  \return from OnActionCommandReceived
         */
        /****************************************************************************/
        void OnActionCommandReceived(Global::tRefType Ref, const NetCommands::CmdSystemAction &Cmd);
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function DataManager
         *
         *  \param p_DataManager = DataManager::CDataManager type parameter
         *
         *  \return from DataManager
         */
        /****************************************************************************/
        void DataManager(DataManager::CDataManager *p_DataManager);

        /****************************************************************************/
        /**
         *  \brief push Command to Q2
         *  \param CmdPtr
         */
        /****************************************************************************/
        void PushDeviceControlCmdQueue(Scheduler::SchedulerCommandShPtr_t CmdPtr);

        /****************************************************************************/
        /**
         *  \brief pop Command from Q2
         *  \param PtrCmd
         *  \return from PopDeviceControlCmdQueue of bool
         */
        /****************************************************************************/
         bool PopDeviceControlCmdQueue(Scheduler::SchedulerCommandShPtr_t& PtrCmd);

        /****************************************************************************/
        /**
         *  \brief Definition/Declaration of function PopDeviceControlCmdQueue
         *  \iparam PtrCmd
         *  \iparam CmdName
         *  \return void
         */
        /****************************************************************************/
         void PopDeviceControlCmdQueue(Scheduler::SchedulerCommandShPtr_t& PtrCmd, const QString& CmdName);
        /****************************************************************************/
        /**
         *  \brief Get the time(in seconds) that Oven has been heated
         *  \return from GetOvenHeatingTime of quint64
         */
        /****************************************************************************/
        quint64 GetOvenHeatingTime();

        /****************************************************************************/
        /**
         *  \brief Get the time(in seconds) that Oven has been heated
         *  \return from GetOvenHeatingRemainingTime of qint64
         */
        /****************************************************************************/
        qint64 GetOvenHeatingRemainingTime();

        /****************************************************************************/
        /**
         *  \brief Init program status
         *  \return from GetOvenHeatingRemainingTime of quint64
         */
        /****************************************************************************/
        void InitProgramStatus();
        /****************************************************************************/
        /**
         *  \brief Get the time(in seconds) for melting paraffin
         *  \return from quint32
         */
        /****************************************************************************/
        quint32 GetSecondsForMeltingParaffin();

        /****************************************************************************/
        /**
         *  \brief Get the RV moving steps
         *  \param firstPos - qint32
         *  \param endPos - qint32
         *  \return from GetMoveSteps of quint32
         */
        /****************************************************************************/
        quint32 GetMoveSteps(qint32 firstPos, qint32 endPos);

        /****************************************************************************/
        /**
         *  \brief Get the pressure offset for powerfailure
         *  \return from qreal
         */
        /****************************************************************************/
        qreal GetLastPressureOffset();

        /****************************************************************************/
        /**
         *  \brief Set the pressure offset for powerfailure
         *  \param pressureOffset - qreal type
         */
        /****************************************************************************/
        void SetLastPressureOffset(qreal pressureOffset);   

        /****************************************************************************/
        /**
         *  \brief Get Scheduler Commnad Processor
         *  \return pointer to SchedulerCommandProcessor
         */
        /****************************************************************************/
        SchedulerCommandProcessorBase* GetSchedCommandProcessor() const { return m_SchedulerCommandProcessor;}

        /****************************************************************************/
        /**
         *  \brief Get Heating Strategy
         *  \return pointer to SchedulerCommandProcessor
         */
        /****************************************************************************/
        QSharedPointer<HeatingStrategy> GetHeatingStrategy() const { return mp_HeatingStrategy; }


        /****************************************************************************/
        /**
         *  \brief Update current secenario in Scheduler
         *  \return void
         */
        /****************************************************************************/
        void UpdateCurrentScenario();
        /****************************************************************************/
        /*!
         *  \brief  Check the oven lock status
         *  \return qint16, 0 - lose, 1 - open
         */
        /****************************************************************************/
        inline quint16 GetOvenLockStatus() const {    return m_OvenLidStatus; }

        /****************************************************************************/
        /**
         *  \brief  Send out Error message
         *  \param  EventId - ReturnCodde_t
         *  \param  IsErrorMsg - bool type
         */
        /****************************************************************************/
        void SendOutErrMsg(ReturnCode_t EventId, bool IsErrorMsg = true);

        /****************************************************************************/
        /*!
         *  \brief  Get the tpe of failed heater
         *  \return Heater type
         */
        /****************************************************************************/
        HeaterType_t GetFailerHeaterType();

        /****************************************************************************/
        /*!
         *  \brief  Shutting down failed heaters
         *  \return bool, ture - success, false -failure
         *
         */
        /****************************************************************************/
        bool ShutdownFailedHeaters();

        /****************************************************************************/
        /*!
         *  \brief  Restart failed heaters
         *  \return bool, ture - success, false -failure
         *
         */
        /****************************************************************************/
        bool RestartFailedHeaters();

        /****************************************************************************/
        /*!
         *  \brief  Check temperature modules current range
         *  \param  interval - interval of time (in seconds)
         *  \return bool, ture - success, false -failure
         *
         */
        /****************************************************************************/
        bool CheckSlaveTempModulesCurrentRange(quint8 interval);

        /****************************************************************************/
        /*!
         *  \brief  Check temperature modules temperature
         *  \return bool, ture - success, false -failure
         *
         */
        /****************************************************************************/
        bool CheckSensorTempOverange();


        /****************************************************************************/
        /*!
         *  \brief	Check if program has paraffin reagent 
         *  \return bool 
         *
         */
        /****************************************************************************/
        inline bool HasParaffinReagent() const { return m_hasParaffin;  }

        /****************************************************************************/
        /*!
         *  \brief  Filling for Rs_Tissue_Protect
         *  \param  StationID - QString
         *  \param  EnableInsufficientCheck - bool
         *  \param  SafeReagent4Paraffin - bool
         *  \return void
         */
        /****************************************************************************/
        void FillRsTissueProtect(const QString& StationID, bool EnableInsufficientCheck,bool SafeReagent4Paraffin);

        /****************************************************************************/
        /*!
         *  \brief  Send Stop Filling for Rs_Tissue_Protect to GUI
         *  \param  StationID - QString
         *  \return void
         */
        /****************************************************************************/
        void StopFillRsTissueProtect(const QString& StationID);

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function SendPowerFailureMsg
         */
        /****************************************************************************/
        void SendPowerFailureMsg();

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function SendSafeReagentFinishedCmd
         */
        /****************************************************************************/
        void SendSafeReagentFinishedCmd();

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function DismissPausingMsgDlg
         */
        /****************************************************************************/
        void DismissPausingMsgDlg();

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function StopTimer
         */
        /****************************************************************************/
        void StopTimer() { m_TickTimer.stop(); }

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function StartTimer
         */
        /****************************************************************************/
        void StartTimer() { m_TickTimer.start(); }

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function GetDashboardStationList
         *  \return QVector list of Dashboard station
         */
        /****************************************************************************/
        QVector< QPair<QString, QString> >&  GetDashboardStationList()  { return m_DashboardStationList; }

        /****************************************************************************/
        /*!
         *  \brief  Send bottleCheck reply
         *  \param stationId - QString
         *  \param type - DataManager::BottleCheckStatusType_t
         *  \return void
         */
        /****************************************************************************/
        void SendBottleCheckReply(const QString& stationId, DataManager::BottleCheckStatusType_t type);

        void OnStartStateHandler();

        /****************************************************************************/
        /*!
         *  \brief  Get the value for the specific element
         *  \param  RetCode - return code
         *  \return quint8
         */
        /****************************************************************************/
        quint8 GetRetCodeCounter(DeviceControl::ReturnCode_t RetCode) const { return m_RetCodeCounterList[RetCode]; }

        /****************************************************************************/
        /*!
         *  \brief  Set value for the specific element
         *  \param  RetCode - return code
         *  \param  val - value of return code
         *  \return void
         */
        /****************************************************************************/
        void SetRetCodeCounter(DeviceControl::ReturnCode_t RetCode, quint8 val) { m_RetCodeCounterList[RetCode] = val; }

        /****************************************************************************/
        /*!
         *  \brief  Actions and messages for resming from pause
         *  \param  currentState - current program state
         *  \return void
         *
         */
        /****************************************************************************/
        void CheckResuemFromPause(SchedulerStateMachine_t currentState);

        /****************************************************************************/
        /*!
         *  \brief  Send command on system busy or not to GUI
         *  \param  isBusy - true for busy, false for idle
         *  \return void
         *
         */
        /****************************************************************************/
        void SendSystemBusy2GUI(bool isBusy);

        /****************************************************************************/
        /*!
         *  \brief Get the status of retort lock
         *
         *  \return 1 indicates the retort lid is open. 0 as closed
         *
         */
        /****************************************************************************/
        inline quint16 RetortLockStatus() { return m_RetortLockStatus; }
    public slots:

        /****************************************************************************/
        /**
         *  \brief For advancing the time
         */
        /****************************************************************************/
        void OnTickTimer();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of function OnDCLConfigurationFinished
         *
         *  \param RetCode = ReturnCode_t type parameter
         *
         *  \return from OnDCLConfigurationFinished
         */
        /****************************************************************************/
        void OnDCLConfigurationFinished(ReturnCode_t RetCode);

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot RcBottleCheckI
         */
        /****************************************************************************/
        void RcBottleCheckI();

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot ReleasePressure
         */
        /****************************************************************************/
        void ReleasePressure();

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot OnSelfTestDone
         *  \param  flag - bool
         */
        /****************************************************************************/
        void OnSelfTestDone(bool flag);

        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot EnablePauseButton
         */
        /****************************************************************************/
        void EnablePauseButton();
        /****************************************************************************/
        /*!
         *  \brief  Definition/Declaration of slot DisablePauseButton
         */
        /****************************************************************************/
        void DisablePauseButton();

        /****************************************************************************/
        /*!
         *  \brief  Send out Tisue Protect done message to GUI
         *  \param  flag = To indicate if safe reagent is fully passed or passed with warning
         */
        /****************************************************************************/
        void SendTissueProtectMsg(bool flag);

        /****************************************************************************/
        /*!
         *  \brief  Send Oven cover open message to GUI
         */
        /****************************************************************************/
        void SendCoverLidOpenMsg();


        /****************************************************************************/
        /*!
         *  \brief  Slot for entering PSSM_STEP_FIN stage
         */
        /****************************************************************************/
        void OnEnterPssMStepFin();

        /****************************************************************************/
        /*!
         *  \brief  Slot for entering PSSM_PROG_FIN stage
         */
        /****************************************************************************/
        void OnEnterPssMProgFin();

        /****************************************************************************/
        /*!
         *  \brief  Slot for OnStartTimer
         */
        /****************************************************************************/
        void OnStartTimer() { m_TickTimer.start(); }
    };

} // EONS ::Scheduler

#endif

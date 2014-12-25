/****************************************************************************/
/*! \file Himalaya/HimalayaMain/Master/Components/Scheduler/Include/HeatingStrategy.h
 *
 *  \brief HeatingStrategy class definition.
 *
 *   $Version: $ 0.1
 *   $Date:    $ April 3rd,2014
 *   $Author:  $ Songtao Yu 
 *
 *  \b Company:
 *
 *       Leica Biosystems SH CN.
 *
 *  (C) Copyright 2014 by Leica Biosystems Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#ifndef HEATING_STRATEGY_H
#define HEATING_STRATEGY_H
#include <QObject>
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "HimalayaDataContainer/Containers/ProgramSettings/Include/ProgramSettings.h"
#include "DeviceControl/Include/Global/DeviceControlGlobal.h"
#include "Scheduler/Commands/Include/CmdSchedulerCommandBase.h"
#include "Scheduler/Include/SchedulerCommandProcessor.h"


namespace DataManager {
class CDataManager;
}

namespace Scheduler{

class SchedulerMainThreadController;
class CSchedulerStateMachine;
class SchedulerCommandProcessorBase;

/****************************************************************************/
/*!
 * \brief struct of FunctionModule
 */
/****************************************************************************/
struct FunctionModule
{
    QString            Id;                  //!< the module of id
    QVector<qint32>    ScenarioList;        //!< vector of scenario
    qreal              TemperatureOffset;   //!< the temperature offset
    qreal              OTTargetTemperature; //!< the start temperature
    qreal              MaxTemperature;      //!< the max temperature
    qreal              HeatingOverTime;     //!< heating over time
    qreal              SlopTempChange;      //!< slop temperature change
    qreal              TemperatureOffsetWithParaffin;   //!< the target of temperature with paraffin
    quint32            ControllerGain;      //!< the PID parameter of P
    quint32            ResetTime;           //!< the PID parameter of I
    quint32            DerivativeTime;      //!< the PID parameter of D
};

/****************************************************************************/
/*!
 * \brief struct of base heatingSensor
 */
/****************************************************************************/
/*lint -e1509 */
struct HeatingSensor
{
    QString							devName;            //!< the device name
    QString							sensorName;         //!< the sensor name
    QMap<QString, FunctionModule>	functionModuleList; //!< map of struct FunctionModule
    qint64							heatingStartTime;   //!< the begin time of heating
    QString							curModuleId;        //!< the current module id
    bool                            OTCheckPassed;      //!< flag to indicate if OT checking is passed or not
};

/****************************************************************************/
/*!
 * \brief struct of retort of level sensor
 */
/****************************************************************************/
struct RTLevelSensor : public HeatingSensor
{
    QMap<QString, QString>	CurrentSpeedList;       //!< the current of speed map
    QMap<QString, qreal>	ExchangePIDTempList;    //!< temperature of exchange PID parameter map
    bool                    SetTemp4High;           //!< whether set temperature high
    bool                    SetTemp4Low;            //!< whether set temperature low
};

/****************************************************************************/
/*!
 * \brief struct of sensor checking in scenario 260
 */
/****************************************************************************/
struct SensorsChecking
{
    qint64  startTime;
    qreal   meltingPoint;
    quint32 minTime;
    bool    firstBottle;
    bool    ovenTopPass;
    bool    LATube1Pass;
};

/****************************************************************************/
/*!
 * \brief struct of retort bottom sensor
 */
/****************************************************************************/
struct RTBottomSensor : public HeatingSensor
{
    QMap<QString, qreal>    TemperatureDiffList;    //!< temperature of diff list
};

/****************************************************************************/
/*!
 * \brief stuct of oven sensor
 */
/****************************************************************************/
struct OvenSensor : public HeatingSensor
{
    bool                                    IsStartedHeating;           //!< started begin flag
    bool                                    OvenBottom2OTCheckPassed;   //!< Oven Bottom2 sensor OT Check passed
    QMap<QString, qreal>                    OTTempOffsetList;           //!< oven temperature offset map
    QMap< QString, QPair<qreal,qreal> >     ParaffinTempRangeList;      //!< paraffin temperature range map
    QMap< QString, QPair<qint64, qint64> >  TimeRangeList;              //!< time range map
};

/****************************************************************************/
/*!
 * \brief enum type of oven sensor
 */
/****************************************************************************/
typedef enum
{
    OVEN_TOP_SENSOR = 0,
    OVEN_BOTTOM1_SENSOR,
    OVEN_BOTTOM2_SENSOR
} OVENSensorType_t;

/****************************************************************************/
/*!
 * \brief struct of Rotray valve sensor
 */
/****************************************************************************/
struct RVSensor : public HeatingSensor
{
    QMap<QString, bool>     UserInputFlagList;      //!< rotary valve sensor user input flag map
};

/****************************************************************************/
/*!
 * \brief struct of Rotray valve outlet
 */
/****************************************************************************/
struct RVOutlet : public HeatingSensor
{
    bool		needCheckOT;			//!< if need check OT or not
    qreal		HeatingOverTime;    	//!< RV the over time of heating
    QString 	needCheckOTModuleId;	//!< whether need check OT module Id
};

/****************************************************************************/
/*!
 * \brief struct of LA Sensor
 */
/****************************************************************************/
struct LASensor : public HeatingSensor
{
    QMap< QString, QVector<qint32> >     OTCheckScenarioList;   //!< LA of check scenario MAP
};

/****************************************************************************/
/*!
 * \brief Heating Strategy Class
 */
/****************************************************************************/
class HeatingStrategy : public QObject
{
    Q_OBJECT
public:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function HeatingStrategy
     *
     *  \param schedController
     *  \param SchedCmdProcessor
     *  \param dataManager
     *
     *  \return from HeatingStrategy
     */
    /****************************************************************************/
    HeatingStrategy(SchedulerMainThreadController* schedController,
                    SchedulerCommandProcessorBase* SchedCmdProcessor,
                    DataManager::CDataManager* dataManager);

    /***************************************************************************/
    /*!
     *  \brief descontructor
     */
    /***************************************************************************/
    /*lint -e1540 */
    ~HeatingStrategy() {}

    /****************************************************************************/
    /*!
     *  \brief  Run and check Heating strategy logic for each time interval (500ms)
     *
     *  \param  strctHWMonitor - current temperature of level sensor
     *  \param  scenario - current scenario in Scheduler main controller
     *
     *  \return DeviceControl::ReturnCode_t - success or failure code
     */
    /****************************************************************************/
    DeviceControl::ReturnCode_t RunHeatingStrategy(const HardwareMonitor_t& strctHWMonitor, qint32 scenario);

    /****************************************************************************/
    /*!
     *  \brief  Start the specific sensor's temperature control of Precheck
     *  \param 	HeaterName - sensor's name
     *  \return ReturnCode_t
     */
    /****************************************************************************/
    ReturnCode_t StartTemperatureControlForPreTest(const QString& HeaterName);

    /****************************************************************************/
    /*!
     *  \brief  Start the specific sensor's temperature control
     *  \param 	HeaterName - sensor's name
     *  \param  NotSureTemperature - bool whether sure temperature
     *  \return ReturnCode_t
     */
    /****************************************************************************/
    ReturnCode_t StartTemperatureControlForSelfTest(const QString& HeaterName, bool NotSureTemperature = false);

    /****************************************************************************/
    /*!
     *  \brief  Start the specific sensor's temperature control
     *  \param 	HeaterName - sensor's name
     *  \return ReturnCode_t
     */
    /****************************************************************************/
    ReturnCode_t StartTemperatureControlForPowerFailure(const QString& HeaterName);

    /****************************************************************************/
    /*!
     *  \brief  Start the specific sensor's temperature control
     *  \param 	HeaterName - sensor's name 
     *  \return ReturnCode_t
     */
    /****************************************************************************/
    ReturnCode_t StartTemperatureControl(const QString& HeaterName);

    /****************************************************************************/
    /*!
     *  \brief  Stop the specific sensor's temperature control
     *  \param 	HeaterName - sensor's name
     *  \return ReturnCode_t
     */
    /****************************************************************************/
    ReturnCode_t StopTemperatureControl(const QString& HeaterName);

    /****************************************************************************/
    /*!
     *  \brief  Check temperature difference of the two Retort bottom sensors
     *  \param  temp1 - temperature of RT Bottom1
     *  \param  temp2 - temperature of RT Bottom2
     *  \return bool true - success, false - failed
     */
    /****************************************************************************/
    bool CheckRTBottomsDifference(qreal temp1, qreal temp2);

    /****************************************************************************/
    /*!
     *  \brief check the sensor heat over time
     *  \param HeaterName - sensor's name
     *  \param HWTemp = qreal temperature
     *  \return 0 - have not get the time out; 1 - failed; 2 - pass
     */
    /****************************************************************************/
    quint16 CheckTemperatureOverTime(const QString& HeaterName, qreal HWTemp);

    /****************************************************************************/
    /*!
     *  \brief check LA Sensor status
     *  \param HeaterName - sensor's name
     *  \param HWTemp = qreal temperature
     *  \return bool true - success, false - failed
     */
    /****************************************************************************/
    bool CheckLASensorStatus(const QString& HeaterName, qreal HWTemp);

    /****************************************************************************/
    /*!
     *  \brief  Initialize parameters when entering scenario 260
     *  \param  void
     *  \return void
     */
    /****************************************************************************/
    void Init260ParamList();

    /****************************************************************************/
    /*!
     *  \brief  check the temperature of sensors in scenario 260
     *  \param  void
     *  \return bool
     */
    /****************************************************************************/
    bool Check260SensorsTemp();

    /****************************************************************************/
    /*!
     *  \brief  Check the status of all the temperature sensors
     *  \return bool
     */
    /****************************************************************************/
    bool CheckTemperatureSenseorsStatus() const;

    /****************************************************************************/
     /*!
     *  \brief  Check if level sensor heating reaches target temperature or not
     *  \param  devName - Qstring
     *  \param  temperature - qreal
     *  \return bool, ture - passed, false - not yet
     */
    /****************************************************************************/
    bool CheckSensorTempOverRange(const QString& devName, qreal temperature);

    /****************************************************************************/
    /*!
     *  \brief  Check if level sensor heating reaches target temperature or not
     *  \return bool, ture - passed, false - not yet
     */
    /****************************************************************************/
    bool CheckLevelSensorHeatingStatus() const { return m_RTLevelSensor.OTCheckPassed; }

    /****************************************************************************/
    /*!
     *  \brief check the temperature wether effective
     *  \param HWTemp = qreal temperature
     *  \return bool
     */
    /****************************************************************************/
    bool isEffectiveTemp(qreal HWTemp);

    /****************************************************************************/
    /*!
     *  \brief reset the heating strategy scenario
     *  \param Scenario = qint32
     *  \return void
     */
    /****************************************************************************/
    void SetHeatingStrategyScenario(qint32 Scenario) {m_CurScenario = Scenario;}

    /****************************************************************************/
    /*!
     *  \brief reset the heating strategy scenario
     *  \return qint32
     */
    /****************************************************************************/
    qint32 GetHeatingStrategyScenario() {return m_CurScenario;}

    /****************************************************************************/
    /*!
     *  \brief reset the Oven heating
     *  \return void
     */
    /****************************************************************************/
    void ResetTheOvenHeating();

private slots:
    /****************************************************************************/
    /*!
     *  \brief  slot function to stop level sensor heating
     *
     */
    /****************************************************************************/
    void OnReportLevelSensorStatus1();
private:
    SchedulerMainThreadController*      mp_SchedulerController;         //!< point class of SchedulerMainThreadController
    SchedulerCommandProcessorBase*      mp_SchedulerCommandProcessor;   //!< point class of SchedulerCommandProcessorBase
    DataManager::CDataManager*          mp_DataManager;                 //!< point class of CDataManager
    qint32								m_CurScenario;                  //!< the current scenario
    RTLevelSensor                       m_RTLevelSensor;                //!< retort of level sensor
    HeatingSensor                       m_RTTop;                        //!< retort of top heat sensor
    RTBottomSensor                      m_RTBottom;                     //!< retort of bottom of heating sensor
    OvenSensor                          m_OvenTop;                      //!< oven of top heat sensor
    OvenSensor                          m_OvenBottom;                   //!< oven of bottom heat sensor
    RVSensor                            m_RV_1_HeatingRod;              //!< rotary valve of heat rod
    RVOutlet							m_RV_2_Outlet;                  //!< rotary valve of outlet
    LASensor                            m_LARVTube;                     //!< LA of tube heat sensor
    LASensor                            m_LAWaxTrap;                    //!< LA of waxTrap
    bool                                m_CmdResult;                    //!< flag to indicate command result
    bool                                m_DiasbleOvenHeatingError;      //!< disable heating strategy error
    SensorsChecking                     m_SensorsChecking;              //!< Checking list of sensors in scenario 260

    /****************************************************************************/
    /*!
     *  \brief construct initialize the heating sensor list
     *  \return form bool
     */
    /****************************************************************************/
    bool ConstructHeatingSensorList();

    /****************************************************************************/
    /*!
     *  \brief construct initialize the heating sensor list
     *  \param heatingSensor = struct HeatingSensor
     *  \param sequenceList = QstringList
     *  \return form bool
     */
    /****************************************************************************/
    bool ConstructHeatingSensor(HeatingSensor& heatingSensor, const QStringList& sequenceList);

    /****************************************************************************/
    /*!
     *  \brief check the current temperature of heat sensor
     *  \param heatingSensor = struct HeatingSensor
     *  \param HWTemp is qreal of tmperature
     *  \return form bool
     */
    /****************************************************************************/
	inline bool CheckSensorCurrentTemperature(const HeatingSensor& heatingSensor, qreal HWTemp);

    /****************************************************************************/
    /*!
     *  \brief  Start Level Sensor Temperature control
     *  \param  strctHWMonitor - structure of hardware parameters
     *  \return ReturnCode_t
     */
    /****************************************************************************/
    DeviceControl::ReturnCode_t StartLevelSensorTemperatureControl(const HardwareMonitor_t& strctHWMonitor);

    /****************************************************************************/
    /*!
     *  \brief set the retort heat sensor of temperature
     *  \param heatingSensor = struct HeatingSensor
     *  \param RTType = enum RTTempCtrlType_t
     *  \return form ReturnCode_t
     */
    /****************************************************************************/
    DeviceControl::ReturnCode_t StartRTTemperatureControl(HeatingSensor& heatingSensor, RTTempCtrlType_t RTType);

    /****************************************************************************/
    /*!
     *  \brief set the oven temperature
     *  \param heatingSensor = struct OvenSensor
     *  \param OvenType = enum OVENTempCtrlType_t
     *  \return form ReturnCode_t
     */
    /****************************************************************************/
    DeviceControl::ReturnCode_t StartOvenTemperatureControl(OvenSensor& heatingSensor, OVENTempCtrlType_t OvenType);

    /****************************************************************************/
    /*!
     *  \brief set rotary valve temperature
     *  \param heatingSensor = struct RVSensor
     *  \return form ReturnCode_t
     */
    /****************************************************************************/
    DeviceControl::ReturnCode_t StartRVTemperatureControl(RVSensor& heatingSensor);

    /****************************************************************************/
    /*!
     *  \brief calculate the ratary valve of heating times
     */
    /****************************************************************************/
    inline void StartRVOutletHeatingOTCalculation();

    /****************************************************************************/
    /*!
     *  \brief set LA temperatur
     *  \param heatingSensor = struct LASensor
     *  \param LAType = enum ALTempCtrlType_t
     *  \return form ReturnCode_t
     */
    /****************************************************************************/
    DeviceControl::ReturnCode_t StartLATemperatureControl(HeatingSensor& heatingSensor,ALTempCtrlType_t LAType);

    /****************************************************************************/
    /*!
     *  \brief check the sensor heat over time or not
     *  \param heatingSensor = struct HeatingSensor
     *  \param HWTemp = qreal temperature
     *  \return bool
     */
    /****************************************************************************/
    inline bool CheckSensorHeatingOverTime(HeatingSensor& heatingSensor, qreal HWTemp);

    /****************************************************************************/
    /*!
     *  \brief check Rotary valve outlet heating over time or not
     *  \param HWTemp = qreal temperature
     *  \return bool
     */
    /****************************************************************************/
	inline bool CheckRVOutletHeatingOverTime(qreal HWTemp);

    /****************************************************************************/
    /*!
     *  \brief check Oven heating over time or not
     *  \param heatingSensor = OvenSensor
     *  \param HWTemp = qreal temperature
     *  \param OvenType = OVENSensorType_t
     *  \return ReturnCode_t
     */
    /****************************************************************************/
    DeviceControl::ReturnCode_t CheckOvenHeatingOverTime(OvenSensor& heatingSensor, qreal HWTemp, OVENSensorType_t OvenType);

private:
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function HeatingStrategy
     *
     *  \param rhs = const HeatingStrategy type parameter
     *
     *  \return from HeatingStrategy
     */
    /****************************************************************************/
    HeatingStrategy(const HeatingStrategy& rhs);
    /****************************************************************************/
    /*!
     *  \brief  Definition/Declaration of function operator=
     *
     *  \param  rhs
     *
     *  \return from operator=
     */
    /****************************************************************************/
    HeatingStrategy& operator=(const HeatingStrategy& rhs);
};
}
#endif // SCHEDULERMACHINE_H

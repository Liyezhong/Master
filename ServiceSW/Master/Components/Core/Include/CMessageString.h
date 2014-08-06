#ifndef SERVICE_CMESSAGESTRING_H
#define SERVICE_CMESSAGESTRING_H

#include <QString>

namespace Service {

class CMessageString
{
public:
	static QString MSG_TITLE_ERROR;                                   //!<Error
	static QString MSG_TITLE_INFO;                                    //!<Information
	static QString MSG_TITLE_WARN;                                    //!<Warning
	static QString MSG_TITLE_WAIT;                                    //!<Please Wait
	static QString MSG_TITLE_COMMUNCATION_ERROR;                      //!<Communication Error
	static QString MSG_TITLE_SERIAL_NUMBER;                           //!<Serial Number
	static QString MSG_TITLE_ENTER_SERILA_NUMBER;                     //!<Enter Serial Number
	static QString MSG_TITLE_SEND_REPORT;                             //!<Send Report
	static QString MSG_TITLE_USER_INPUT;                              //!<User Input
	static QString MSG_TITLE_ENTER_VALVE;                             //!<Enter Value
	static QString MSG_TITLE_SEALING_TEST;                            //!<Sealing Test
	static QString MSG_TITLE_CONFIRM_WATER_LEVEL;                     //!<Confirm water level

	static QString MSG_BUTTON_OK;                                     //!<Ok
	static QString MSG_BUTTON_NO;                                     //!<No
	static QString MSG_BUTTON_YES;                                    //!<Yes
	static QString MSG_BUTTON_NEXT;                                   //!<Next
	static QString MSG_BUTTON_CANCEL;                                 //!<Cancel
	static QString MSG_BUTTON_CLOSE;                                  //!<Close
	static QString MSG_BUTTON_ABORT;                                  //!<Abort
	static QString MSG_BUTTON_PASS;                                   //!<PASS
	static QString MSG_BUTTON_FAIL;                                   //!<FAIL
	static QString MSG_BUTTON_RETEST;                                 //!<Retest

	static QString MSG_DIAGNOSTICS_DISPLAY;                           //!<Display
	static QString MSG_DIAGNOSTICS_MC;                                //!<Main Control
	static QString MSG_DIAGNOSTICS_RETORT;                            //!<Retort
	static QString MSG_DIAGNOSTICS_OVEN;                              //!<Paraffin Oven
	static QString MSG_DIAGNOSTICS_RV;                                //!<Rotary Valve
	static QString MSG_DIAGNOSTICS_LA;                                //!<LA System
	static QString MSG_DIAGNOSTICS_SYSTEM;                            //!<System
	static QString MSG_DIAGNOSTICS_CLEANING;                          //!<Cleaning
	static QString MSG_DIAGNOSTICS_CLEANING_BEGIN;                    //!<Begin cleaning...
	static QString MSG_DIAGNOSTICS_DIAGNOSTICS;                       //!<Diagnostics
	static QString MSG_DIAGNOSTICS_FAILED;                            //!<Failed
	static QString MSG_DIAGNOSTICS_SUCCESS;                           //!<Success
	static QString MSG_DIAGNOSTICS_ENTER_SN;                          //!<Please enter the serial number.
	static QString MSG_DIAGNOSTICS_ENTER_SYSTEM_SN;                   //!<Please enter the system serial number.
	static QString MSG_DIAGNOSTICS_SELECT_TEST_CASE;                  //!<Please select a test case.
	static QString MSG_DIAGNOSTICS_SENDING;                           //!<Sending...
	static QString MSG_DIAGNOSTICS_SEND_REPORT_OK;                    //!<Send test report ok.
	static QString MSG_DIAGNOSTICS_GEN_REPORT_FILE_FAILED;            //!<Generation report file failed.
	static QString MSG_DIAGNOSTICS_REPORT_FILE_NOT_EXISTS;            //!<The report file is not exists.
	static QString MSG_DIAGNOSTICS_TEST_NUMBER;                       //!<Nr.
	static QString MSG_DIAGNOSTICS_TEST_NAME;                         //!<Tests
	static QString MSG_DIAGNOSTICS_CURRENT_TEMP;                      //!<Current Temperature:
	static QString MSG_DIAGNOSTICS_CURRENT_TEMP_SENSOR1;              //!<Current Temperature (Sensor 1):
	static QString MSG_DIAGNOSTICS_CURRENT_TEMP_SENSOR2;              //!<Current Temperature (Sensor 2):
	static QString MSG_DIAGNOSTICS_CURRENT_TEMP_SIDE;                 //!<Current Temperature (Side):
	static QString MSG_DIAGNOSTICS_CURRENT_TEMP_BOTTOM1;              //!<Current Temperature (Bottom 1):
	static QString MSG_DIAGNOSTICS_CURRENT_TEMP_BOTTOM2;              //!<Current Temperature (Bottom 2):
	static QString MSG_DIAGNOSTICS_LEFT_SENSOR_TEMP;                  //!<Left Sensor Temperature:
	static QString MSG_DIAGNOSTICS_MIDDLE_SENSOR_TEMP;                //!<Middle Sensor Temperature:
	static QString MSG_DIAGNOSTICS_RIGHT_SENSOR_TEMP;                 //!<Right Sensor Temperature:
	static QString MSG_DIAGNOSTICS_EXTERNAL_SENSOR;                   //!<External sensor value:
	static QString MSG_DIAGNOSTICS_EXTERNAL_SENSOR_AGAIN;             //!<External sensor value (again):
	static QString MSG_DIAGNOSTICS_ENTER_VALVES_NOT_SAME;             //!<The two values you input are not same. Please check again.
	static QString MSG_DIAGNOSTICS_LEFT_SENSOR_VALVE_WRONG;           //!<The value of Left sensor is wrong
	static QString MSG_DIAGNOSTICS_EXTERNAL_SENSOR_VALVE_WRONG;       //!<The external sensor value is wrong
	static QString MSG_DIAGNOSTICS_MIDDLE_SENSOR_VALVE_WRONG;         //!<The value of Middle sensor is wrong
	static QString MSG_DIAGNOSTICS_EXTERNAL_SENSOR_VALVE_WRONG_AGAIN; //!<The external sensor value (again) is wrong
	static QString MSG_DIAGNOSTICS_RIGHT_SENSOR_VALVE_WRONG;          //!<The value of Right sensor is wrong
	static QString MSG_DIAGNOSTICS_USER_INPUT_PRESSURE;               //!<The value of Original Pressure should be in
	static QString MSG_DIAGNOSTICS_POSITION;                          //!<Position
	static QString MSG_DIAGNOSTICS_POSITION_READY;                    //!<Positon %1# is ready.
	static QString MSG_DIAGNOSTICS_IF_TEST_PASS;                      //!<Is the test pass?
	static QString MSG_DIAGNOSTICS_CHECK_VENRILATION_FAN;             //!<Please check if the ventilation fan is runing and check if the air flow direction is out of device
	static QString MSG_DIAGNOSTICS_ADJUST_LID_LOCK;                   //!<Please adjust retort lid lock assembly then re-test...
	static QString MSG_DIAGNOSTICS_ASB_OUPUT_VOLTAGE_FAILED;          //!<ASB %1 DC output voltage is failed (%2 V). <br>and current is failed (%3 mA).
	static QString MSG_DIAGNOSTICS_ASB_OUPUT_VOLTAGE_OK;              //!<ASB %1 DC output voltage is Ok (%2 V). <br>and current is Ok (%3 mA).
	static QString MSG_DIAGNOSTICS_TEST_COVER_SENSOR;                 //!<Open the retort lid. do you see water cover the level sensor?
	static QString MSG_DIAGNOSTICS_RV_SELECT_TEST;                    //!<Rotary valve selecting test in progress...
	static QString MSG_DIAGNOSTICS_RV_SEALINT_TEST;                   //!<Rotary valve sealing test in progress...
	static QString MSG_DIAGNOSTICS_VOLTAGE_TEST;                      //!<%1 V test
	static QString MSG_DIAGNOSTICS_CURRENT_VOLTAGE;                   //!<(Current Voltage: %1 V)
	static QString MSG_DIAGNOSTICS_STATUS_CLOSE;                      //!<Close
	static QString MSG_DIAGNOSTICS_STATUS_OPEN;                       //!<Open
	static QString MSG_DIAGNOSTICS_STATUS_DISCONNECTED;               //!<Disconnected
	static QString MSG_DIAGNOSTICS_STATUS_CONNECTED;                  //!<Connected
	static QString MSG_DIAGNOSTICS_UPDATE_FIRMWARE_SLAVE;             //!<Update Firmware for Slave %1. please wait...
	static QString MSG_DIAGNOSTICS_REQUESTED_OPERATION;               //!<Requested operation is in progress...
	static QString MSG_DIAGNOSTICS_ROTATE_RV_TO_TUBE;                 //!<Rotate rotary valve to tube position #%1
	static QString MSG_DIAGNOSTICS_ROTATE_RV_TO_SEALINT;              //!<Rotate rotary valve to sealing position #%1
	static QString MSG_DIAGNOSTICS_CREATING_PRESSURE;                 //!<Creating pressure to %1Kpa...
	static QString MSG_DIAGNOSTICS_KEEP_PRESSURE;                     //!<Keep pressure for %1 seconds
	static QString MSG_DIAGNOSTICS_RELEASING_PRESSURE;                //!<Releasing pressure...
	static QString MSG_DIAGNOSTICS_INIT_RV;                           //!<Rotary valve is initializing...
	static QString MSG_DIAGNOSTICS_HEATING_LEVEL_SENSOR;              //!<Heating level sensor...
	static QString MSG_DIAGNOSTICS_FILLING;                           //!<Filling...
	static QString MSG_DIAGNOSTICS_DRAINING;                          //!<Draining...
	static QString MSG_DIAGNOSTICS_AIR_BLOWING;                       //!<Air Blowing...
	static QString MSG_DIAGNOSTICS_TEST_FINISH;                       //!<Test is finished.
	static QString MSG_DIAGNOSTICS_ROTATE_RV_TO_TUBE_FAILED;          //!<Rotate rotary valve to tube position %1 failed
	static QString MSG_DIAGNOSTICS_ROTATE_RV_TO_SEALING_FAILED;       //!<Rotate rotary valve to sealing position %1 failed
	static QString MSG_DIAGNOSTICS_CREATE_PRESSURE_FALIED;            //!<create pressure failed.
	static QString MSG_DIAGNOSTICS_FILLING_FAILED;                    //!<Filling is failed.
	static QString MSG_DIAGNOSTICS_HEATING_LEVEL_SENSOR_FAILED;       //!<Level sensor heating is failed.
	static QString MSG_DIAGNOSTICS_INIT_RV_FAILED;                    //!<Rotary valve initializing is failed.
	static QString MSG_DIAGNOSTICS_OVEN_TEMP_NO_MATCH;                //!<Oven Current Temperature is (%1 %2 %3) which is not in (%4~%5)
	static QString MSG_DIAGNOSTICS_RV_TEMP_NO_MATCH;                  //!<Rotary Valve Current Temperature is (%1 %2) which is not in (%3~%4)
	static QString MSG_DIAGNOSTICS_LEVEL_SENSOR_TEMP_NO_MATCH;        //!<Level Sensor Current Temperature is (%1) which is not in (%2~%3)
	static QString MSG_DIAGNOSTICS_RETORT_TEMP_NO_MATCH;              //!<Retort level sensor current temperature is (%1) which is less than target (%2) in %3 seconds
	static QString MSG_DIAGNOSTICS_TUBE_TEMP_NO_MATCH;                //!<Tube Current Temperature is (%1) which is not in (%2~%3)

	static QString MSG_DEVICECOMMAND_TIMEOUT;                         //!<ERROR: Device is not responding restart the system!

	static QString MSG_MANUFACTURINGTEST_RESULT;                      //!<Manufacturing Test Result

	static QString MSG_SERVER_IP_CANNOT_REACHABLE;                    //!<The server ip can't reachable.
	static QString MSG_SERVER_FOLDER_CANNOT_ACCESS;                   //!<Server folder can't access.
	static QString MSG_SERVER_DOWNLOAD_FILES_FAILED;                  //!<Download firmware files failed.
	static QString MSG_SERVER_SEND_REPORT_FALIED;                     //!<Send report file to server failed.

	static QString MSG_DEVICE_NOTYET_READY;                           //!<Error: Device Layer not yet Initialized. Please retry after some time...
	static QString MSG_DEVICE_RELEASE_PRESSURE_FAILED;                //!<ERROR: Release pressure failure.
	static QString MSG_DEVICE_PRESSURE_SENSOR_CALIBRATION_FAILED;     //!<ERROR: Pressure sensor calibration failure. Please confirm the air system is not blocked and open the retort lid do calibrate again!
	static QString MSG_DEVICE_PRESSURE_SENSOR_CALIBRATION_SUCCESS;    //!<Pressure sensor calibration is successful! Pressure drift value(Kpa):


	static void RetranslateUI();
};

}// end of namespace Service

#endif // SERVICE_CMESSAGESTRING_H
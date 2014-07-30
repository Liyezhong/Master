#include <Core/Include/CMessageString.h>
#include <QApplication>

namespace Service {

QString CMessageString::MSG_TITLE_ERROR;                                   //!<Error
QString CMessageString::MSG_TITLE_INFO;                                    //!<Information
QString CMessageString::MSG_TITLE_WARN;                                    //!<Warning
QString CMessageString::MSG_TITLE_WAIT;                                    //!<Please Wait
QString CMessageString::MSG_TITLE_COMMUNCATION_ERROR;                      //!<Communication Error
QString CMessageString::MSG_TITLE_SERIAL_NUMBER;                           //!<Serial Number
QString CMessageString::MSG_TITLE_ENTER_SERILA_NUMBER;                     //!<Enter Serial Number
QString CMessageString::MSG_TITLE_SEND_REPORT;                             //!<Send Report
QString CMessageString::MSG_TITLE_USER_INPUT;                              //!<User Input
QString CMessageString::MSG_TITLE_ENTER_VALVE;                             //!<Enter Value
QString CMessageString::MSG_TITLE_SEALING_TEST;                            //!<Sealing Test
QString CMessageString::MSG_TITLE_CONFIRM_WATER_LEVEL;                     //!<Confirm water level

QString CMessageString::MSG_BUTTON_OK;                                     //!<Ok
QString CMessageString::MSG_BUTTON_NO;                                     //!<No
QString CMessageString::MSG_BUTTON_YES;                                    //!<Yes
QString CMessageString::MSG_BUTTON_NEXT;                                   //!<Next
QString CMessageString::MSG_BUTTON_CANCEL;                                 //!<Cancel
QString CMessageString::MSG_BUTTON_CLOSE;                                  //!<Close
QString CMessageString::MSG_BUTTON_ABORT;                                  //!<Abort
QString CMessageString::MSG_BUTTON_PASS;                                   //!<PASS
QString CMessageString::MSG_BUTTON_FAIL;                                   //!<FAIL
QString CMessageString::MSG_BUTTON_RETEST;                                 //!<Retest

QString CMessageString::MSG_DIAGNOSTICS_DISPLAY;                           //!<Display
QString CMessageString::MSG_DIAGNOSTICS_MC;                                //!<Main Control
QString CMessageString::MSG_DIAGNOSTICS_RETORT;                            //!<Retort
QString CMessageString::MSG_DIAGNOSTICS_OVEN;                              //!<Paraffin Oven
QString CMessageString::MSG_DIAGNOSTICS_RV;                                //!<Rotary Valve
QString CMessageString::MSG_DIAGNOSTICS_LA;                                //!<LA System
QString CMessageString::MSG_DIAGNOSTICS_SYSTEM;                            //!<System
QString CMessageString::MSG_DIAGNOSTICS_CLEANING;                          //!<Cleaning
QString CMessageString::MSG_DIAGNOSTICS_CLEANING_BEGIN;                    //!<Begin cleaning...
QString CMessageString::MSG_DIAGNOSTICS_DIAGNOSTICS;                       //!<Diagnostics
QString CMessageString::MSG_DIAGNOSTICS_FAILED;                            //!<Failed
QString CMessageString::MSG_DIAGNOSTICS_SUCCESS;                           //!<Success
QString CMessageString::MSG_DIAGNOSTICS_ENTER_SN;                          //!<Please enter the serial number.
QString CMessageString::MSG_DIAGNOSTICS_ENTER_SYSTEM_SN;                   //!<Please enter the system serial number.
QString CMessageString::MSG_DIAGNOSTICS_SELECT_TEST_CASE;                  //!<Please select a test case.
QString CMessageString::MSG_DIAGNOSTICS_SENDING;                           //!<Sending...
QString CMessageString::MSG_DIAGNOSTICS_SEND_REPORT_OK;                    //!<Send test report ok.
QString CMessageString::MSG_DIAGNOSTICS_SEND_REPORT_FAILED;                //!<Send test report failed.
QString CMessageString::MSG_DIAGNOSTICS_TEST_NUMBER;                       //!<Nr.
QString CMessageString::MSG_DIAGNOSTICS_TEST_NAME;                         //!<Tests
QString CMessageString::MSG_DIAGNOSTICS_CURRENT_TEMP;                      //!<Current Temperature:
QString CMessageString::MSG_DIAGNOSTICS_CURRENT_TEMP_SENSOR1;              //!<Current Temperature (Sensor 1):
QString CMessageString::MSG_DIAGNOSTICS_CURRENT_TEMP_SENSOR2;              //!<Current Temperature (Sensor 2):
QString CMessageString::MSG_DIAGNOSTICS_CURRENT_TEMP_SIDE;                 //!<Current Temperature (Side):
QString CMessageString::MSG_DIAGNOSTICS_CURRENT_TEMP_BOTTOM1;              //!<Current Temperature (Bottom 1):
QString CMessageString::MSG_DIAGNOSTICS_CURRENT_TEMP_BOTTOM2;              //!<Current Temperature (Bottom 2):
QString CMessageString::MSG_DIAGNOSTICS_LEFT_SENSOR_TEMP;                  //!<Left Sensor Temperature:
QString CMessageString::MSG_DIAGNOSTICS_MIDDLE_SENSOR_TEMP;                //!<Middle Sensor Temperature:
QString CMessageString::MSG_DIAGNOSTICS_RIGHT_SENSOR_TEMP;                 //!<Right Sensor Temperature:
QString CMessageString::MSG_DIAGNOSTICS_EXTERNAL_SENSOR;                   //!<External sensor value:
QString CMessageString::MSG_DIAGNOSTICS_EXTERNAL_SENSOR_AGAIN;             //!<External sensor value (again):
QString CMessageString::MSG_DIAGNOSTICS_ENTER_VALVES_NOT_SAME;             //!<The two values you input are not same. Please check again.
QString CMessageString::MSG_DIAGNOSTICS_LEFT_SENSOR_VALVE_WRONG;           //!<The value of Left sensor is wrong
QString CMessageString::MSG_DIAGNOSTICS_EXTERNAL_SENSOR_VALVE_WRONG;       //!<The external sensor value is wrong
QString CMessageString::MSG_DIAGNOSTICS_MIDDLE_SENSOR_VALVE_WRONG;         //!<The value of Middle sensor is wrong
QString CMessageString::MSG_DIAGNOSTICS_EXTERNAL_SENSOR_VALVE_WRONG_AGAIN; //!<The external sensor value (again) is wrong
QString CMessageString::MSG_DIAGNOSTICS_RIGHT_SENSOR_VALVE_WRONG;          //!<The value of Right sensor is wrong
QString CMessageString::MSG_DIAGNOSTICS_USER_INPUT_PRESSURE;               //!<The value of Original Pressure should be in
QString CMessageString::MSG_DIAGNOSTICS_POSITION;                          //!<Position
QString CMessageString::MSG_DIAGNOSTICS_POSITION_READY;                    //!<Positon %1# is ready.
QString CMessageString::MSG_DIAGNOSTICS_IF_TEST_PASS;                      //!<Is the test pass?
QString CMessageString::MSG_DIAGNOSTICS_CHECK_VENRILATION_FAN;             //!<Please check if the ventilation fan is runing and check if the air flow direction is out of device
QString CMessageString::MSG_DIAGNOSTICS_ADJUST_LID_LOCK;                   //!<Please adjust retort lid lock assembly then re-test...
QString CMessageString::MSG_DIAGNOSTICS_ASB_OUPUT_VOLTAGE_FAILED;          //!<ASB %1 DC output voltage is failed (%2 V). <br>and current is failed (%3 mA).
QString CMessageString::MSG_DIAGNOSTICS_ASB_OUPUT_VOLTAGE_OK;              //!<ASB %1 DC output voltage is Ok (%2 V). <br>and current is Ok (%3 mA).
QString CMessageString::MSG_DIAGNOSTICS_TEST_COVER_SENSOR;                 //!<Open the retort lid. do you see water cover the level sensor?
QString CMessageString::MSG_DIAGNOSTICS_RV_SELECT_TEST;                    //!<Rotary valve selecting test in progress...
QString CMessageString::MSG_DIAGNOSTICS_RV_SEALINT_TEST;                   //!<Rotary valve sealing test in progress...
QString CMessageString::MSG_DIAGNOSTICS_VOLTAGE_TEST;                      //!<%1 V test
QString CMessageString::MSG_DIAGNOSTICS_CURRENT_VOLTAGE;                   //!<(Current Voltage: %1 V)
QString CMessageString::MSG_DIAGNOSTICS_STATUS_CLOSE;                      //!<Close
QString CMessageString::MSG_DIAGNOSTICS_STATUS_OPEN;                       //!<Open
QString CMessageString::MSG_DIAGNOSTICS_STATUS_DISCONNECTED;               //!<Disconnected
QString CMessageString::MSG_DIAGNOSTICS_STATUS_CONNECTED;                  //!<Connected
QString CMessageString::MSG_DIAGNOSTICS_UPDATE_FIRMWARE_SLAVE;             //!<Update Firmware for Slave %1. please wait...
QString CMessageString::MSG_DIAGNOSTICS_REQUESTED_OPERATION;               //!<Requested operation is in progress...
QString CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_TUBE;                 //!<Rotate rotary valve to tube position #%1
QString CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_SEALINT;              //!<Rotate rotary valve to sealing position #%1
QString CMessageString::MSG_DIAGNOSTICS_CREATING_PRESSURE;                 //!<Creating pressure to %1Kpa...
QString CMessageString::MSG_DIAGNOSTICS_KEEP_PRESSURE;                     //!<Keep pressure for %1 seconds
QString CMessageString::MSG_DIAGNOSTICS_RELEASING_PRESSURE;                //!<Releasing pressure...
QString CMessageString::MSG_DIAGNOSTICS_INIT_RV;                           //!<Rotary valve is initializing...
QString CMessageString::MSG_DIAGNOSTICS_HEATING_LEVEL_SENSOR;              //!<Heating level sensor...
QString CMessageString::MSG_DIAGNOSTICS_FILLING;                           //!<Filling...
QString CMessageString::MSG_DIAGNOSTICS_DRAINING;                          //!<Draining...
QString CMessageString::MSG_DIAGNOSTICS_AIR_BLOWING;                       //!<Air Blowing...
QString CMessageString::MSG_DIAGNOSTICS_TEST_FINISH;                       //!<Test is finished.
QString CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_TUBE_FAILED;          //!<Rotate rotary valve to tube position %1 failed
QString CMessageString::MSG_DIAGNOSTICS_ROTATE_RV_TO_SEALING_FAILED;       //!<Rotate rotary valve to sealing position %1 failed
QString CMessageString::MSG_DIAGNOSTICS_CREATE_PRESSURE_FALIED;            //!<create pressure failed.
QString CMessageString::MSG_DIAGNOSTICS_FILLING_FAILED;                    //!<Filling is failed.
QString CMessageString::MSG_DIAGNOSTICS_HEATING_LEVEL_SENSOR_FAILED;       //!<Level sensor heating is failed.
QString CMessageString::MSG_DIAGNOSTICS_INIT_RV_FAILED;                    //!<Rotary valve initializing is failed.
QString CMessageString::MSG_DIAGNOSTICS_OVEN_TEMP_NO_MATCH;                //!<Oven Current Temperature is (%1 %2 %3) which is not in (%4~%5)
QString CMessageString::MSG_DIAGNOSTICS_RV_TEMP_NO_MATCH;                  //!<Rotary Valve Current Temperature is (%1 %2) which is not in (%3~%4)
QString CMessageString::MSG_DIAGNOSTICS_LEVEL_SENSOR_TEMP_NO_MATCH;        //!<Level Sensor Current Temperature is (%1) which is not in (%2~%3)
QString CMessageString::MSG_DIAGNOSTICS_RETORT_TEMP_NO_MATCH;              //!<Retort level sensor current temperature is (%1) which is less than target (%2) in %3 seconds
QString CMessageString::MSG_DIAGNOSTICS_TUBE_TEMP_NO_MATCH;                //!<Tube Current Temperature is (%1) which is not in (%2~%3)

QString CMessageString::MSG_DEVICECOMMAND_TIMEOUT;                         //!<ERROR: Device is not responding restart the system!

QString CMessageString::MSG_MANUFACTURINGTEST_RESULT;                      //!<Manufacturing Test Result

QString CMessageString::MSG_DEVICE_NOTYET_READY;                           //!<Error: Device Layer not yet Initialized. Please retry after some time...
QString CMessageString::MSG_DEVICE_RELEASE_PRESSURE_FAILED;                //!<ERROR: Release pressure failure.
QString CMessageString::MSG_DEVICE_PRESSURE_SENSOR_CALIBRATION_FAILED;     //!<ERROR: Pressure sensor calibration failure. Please confirm the air system is not blocked and open the retort lid do calibrate again!
QString CMessageString::MSG_DEVICE_PRESSURE_SENSOR_CALIBRATION_SUCCESS;    //!<Pressure sensor calibration is successful! Pressure drift value(Kpa):

void CMessageString::RetranslateUI()
{
	MSG_TITLE_ERROR                                   = (QApplication::translate("Service", "Error", 0, QApplication::UnicodeUTF8));
	MSG_TITLE_INFO                                    = (QApplication::translate("Service", "Information", 0, QApplication::UnicodeUTF8));
	MSG_TITLE_WARN                                    = (QApplication::translate("Service", "Warning", 0, QApplication::UnicodeUTF8));
	MSG_TITLE_WAIT                                    = (QApplication::translate("Service", "Please Wait", 0, QApplication::UnicodeUTF8));
	MSG_TITLE_COMMUNCATION_ERROR                      = (QApplication::translate("Service", "Communication Error", 0, QApplication::UnicodeUTF8));
	MSG_TITLE_SERIAL_NUMBER                           = (QApplication::translate("Service", "Serial Number", 0, QApplication::UnicodeUTF8));
	MSG_TITLE_ENTER_SERILA_NUMBER                     = (QApplication::translate("Service", "Enter Serial Number", 0, QApplication::UnicodeUTF8));
	MSG_TITLE_SEND_REPORT                             = (QApplication::translate("Service", "Send Report", 0, QApplication::UnicodeUTF8));
	MSG_TITLE_USER_INPUT                              = (QApplication::translate("Service", "User Input", 0, QApplication::UnicodeUTF8));
	MSG_TITLE_ENTER_VALVE                             = (QApplication::translate("Service", "Enter Value", 0, QApplication::UnicodeUTF8));
	MSG_TITLE_SEALING_TEST                            = (QApplication::translate("Service", "Sealing Test", 0, QApplication::UnicodeUTF8));
	MSG_TITLE_CONFIRM_WATER_LEVEL                     = (QApplication::translate("Service", "Confirm water level", 0, QApplication::UnicodeUTF8));
	MSG_BUTTON_OK                                     = (QApplication::translate("Service", "Ok", 0, QApplication::UnicodeUTF8));
	MSG_BUTTON_NO                                     = (QApplication::translate("Service", "No", 0, QApplication::UnicodeUTF8));
	MSG_BUTTON_YES                                    = (QApplication::translate("Service", "Yes", 0, QApplication::UnicodeUTF8));
	MSG_BUTTON_NEXT                                   = (QApplication::translate("Service", "Next", 0, QApplication::UnicodeUTF8));
	MSG_BUTTON_CANCEL                                 = (QApplication::translate("Service", "Cancel", 0, QApplication::UnicodeUTF8));
	MSG_BUTTON_CLOSE                                  = (QApplication::translate("Service", "Close", 0, QApplication::UnicodeUTF8));
	MSG_BUTTON_ABORT                                  = (QApplication::translate("Service", "Abort", 0, QApplication::UnicodeUTF8));
	MSG_BUTTON_PASS                                   = (QApplication::translate("Service", "PASS", 0, QApplication::UnicodeUTF8));
	MSG_BUTTON_FAIL                                   = (QApplication::translate("Service", "FAIL", 0, QApplication::UnicodeUTF8));
	MSG_BUTTON_RETEST                                 = (QApplication::translate("Service", "Retest", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_DISPLAY                           = (QApplication::translate("Service", "Display", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_MC                                = (QApplication::translate("Service", "Main Control", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_RETORT                            = (QApplication::translate("Service", "Retort", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_OVEN                              = (QApplication::translate("Service", "Paraffin Oven", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_RV                                = (QApplication::translate("Service", "Rotary Valve", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_LA                                = (QApplication::translate("Service", "LA System", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_SYSTEM                            = (QApplication::translate("Service", "System", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_CLEANING                          = (QApplication::translate("Service", "Cleaning", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_CLEANING_BEGIN                    = (QApplication::translate("Service", "Begin cleaning...", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_DIAGNOSTICS                       = (QApplication::translate("Service", "Diagnostics", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_FAILED                            = (QApplication::translate("Service", "Failed", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_SUCCESS                           = (QApplication::translate("Service", "Success", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_ENTER_SN                          = (QApplication::translate("Service", "Please enter the serial number.", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_ENTER_SYSTEM_SN                   = (QApplication::translate("Service", "Please enter the system serial number.", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_SELECT_TEST_CASE                  = (QApplication::translate("Service", "Please select a test case.", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_SENDING                           = (QApplication::translate("Service", "Sending...", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_SEND_REPORT_OK                    = (QApplication::translate("Service", "Send test report ok.", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_SEND_REPORT_FAILED                = (QApplication::translate("Service", "Send test report failed.", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_TEST_NUMBER                       = (QApplication::translate("Service", "Nr.", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_TEST_NAME                         = (QApplication::translate("Service", "Tests", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_CURRENT_TEMP                      = (QApplication::translate("Service", "Current Temperature:", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_CURRENT_TEMP_SENSOR1              = (QApplication::translate("Service", "Current Temperature (Sensor 1):", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_CURRENT_TEMP_SENSOR2              = (QApplication::translate("Service", "Current Temperature (Sensor 2):", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_CURRENT_TEMP_SIDE                 = (QApplication::translate("Service", "Current Temperature (Side):", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_CURRENT_TEMP_BOTTOM1              = (QApplication::translate("Service", "Current Temperature (Bottom 1):", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_CURRENT_TEMP_BOTTOM2              = (QApplication::translate("Service", "Current Temperature (Bottom 2):", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_LEFT_SENSOR_TEMP                  = (QApplication::translate("Service", "Left Sensor Temperature:", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_MIDDLE_SENSOR_TEMP                = (QApplication::translate("Service", "Middle Sensor Temperature:", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_RIGHT_SENSOR_TEMP                 = (QApplication::translate("Service", "Right Sensor Temperature:", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_EXTERNAL_SENSOR                   = (QApplication::translate("Service", "External sensor value:", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_EXTERNAL_SENSOR_AGAIN             = (QApplication::translate("Service", "External sensor value (again):", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_ENTER_VALVES_NOT_SAME             = (QApplication::translate("Service", "The two values you input are not same. Please check again.", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_LEFT_SENSOR_VALVE_WRONG           = (QApplication::translate("Service", "The value of Left sensor is wrong", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_EXTERNAL_SENSOR_VALVE_WRONG       = (QApplication::translate("Service", "The external sensor value is wrong", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_MIDDLE_SENSOR_VALVE_WRONG         = (QApplication::translate("Service", "The value of Middle sensor is wrong", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_EXTERNAL_SENSOR_VALVE_WRONG_AGAIN = (QApplication::translate("Service", "The external sensor value (again) is wrong", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_RIGHT_SENSOR_VALVE_WRONG          = (QApplication::translate("Service", "The value of Right sensor is wrong", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_USER_INPUT_PRESSURE               = (QApplication::translate("Service", "The value of Original Pressure should be in", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_POSITION                          = (QApplication::translate("Service", "Position", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_POSITION_READY                    = (QApplication::translate("Service", "Positon %1# is ready.", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_IF_TEST_PASS                      = (QApplication::translate("Service", "Is the test pass?", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_CHECK_VENRILATION_FAN             = (QApplication::translate("Service", "Please check if the ventilation fan is runing and check if the air flow direction is out of device", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_ADJUST_LID_LOCK                   = (QApplication::translate("Service", "Please adjust retort lid lock assembly then re-test...", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_ASB_OUPUT_VOLTAGE_FAILED          = (QApplication::translate("Service", "ASB %1 DC output voltage is failed (%2 V). <br>and current is failed (%3 mA).", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_ASB_OUPUT_VOLTAGE_OK              = (QApplication::translate("Service", "ASB %1 DC output voltage is Ok (%2 V). <br>and current is Ok (%3 mA).", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_TEST_COVER_SENSOR                 = (QApplication::translate("Service", "Open the retort lid. do you see water cover the level sensor?", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_RV_SELECT_TEST                    = (QApplication::translate("Service", "Rotary valve selecting test in progress...", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_RV_SEALINT_TEST                   = (QApplication::translate("Service", "Rotary valve sealing test in progress...", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_VOLTAGE_TEST                      = (QApplication::translate("Service", "%1 V test", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_CURRENT_VOLTAGE                   = (QApplication::translate("Service", "(Current Voltage: %1 V)", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_STATUS_CLOSE                      = (QApplication::translate("Service", "Close", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_STATUS_OPEN                       = (QApplication::translate("Service", "Open", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_STATUS_DISCONNECTED               = (QApplication::translate("Service", "Disconnected", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_STATUS_CONNECTED                  = (QApplication::translate("Service", "Connected", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_UPDATE_FIRMWARE_SLAVE             = (QApplication::translate("Service", "Update Firmware for Slave %1. please wait...", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_REQUESTED_OPERATION               = (QApplication::translate("Service", "Requested operation is in progress...", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_ROTATE_RV_TO_TUBE                 = (QApplication::translate("Service", "Rotate rotary valve to tube position #%1", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_ROTATE_RV_TO_SEALINT              = (QApplication::translate("Service", "Rotate rotary valve to sealing position #%1", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_CREATING_PRESSURE                 = (QApplication::translate("Service", "Creating pressure to %1Kpa...", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_KEEP_PRESSURE                     = (QApplication::translate("Service", "Keep pressure for %1 seconds", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_RELEASING_PRESSURE                = (QApplication::translate("Service", "Releasing pressure...", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_INIT_RV                           = (QApplication::translate("Service", "Rotary valve is initializing...", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_HEATING_LEVEL_SENSOR              = (QApplication::translate("Service", "Heating level sensor...", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_FILLING                           = (QApplication::translate("Service", "Filling...", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_DRAINING                          = (QApplication::translate("Service", "Draining...", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_AIR_BLOWING                       = (QApplication::translate("Service", "Air Blowing...", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_TEST_FINISH                       = (QApplication::translate("Service", "Test is finished.", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_ROTATE_RV_TO_TUBE_FAILED          = (QApplication::translate("Service", "Rotate rotary valve to tube position %1 failed", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_ROTATE_RV_TO_SEALING_FAILED       = (QApplication::translate("Service", "Rotate rotary valve to sealing position %1 failed", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_CREATE_PRESSURE_FALIED            = (QApplication::translate("Service", "create pressure failed.", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_FILLING_FAILED                    = (QApplication::translate("Service", "Filling is failed.", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_HEATING_LEVEL_SENSOR_FAILED       = (QApplication::translate("Service", "Level sensor heating is failed.", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_INIT_RV_FAILED                    = (QApplication::translate("Service", "Rotary valve initializing is failed.", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_OVEN_TEMP_NO_MATCH                = (QApplication::translate("Service", "Oven Current Temperature is (%1 %2 %3) which is not in (%4~%5)", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_RV_TEMP_NO_MATCH                  = (QApplication::translate("Service", "Rotary Valve Current Temperature is (%1 %2) which is not in (%3~%4)", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_LEVEL_SENSOR_TEMP_NO_MATCH        = (QApplication::translate("Service", "Level Sensor Current Temperature is (%1) which is not in (%2~%3)", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_RETORT_TEMP_NO_MATCH              = (QApplication::translate("Service", "Retort level sensor current temperature is (%1) which is less than target (%2) in %3 seconds", 0, QApplication::UnicodeUTF8));
	MSG_DIAGNOSTICS_TUBE_TEMP_NO_MATCH                = (QApplication::translate("Service", "Tube Current Temperature is (%1) which is not in (%2~%3)", 0, QApplication::UnicodeUTF8));
	MSG_DEVICECOMMAND_TIMEOUT                         = (QApplication::translate("Service", "ERROR: Device is not responding restart the system!", 0, QApplication::UnicodeUTF8));
	MSG_MANUFACTURINGTEST_RESULT                      = (QApplication::translate("Service", "Manufacturing Test Result", 0, QApplication::UnicodeUTF8));
	MSG_DEVICE_NOTYET_READY                           = (QApplication::translate("Service", "Error: Device Layer not yet Initialized. Please retry after some time...", 0, QApplication::UnicodeUTF8));
	MSG_DEVICE_RELEASE_PRESSURE_FAILED                = (QApplication::translate("Service", "ERROR: Release pressure failure.", 0, QApplication::UnicodeUTF8));
	MSG_DEVICE_PRESSURE_SENSOR_CALIBRATION_FAILED     = (QApplication::translate("Service", "ERROR: Pressure sensor calibration failure. Please confirm the air system is not blocked and open the retort lid do calibrate again!", 0, QApplication::UnicodeUTF8));
	MSG_DEVICE_PRESSURE_SENSOR_CALIBRATION_SUCCESS    = (QApplication::translate("Service", "Pressure sensor calibration is successful! Pressure drift value(Kpa):", 0, QApplication::UnicodeUTF8));
}

}// end of namespace Service

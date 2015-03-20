/****************************************************************************/
/*! \file Include/HimalayaServiceEventCodes.h
 *
 *  \brief Definition of event groups for event codes.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-5-9
 *   $Author:  $ Dixiong.li
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2010 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/
#ifndef HIMALAYASERVICEEVENTCODES_H
#define HIMALAYASERVICEEVENTCODES_H

#include <QtGlobal>

const quint32 EVENT_GROUP_HIMALAYASERVICE                                     = 0x06; //!< Event service group id
const quint32 EVENT_GROUP_HIMALAYASERVICE_MAIN                                = ((EVENT_GROUP_HIMALAYASERVICE << 8) + 0x01) << 16; //!< Event service group main id

const quint32 EVENT_LOGIN_MANUFACTURING                                       = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0001;  ///< 100728833 Manufacturing user logged in
const quint32 EVENT_LOGIN_SERVICE                                             = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0002;  ///< 100728834 Service user logged in
const quint32 EVENT_HIMALAYASERVICE_START                                     = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0003;  ///< 100728835 Service and Manufacturing SW Start
const quint32 EVENT_PASSWORD_INCORRECT                                        = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0004;  ///< 100728836 Password Incorrect
const quint32 EVENT_PASSWORD_NOT_MATCHING                                     = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0005;  ///< 100728837 Password Not matching
const quint32 EVENT_LOGIN_SERVICEUSER                                         = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0006;  ///< 100728838 Logged in Service User Name : %1
const quint32 EVENT_LOGIN_MANUFUSER                                           = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0007;  ///< 100728839 Logged in Manufacturing User Name : %1
const quint32 EVENT_SYSTEM_SHUT_DOWN                                          = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0008;  ///< 100728840 System shut down is requested User Name : %1
const quint32 EVENT_GUI_LOGVIEWER_LOGCONTENT_DISPLAY_INFO                     = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0009;  ///< 100728841 log content display is requested
const quint32 EVENT_GUI_LOGVIEWER_SYSTEMLOG_FILTERERING                       = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x000A;  ///< 100728842 System log info filtering is requested based on %1
const quint32 EVENT_GUI_LOGVIEWER_SYSTEMLOG_DETAILED_INFO                     = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x000B;  ///< 100728843 Detailed info of selected log is requested
const quint32 EVENT_GUI_LOGVIEWER_SYSTEMLOG_SERVICESERVICEHELPTEXT_FOR_ERROR  = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x000C;  ///< 100728844 Service help text is requested for selected error
const quint32 EVENT_GUI_LOGVIEWER_SERVICESERVICEHELPTEXT_DISPLAY_INFO         = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x000D;  ///< 100728845 Service help text action file info display is requested 
const quint32 EVENT_GUI_LOGVIEWER_SERVICELOG_DISPLAY_INFO                     = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x000E;  ///< 100728846 Service log information display is requested
const quint32 EVENT_GUI_LOGVIEWER_SYSTEMLOG_DISPLAY_INFO                      = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x000F;  ///< 100728847 System log information display is requested
const quint32 EVENT_GUI_LOGVIEWER_SOFTWAREUPDATELOG_DISPLAY_INFO              = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0010;  ///< 100728848 Software update log information display is requested
const quint32 EVENT_GUI_CALIBRATION_PRESSURE_SENSOR                           = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0011;  ///< 100728849 Pressure sensor calibration is requested
const quint32 EVENT_GUI_CALIBRATION_PRESSURE_SENSOR_PASS                      = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0012;  ///< 100728850 Pressure sensor calibration completed successfully
const quint32 EVENT_GUI_CALIBRATION_PRESSURE_SENSOR_FAIL                      = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0013;  ///< 100728851 Pressure sensor calibration failed
const quint32 EVENT_GUI_CALIBRATION_TOUCHSCREEN                               = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0014;  ///< 100728852 Touchscreen calibration is requested
const quint32 EVENT_GUI_SERVICEUPDATES_FIRMWARE_UPDATE                        = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0015;  ///< 100728853 Firmware update is requested
const quint32 EVENT_GUI_SERVICEUPDATES_FIRMWARE_UPDATE_SUCCESS                = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0016;  ///< 100728854 Firmware update of %1 completed successfully
const quint32 EVENT_GUI_SERVICEUPDATES_FIRMWARE_UPDATE_FAILURE                = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0017;  ///< 100728855 Firmware update of %1 failed
const quint32 EVENT_GUI_SERVICEUPDATES_FIRMWARE_UPDATE_REJECT                 = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0018;  ///< 100728856 The current firmware version has been latest
const quint32 EVENT_GUI_SERVICEUPDATES_FIRMWARE_DOWNLOAD                      = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0019;  ///< 100728857 Download firmware files is requested
const quint32 EVENT_GUI_SERVICEUPDATES_FIRMWARE_DOWNLOAD_SUCCESS              = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x001A;  ///< 100728858 Download firmware files completed successfully
const quint32 EVENT_GUI_SERVICEUPDATES_FIRMWARE_DOWNLOAD_FAILURE              = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x001B;  ///< 100728859 Download firmware files failed
const quint32 EVENT_GUI_SERVICEUPDATES_DATAMANAGEMENT_IMPORT                  = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x001C;  ///< 100728860 Import of data is requested
const quint32 EVENT_GUI_SERVICEUPDATES_DATAMANAGEMENT_EXPORT                  = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x001D;  ///< 100728861 Export of data is requested
const quint32 EVENT_GUI_CURRENTCONFIG_MODULEINFO_REQUESTED                    = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x001E;  ///< 100728862 Current configuration module info requested:%1
const quint32 EVENT_GUI_CURRENTCONFIG_SUBMODULEINFO_REQUESTED                 = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x001F;  ///< 100728863 Current configuration submodule info requested:%1
const quint32 EVENT_GUI_MODIFY_RETORT_MODULE                                  = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0020;  ///< 100728864 Modify retort info
const quint32 EVENT_GUI_MODIFY_RETORT_HEATER_MODULE                           = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0021;  ///< 100728865 Modify retort heater module info
const quint32 EVENT_GUI_MODIFY_RETORT_LIDLOCK_MODULE                          = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0022;  ///< 100728866 Modify retort lid lock module info
const quint32 EVENT_GUI_MODIFY_RETORT_LEVELSENSOR_MODULE                      = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0023;  ///< 100728867 Modify retort level sensor module info
const quint32 EVENT_GUI_MODIFY_MAIN_CONTROL_SUBMODULE                         = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0024;  ///< 100728868 Modify main control submodule info:%1
const quint32 EVENT_GUI_MODIFY_OVEN_MODULE                                    = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0025;  ///< 100728869 Modify oven info
const quint32 EVENT_GUI_MODIFY_OVEN_HEATER_MODULE                             = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0026;  ///< 100728870 Modify oven heater module info
const quint32 EVENT_GUI_MODIFY_OVEN_COVER_SENSOR_MODULE                       = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0027;  ///< 100728871 Modify oven cover sensor module info
const quint32 EVENT_GUI_MODIFY_ROTARYVALVE_MODULE                             = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0028;  ///< 100728872 Modify retary valve module info
const quint32 EVENT_GUI_MODIFY_ROTARYVALVE_HEATER_MODULE                      = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0029;  ///< 100728873 Modify retary valve heater module info
const quint32 EVENT_GUI_MODIFY_ROTARYVALVE_MOTOR_MODULE                       = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x002A;  ///< 100728874 Modify retary valve motor module info
const quint32 EVENT_GUI_MODIFY_LASYSTEM_PUMP_MODULE                           = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x002B;  ///< 100728875 Modify L&A system pump module info
const quint32 EVENT_GUI_MODIFY_LASYSTEM_VALVE1_MODULE                         = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x002C;  ///< 100728876 Modify L&A system valve 1 module info
const quint32 EVENT_GUI_MODIFY_LASYSTEM_VALVE2_MODULE                         = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x002D;  ///< 100728877 Modify L&A system valve 2 module info
const quint32 EVENT_GUI_MODIFY_LASYSTEM_HEATING_LIQUID_MODULE                 = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x002E;  ///< 100728878 Modify L&A system heating liquid module info
const quint32 EVENT_GUI_MODIFY_LASYSTEM_HEATING_AIR_MODULE                    = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x002F;  ///< 100728879 Modify L&A system heating air module info
const quint32 EVENT_GUI_MODIFY_LASYSTEM_PRESSURE_SENSOR_MODULE                = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0030;  ///< 100728880 Modify L&A system pressure sensor module info
const quint32 EVENT_GUI_MODIFY_LASYSTEM_CARBON_FILTER_MODULE                  = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0031;  ///< 100728881 Modify L&A system carbon filter module info
const quint32 EVENT_GUI_MODIFY_LASYSTEM_FAN_MODULE                            = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0032;  ///< 100728882 Modify L&A system fan module info
const quint32 EVENT_GUI_VIEWHISTORY_TIMESTAMP_SHOWDIFFERENCE                  = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0033;  ///< 100728883 Timestamp %1 and %2 is selected for module list show difference
const quint32 EVENT_GUI_VIEWHISTORY_TIMESTAMP_SHOWDETAILS                     = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0034;  ///< 100728884 View history Module TimeStamp %1 is selected for module details
const quint32 EVENT_GUI_DIAGNOSTICS_TEST_ABORT                                = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0035;  ///< 100728885 %1 test is abort
const quint32 EVENT_GUI_DIAGNOSTICS_DISPLAY_BASICCOLOR_TEST                   = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0036;  ///< 100728886 Basic color display test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_DISPLAY_BASICCOLOR_TEST_SUCCESS           = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0037;  ///< 100728887 Basic color display completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_DISPLAY_BASICCOLOR_TEST_FAILURE           = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0038;  ///< 100728888 Basic color display test failed
const quint32 EVENT_GUI_DIAGNOSTICS_RETORT_DRAINREAGENT_TEST                  = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0039;  ///< 100728889 Retort drain reagent test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_RETORT_DRAINREAGENT_TEST_SUCCESS          = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x003A;  ///< 100728890 Retort drain reagent test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_RETORT_DRAINREAGENT_TEST_FAILURE          = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x003B;  ///< 100728891 Retort drain reagent test failed
const quint32 EVENT_GUI_DIAGNOSTICS_RETORT_LIDLOCK_TEST                       = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x003C;  ///< 100728892 Retort lid lock test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_RETORT_LIDLOCK_TEST_SUCCESS               = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x003D;  ///< 100728893 Retort lid lock test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_RETORT_LIDLOCK_TEST_FAILURE               = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x003E;  ///< 100728894 Retort lid lock test failed
const quint32 EVENT_GUI_DIAGNOSTICS_RETORT_LEVELSENSOR_HEATING_TEST           = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x003F;  ///< 100728895 Retort level sensor heating test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_RETORT_LEVELSENSOR_HEATING_TEST_SUCCESS   = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0040;  ///< 100728896 Retort level sensor heating test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_RETORT_LEVELSENSOR_HEATING_TEST_FAILURE   = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0041;  ///< 100728897 Retort level sensor heating test failed
const quint32 EVENT_GUI_DIAGNOSTICS_RETORT_LEVELSENSOR_DETECT_TEST            = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0042;  ///< 100728898 Retort level sensor detecting test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_RETORT_LEVELSENSOR_DETECT_TEST_SUCCESS    = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0043;  ///< 100728899 Retort level sensor detecting test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_RETORT_LEVELSENSOR_DETECT_TEST_FAILURE    = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0044;  ///< 100728900 Retort level sensor detecting test failed
const quint32 EVENT_GUI_DIAGNOSTICS_RETORT_HEATING_EMPTY_TEST                 = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0045;  ///< 100728901 Retort heating test empty is requested
const quint32 EVENT_GUI_DIAGNOSTICS_RETORT_HEATING_EMPTY_TEST_SUCCESS         = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0046;  ///< 100728902 Retort heating test empty completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_RETORT_HEATING_EMPTY_TEST_FAILURE         = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0047;  ///< 100728903 Retort heating test empty failed
const quint32 EVENT_GUI_DIAGNOSTICS_RETORT_HEATING_LIQUID_TEST                = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0048;  ///< 100728904 Retort heating test with (with water) is requested
const quint32 EVENT_GUI_DIAGNOSTICS_RETORT_HEATING_LIQUID_TEST_SUCCESS        = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0049;  ///< 100728905 Retort heating test with (with water) completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_RETORT_HEATING_LIQUID_TEST_FAILURE        = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x004A;  ///< 100728906 Retort heating test with (with water) failed
const quint32 EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_EMPTY_TEST                   = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x004B;  ///< 100728907 Oven heating test empty is requested
const quint32 EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_EMPTY_TEST_SUCCESS           = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x004C;  ///< 100728908 Oven heating test empty completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_EMPTY_TEST_FAILURE           = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x004D;  ///< 100728909 Oven heating test empty failed
const quint32 EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_LIQUID_TEST                  = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x004E;  ///< 100728910 Oven heating test with water is requested
const quint32 EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_LIQUID_TEST_SUCCESS          = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x004F;  ///< 100728911 Oven heating test with water completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_OVEN_HEATING_LIQUID_TEST_FAILURE          = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0050;  ///< 100728912 Oven heating test with water failed
const quint32 EVENT_GUI_DIAGNOSTICS_OVEN_COVER_SENSOR_TEST                    = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0051;  ///< 100728913 Oven cover sensor test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_OVEN_COVER_SENSOR_TEST_SUCCESS            = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0052;  ///< 100728914 Oven cover sensor test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_OVEN_COVER_SENSOR_TEST_FAILURE            = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0053;  ///< 100728915 Oven cover sensor test failed
const quint32 EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_INITIALIZING_TEST             = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0054;  ///< 100728916 Rotary valve initializing test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_INITIALIZING_TEST_SUCCESS     = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0055;  ///< 100728917 Rotary valve initializing test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_INITIALIZING_TEST_FAILURE     = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0056;  ///< 100728918 Rotary valve initializing test failed
const quint32 EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SELECTING_TEST                = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0057;  ///< 100728919 Rotary valve selecting test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SELECTING_TEST_SUCCESS        = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0058;  ///< 100728920 Rotary valve selecting test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SELECTING_TEST_FAILURE        = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0059;  ///< 100728921 Rotary valve selecting test failed
const quint32 EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SEALING_TEST                  = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x005A;  ///< 100728922 Rotary valve sealing test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SEALING_TEST_SUCCESS          = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x005B;  ///< 100728923 Rotary valve sealing test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_SEALING_TEST_FAILURE          = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x005C;  ///< 100728924 Rotary valve sealing test failed
const quint32 EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_HEATING_END_TEST              = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x005D;  ///< 100728925 Rotary valve heating property end test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_HEATING_END_TEST_SUCCESS      = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x005E;  ///< 100728926 Rotary valve heating property end test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_HEATING_END_TEST_FAILURE      = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x005F;  ///< 100728927 Rotary valve heating property end test failed
const quint32 EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_HEATING_STATION_TEST          = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0060;  ///< 100728928 Rotary valve heating property station test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_HEATING_STATION_TEST_SUCCESS  = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0061;  ///< 100728929 Rotary valve heating property station test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_HEATING_STATION_TEST_FAILURE  = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0062;  ///< 100728930 Rotary valve heating property station test failed
const quint32 EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_MOVEMENT_TEST                 = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0063;  ///< 100728931 Rotary valve movement test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_MOVEMENT_TEST_SUCCESS         = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0064;  ///< 100728932 Rotary valve movement test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_ROTARYVALVE_MOVEMENT_TEST_FAILURE         = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0065;  ///< 100728933 Rotary valve movement test failed
const quint32 EVENT_GUI_DIAGNOSTICS_LASYSTEM_HEATING_LIQUID_TEST              = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0066;  ///< 100728934 L&A system heating liquid test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_LASYSTEM_HEATING_LIQUID_TEST_SUCCESS      = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0067;  ///< 100728935 L&A system heating liquid test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_LASYSTEM_HEATING_LIQUID_TEST_FAILURE      = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0068;  ///< 100728936 L&A system heating liquid test failed
const quint32 EVENT_GUI_DIAGNOSTICS_LASYSTEM_HEATING_AIR_TEST                 = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0069;  ///< 100728937 L&A system heating air test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_LASYSTEM_HEATING_AIR_TEST_SUCCESS         = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x006A;  ///< 100728938 L&A system heating air test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_LASYSTEM_HEATING_AIR_TEST_FAILURE         = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x006B;  ///< 100728939 L&A system heating air test failed
const quint32 EVENT_GUI_DIAGNOSTICS_LASYSTEM_AIR_SYSTEM_TEST                  = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x006C;  ///< 100728940 L&A system air system test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_LASYSTEM_AIR_SYSTEM_TEST_SUCCESS          = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x006D;  ///< 100728941 L&A system air system test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_LASYSTEM_AIR_SYSTEM_TEST_FAILURE          = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x006E;  ///< 100728942 L&A system air system test failed
const quint32 EVENT_GUI_DIAGNOSTICS_MAINCONTROL_ASB3_TEST                     = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x006F;  ///< 100728943 Main control ASB3 test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_MAINCONTROL_ASB3_TEST_SUCCESS             = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0070;  ///< 100728944 Main control ASB3 completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_MAINCONTROL_ASB3_TEST_FAILURE             = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0071;  ///< 100728945 Main control ASB3 test failed
const quint32 EVENT_GUI_DIAGNOSTICS_MAINCONTROL_ASB5_TEST                     = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0072;  ///< 100728946 Main control ASB5 test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_MAINCONTROL_ASB5_TEST_SUCCESS             = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0073;  ///< 100728947 Main control ASB5 completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_MAINCONTROL_ASB5_TEST_FAILURE             = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0074;  ///< 100728948 Main control ASB5 test failed
const quint32 EVENT_GUI_DIAGNOSTICS_MAINCONTROL_ASB15_TEST                    = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0075;  ///< 100728949 Main control ASB15 test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_MAINCONTROL_ASB15_TEST_SUCCESS            = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0076;  ///< 100728950 Main control ASB15 completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_MAINCONTROL_ASB15_TEST_FAILURE            = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0077;  ///< 100728951 Main control ASB15 test failed
const quint32 EVENT_GUI_DIAGNOSTICS_MAINCONTROL_EBOX_TEST                     = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0078;  ///< 100728952 Main control EBox test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_MAINCONTROL_EBOX_TEST_SUCCESS             = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0079;  ///< 100728953 Main control EBox completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_MAINCONTROL_EBOX_TEST_FAILURE             = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x007A;  ///< 100728954 Main control EBox test failed
const quint32 EVENT_GUI_DIAGNOSTICS_MAINCONTROL_TOUCH_SCREEN_TEST             = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x007B;  ///< 100728955 Main control touch screen test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_MAINCONTROL_TOUCH_SCREEN_TEST_SUCCESS     = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x007C;  ///< 100728956 Main control touch screen completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_MAINCONTROL_TOUCH_SCREEN_TEST_FAILURE     = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x007D;  ///< 100728957 Main control touch screen test failed
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_LIQUIDHOSE_TEST                    = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x007E;  ///< 100728958 System liquid hose test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_LIQUIDHOSE_TEST_SUCCESS            = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x007F;  ///< 100728959 System liquid hose test finished
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_LIQUIDHOSE_TEST_FAILURE            = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0080;  ///< 100728960 System liquid hose test failed
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_LIQUIDHOSE_TEST_POSITION           = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0081;  ///< 100728961 System liquid hose test position is tube %1 pressure is %2Kpa result is %3
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_SEALING_TEST                       = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0082;  ///< 100728962 System sealing function test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_SEALING_TEST_SUCCESS               = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0083;  ///< 100728963 System sealing function test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_SEALING_TEST_FAILURE               = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0084;  ///< 100728964 System sealing function test failed
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_EXHAUST_FUN_TEST                   = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0085;  ///< 100728965 System exhaust fun test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_EXHAUST_FUN_TEST_SUCCESS           = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0086;  ///< 100728966 System exhaust fun test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_EXHAUST_FUN_TEST_FAILURE           = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0087;  ///< 100728967 System exhaust fun test failed
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_VENTILATION_FUN_TEST               = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0088;  ///< 100728968 System ventilation fun test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_VENTILATION_FUN_TEST_SUCCESS       = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0089;  ///< 100728969 System ventilation fun test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_VENTILATION_FUN_TEST_FAILURE       = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x008A;  ///< 100728970 System ventilation fun test failed
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_FILLING_DRAINING_TEST              = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x008B;  ///< 100728971 System filling and draining test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_FILLING_DRAINING_TEST_SUCCESS      = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x008C;  ///< 100728972 System filling and draining test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_FILLING_DRAINING_TEST_FAILURE      = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x008D;  ///< 100728973 System filling and draining test failed
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_OVERFLOW_TEST                      = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x008E;  ///< 100728974 System overflow test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_OVERFLOW_TEST_SUCCESS              = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x008F;  ///< 100728975 System overflow test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_OVERFLOW_TEST_FAILURE              = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0090;  ///< 100728976 System overflow test failed
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_SPEAKER_TEST                       = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0091;  ///< 100728977 System speaker test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_SPEAKER_TEST_SUCCESS               = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0092;  ///< 100728978 System speaker test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_SPEAKER_TEST_FAILURE               = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0093;  ///< 100728979 System speaker test failed
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_ALARM_TEST                         = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0094;  ///< 100728980 System remote local alarm test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_ALARM_TEST_SUCCESS                 = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0095;  ///< 100728981 System remote local alarm test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_ALARM_TEST_FAILURE                 = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0096;  ///< 100728982 System remote local alarm test is failed
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_USB_TEST                           = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0097;  ///< 100728983 System usb test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_USB_TEST_SUCCESS                   = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0098;  ///< 100728984 System usb test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_USB_TEST_FAILURE                   = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x0099;  ///< 100728985 System usb test failed
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_MAINS_RELAY_TEST                   = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x009A;  ///< 100728986 System mains relay test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_MAINS_RELAY_TEST_SUCCESS           = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x009B;  ///< 100728987 System mains relay test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_MAINS_RELAY_TEST_FAILURE           = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x009C;  ///< 100728988 System mains relay test is failed
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_110V220V_TEST                      = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x009D;  ///< 100728989 System 110V/220V switching test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_110V220V_TEST_SUCCESS              = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x009E;  ///< 100728990 System 110V/220V switching test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_SYSTEM_110V220V_TEST_FAILURE              = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x009F;  ///< 100728991 System 110V/220V switching test failed
const quint32 EVENT_GUI_DIAGNOSTICS_CLEANING_SYSTEM_TEST                      = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00A0;  ///< 100728992 Cleaning system test is requested
const quint32 EVENT_GUI_DIAGNOSTICS_CLEANING_SYSTEM_TEST_SUCCESS              = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00A1;  ///< 100728993 Cleaning system test completed successfully
const quint32 EVENT_GUI_DIAGNOSTICS_CLEANING_SYSTEM_TEST_FAILURE              = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00A2;  ///< 100728994 Cleaning system test failed
const quint32 EVENT_GUI_SVCDIAGNOSTICS_ACCEPT_RISK                            = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00A3;  ///< 100728995 Enter the SVC diagnostics screen is accept risk
const quint32 EVENT_GUI_SVCDIAGNOSTICS_ABORT                                  = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00A4;  ///< 100728996 Enter the SVC diagnostics screen is abort
const quint32 EVENT_GUI_SVCDIAGNOSTICS_PART_HEATING                           = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00A5;  ///< 100728997 Heating %1 activated and set temperature was %2
const quint32 EVENT_GUI_SVCDIAGNOSTICS_PART_ACTIVATE                          = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00A6;  ///< 100728998 %1 is activated
const quint32 EVENT_GUI_SVCDIAGNOSTICS_PART_DEACTIVATE                        = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00A7;  ///< 100728999 %1 is deactivated
const quint32 EVENT_GUI_SVCDIAGNOSTICS_SELECTED_POSITION                      = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00A8;  ///< 100729000 Selected position %1 for Rotary valve
const quint32 EVENT_GUI_SVCDIAGNOSTICS_MOVE_POSITION_SUCCESS                  = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00A9;  ///< 100729001 Selected position is reached
const quint32 EVENT_GUI_SVCDIAGNOSTICS_MOVE_POSITION_FAILED                   = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00AA;  ///< 100729002 Selected position is not reached
const quint32 EVENT_GUI_MANUF_RETORT_TEST_REQUESTED                           = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00AB;  ///< 100729003 Manufacturing: Retort module test is requested
const quint32 EVENT_GUI_MANUF_RETORT_SENDTESTREPORT_REQUESTED                 = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00AC;  ///< 100729004 Manufacturing: Retort send test report
const quint32 EVENT_GUI_MANUF_OVEN_TEST_REQUESTED                             = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00AD;  ///< 100729005 Manufacturing: Oven module test is requested
const quint32 EVENT_GUI_MANUF_OVEN_SENDTESTREPORT_REQUESTED                   = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00AE;  ///< 100729006 Manufacturing: Oven send test report
const quint32 EVENT_GUI_MANUF_ROTARYVALVE_TEST_REQUESTED                      = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00AF;  ///< 100729007 Manufacturing: Rotary valve module test is requested
const quint32 EVENT_GUI_MANUF_ROTARYVALVE_SENDTESTREPORT_REQUESTED            = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00B0;  ///< 100729008 Manufacturing: Rotary valve send test report
const quint32 EVENT_GUI_MANUF_LASYSTEM_TEST_REQUESTED                         = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00B1;  ///< 100729009 Manufacturing: L&A system module test is requested
const quint32 EVENT_GUI_MANUF_LASYSTEM_SENDTESTREPORT_REQUESTED               = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00B2;  ///< 100729010 Manufacturing: L&A system send test report
const quint32 EVENT_GUI_MANUF_MAINCONTROL_TEST_REQUESTED                      = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00B3;  ///< 100729011 Manufacturing: Main control module test is requested
const quint32 EVENT_GUI_MANUF_MAINCONTROL_SENDTESTREPORT_REQUESTED            = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00B4;  ///< 100729012 Manufacturing: Main control send test report
const quint32 EVENT_GUI_MANUF_SYSTEM_TEST_REQUESTED                           = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00B5;  ///< 100729013 Manufacturing: System module test is requested
const quint32 EVENT_GUI_MANUF_SYSTEM_SENDTESTREPORT_REQUESTED                 = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00B6;  ///< 100729014 Manufacturing: System send test report
const quint32 EVENT_GUI_MANUF_CLEANING_SYSTEM_TEST_REQUESTED                  = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00B7;  ///< 100729015 Manufacturing: Cleaning system test is requested
const quint32 EVENT_GUI_MANUF_CLEANING_SYSTEM_SENDTESTREPORT_REQUESTED        = EVENT_GROUP_HIMALAYASERVICE_MAIN + 0x00B8;  ///< 100729016 Manufacturing: Cleaning system send test report


#endif


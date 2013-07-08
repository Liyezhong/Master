#!/bin/sh

if ! dpkg -l xmlstarlet >/dev/null; then
  echo "Please install xmlstarlet"
  exit 1
fi

subst () {
  FILE="/var/tmp/tmp-$(id -u)-$$"
  ( echo "cat <<EOF"; cat <$1; echo "EOF" ) >$FILE
  . $FILE; rm $FILE
}

# set IFS to defaults, i.e. whitespace, tab, newline
unset IFS
# read examples: http://mywiki.wooledge.org/BashFAQ/001
while read -r lin; do
  if echo "$lin" | grep -q "^ *\(#\|$\)"; then continue; fi
  ( eval $lin;  subst blocks/template_${TEMPLATE}.xml )
done <<EOF | xmlstarlet ed -P -d "//*[@rm_ls1=1]" -d "//@rm_ls1" -d "//*[@rm_ls2=1]" -d "//@rm_ls2"

TEMPLATE="header"

  TEMPLATE="slave_header" ASB_TYPE=1 ASB_INDEX=0
    TEMPLATE="FM_digital_out"          FM_KEY="lsensor_teach1"     FM_CHANNEL=1
    TEMPLATE="FM_analog_in"            FM_KEY="lsensor1"           FM_CHANNEL=2
    TEMPLATE="FM_analog_in"            FM_KEY="phasea_z1"          FM_CHANNEL=3
    TEMPLATE="FM_analog_in"            FM_KEY="phaseb_z1"          FM_CHANNEL=4
    TEMPLATE="FM_rfid11785"            FM_KEY="rfid1"              FM_CHANNEL=5
    TEMPLATE="FM_steppermotor"         FM_KEY="motor_z1"           FM_CHANNEL=6  RM_LS2=1 CURRENT=6  
  TEMPLATE="slave_footer"

  TEMPLATE="slave_header" ASB_TYPE=1 ASB_INDEX=1
    TEMPLATE="FM_digital_out"          FM_KEY="lsensor_teach2"     FM_CHANNEL=1
    TEMPLATE="FM_analog_in"            FM_KEY="lsensor2"           FM_CHANNEL=2
    TEMPLATE="FM_analog_in"            FM_KEY="phasea_z2"          FM_CHANNEL=3
    TEMPLATE="FM_analog_in"            FM_KEY="phaseb_z2"          FM_CHANNEL=4
    TEMPLATE="FM_rfid11785"            FM_KEY="rfid2"              FM_CHANNEL=5
    TEMPLATE="FM_steppermotor"         FM_KEY="motor_z2"           FM_CHANNEL=6  RM_LS2=1 CURRENT=6
  TEMPLATE="slave_footer"

  TEMPLATE="slave_header" ASB_TYPE=2 ASB_INDEX=0
    TEMPLATE="FM_analog_in"            FM_KEY="phasea_y1"          FM_CHANNEL=1
    TEMPLATE="FM_analog_in"            FM_KEY="phaseb_y1"          FM_CHANNEL=2
    TEMPLATE="FM_steppermotor"         FM_KEY="motor_y1"           FM_CHANNEL=3  RM_LS2=1 DIR=cw DIR_LS1=ccw
  TEMPLATE="slave_footer"

  TEMPLATE="slave_header" ASB_TYPE=2 ASB_INDEX=1
    TEMPLATE="FM_analog_in"            FM_KEY="phasea_y2"          FM_CHANNEL=1
    TEMPLATE="FM_analog_in"            FM_KEY="phaseb_y2"          FM_CHANNEL=2
    TEMPLATE="FM_steppermotor"         FM_KEY="motor_y2"           FM_CHANNEL=3  RM_LS2=1 DIR=cw DIR_LS1=ccw
  TEMPLATE="slave_footer"

  TEMPLATE="slave_header" ASB_TYPE=2 ASB_INDEX=2
    TEMPLATE="FM_analog_in"            FM_KEY="phasea_transfer"   FM_CHANNEL=1
    TEMPLATE="FM_analog_in"            FM_KEY="phaseb_transfer"   FM_CHANNEL=2
    TEMPLATE="FM_steppermotor"         FM_KEY="motor_transfer"    FM_CHANNEL=3  DIR=cw DIR_LS1=ccw DIR_LS2=cw CURRENT=8
  TEMPLATE="slave_footer"

  TEMPLATE="slave_header" ASB_TYPE=3 ASB_INDEX=0
    TEMPLATE="FM_analog_in"            FM_KEY="phasea_oven"        FM_CHANNEL=1
    TEMPLATE="FM_analog_in"            FM_KEY="phaseb_oven"        FM_CHANNEL=2
    TEMPLATE="FM_steppermotor"         FM_KEY="motor_oven"         FM_CHANNEL=3 CURRENT=8
    TEMPLATE="FM_temperature_control"  FM_KEY="tempctrl"           FM_CHANNEL=4
  TEMPLATE="slave_footer"

  TEMPLATE="slave_header" ASB_TYPE=4 ASB_INDEX=0
    TEMPLATE="FM_digital_in"           FM_KEY="btn_loader"         FM_CHANNEL=1
    TEMPLATE="FM_digital_out"          FM_KEY="led_red_loader"     FM_CHANNEL=2
    TEMPLATE="FM_digital_out"          FM_KEY="led_green_loader"   FM_CHANNEL=3
    TEMPLATE="FM_analog_in"            FM_KEY="phasea_loader"      FM_CHANNEL=4
    TEMPLATE="FM_analog_in"            FM_KEY="phaseb_loader"      FM_CHANNEL=5
    TEMPLATE="FM_rfid11785"            FM_KEY="rfid_loader"        FM_CHANNEL=6
    TEMPLATE="FM_steppermotor"         FM_KEY="motor_loader"       FM_CHANNEL=7
  TEMPLATE="slave_footer"

  TEMPLATE="slave_header" ASB_TYPE=4 ASB_INDEX=1
    TEMPLATE="FM_digital_in"           FM_KEY="btn_unloader"       FM_CHANNEL=1
    TEMPLATE="FM_digital_out"          FM_KEY="led_red_unloader"   FM_CHANNEL=2
    TEMPLATE="FM_digital_out"          FM_KEY="led_green_unloader" FM_CHANNEL=3
    TEMPLATE="FM_analog_in"            FM_KEY="phasea_unloader"    FM_CHANNEL=4
    TEMPLATE="FM_analog_in"            FM_KEY="phaseb_unloader"    FM_CHANNEL=5
    TEMPLATE="FM_rfid11785"            FM_KEY="rfid_unloader"      FM_CHANNEL=6
    TEMPLATE="FM_steppermotor"         FM_KEY="motor_unloader"     FM_CHANNEL=7
  TEMPLATE="slave_footer"

  TEMPLATE="slave_header" ASB_TYPE=5 ASB_INDEX=0
    TEMPLATE="FM_tempctrl_vessels"     FM_KEY="tempctrl1"          FM_CHANNEL=1
    TEMPLATE="FM_tempctrl_vessels"     FM_KEY="tempctrl2"          FM_CHANNEL=2
    TEMPLATE="FM_tempctrl_vessels"     FM_KEY="tempctrl3"          FM_CHANNEL=3
    TEMPLATE="FM_tempctrl_vessels"     FM_KEY="tempctrl4"          FM_CHANNEL=4
  TEMPLATE="slave_footer"

  TEMPLATE="slave_header" ASB_TYPE=6 ASB_INDEX=0
    TEMPLATE="FM_digital_in"           FM_KEY="airflow_switch"     FM_CHANNEL=1
    TEMPLATE="FM_digital_in"           FM_KEY="level_switch"       FM_CHANNEL=2
    TEMPLATE="FM_digital_in"           FM_KEY="reed_active1"       FM_CHANNEL=3
    TEMPLATE="FM_digital_out"          FM_KEY="airfan_control1"    FM_CHANNEL=4
    TEMPLATE="FM_digital_out"          FM_KEY="airfan_control2"    FM_CHANNEL=5
    TEMPLATE="FM_digital_out"          FM_KEY="light_control2"     FM_CHANNEL=6
    TEMPLATE="FM_digital_out"          FM_KEY="valve_control1"     FM_CHANNEL=7
    TEMPLATE="FM_digital_out"          FM_KEY="valve_control2"     FM_CHANNEL=8
    TEMPLATE="FM_digital_out"          FM_KEY="valve_control3"     FM_CHANNEL=9
    TEMPLATE="FM_digital_out"          FM_KEY="valve_control4"     FM_CHANNEL=10
    TEMPLATE="FM_digital_out"          FM_KEY="valve_control5"     FM_CHANNEL=11
    TEMPLATE="FM_digital_out"          FM_KEY="valve_control6"     FM_CHANNEL=12
    TEMPLATE="FM_analog_in"            FM_KEY="airfan_current1"    FM_CHANNEL=13
    TEMPLATE="FM_analog_in"            FM_KEY="airfan_current2"    FM_CHANNEL=14
    TEMPLATE="FM_analog_in"            FM_KEY="phasea_x2"          FM_CHANNEL=15
    TEMPLATE="FM_analog_in"            FM_KEY="phaseb_x2"          FM_CHANNEL=16
    TEMPLATE="FM_steppermotor"         FM_KEY="motor_x2"           FM_CHANNEL=17 RM_LS2=1
  TEMPLATE="slave_footer"

  TEMPLATE="slave_header" ASB_TYPE=7 ASB_INDEX=0
    TEMPLATE="FM_digital_in"           FM_KEY="reed_active2"       FM_CHANNEL=1
    TEMPLATE="FM_analog_in"            FM_KEY="phasea_x1"          FM_CHANNEL=2
    TEMPLATE="FM_analog_in"            FM_KEY="phaseb_x1"          FM_CHANNEL=3
    TEMPLATE="FM_analog_in"            FM_KEY="phasea_agitation"   FM_CHANNEL=4
    TEMPLATE="FM_analog_in"            FM_KEY="phaseb_agitation"   FM_CHANNEL=5
    TEMPLATE="FM_steppermotor"         FM_KEY="motor_x1"           FM_CHANNEL=6 RM_LS2=1 DIR=cw DIR_LS1=ccw
    TEMPLATE="FM_steppermotor"         FM_KEY="motor_agitation"    FM_CHANNEL=7 DIR=cw DIR_LS1=ccw DIR_LS2=cw CURRENT=8
  TEMPLATE="slave_footer"

  TEMPLATE="slave_header" ASB_TYPE=12 ASB_INDEX=0
    TEMPLATE="FM_analog_in"            FM_KEY="photo_detector"     FM_CHANNEL=1  FAST_SAMPLING=1 INTERVAL=1  VALUE1_SEND=1 VALUE1=200
    TEMPLATE="FM_analog_out"           FM_KEY="transmit_control"   FM_CHANNEL=2  LIMIT=65535 OUTVAL=65535
    TEMPLATE="FM_analog_out"           FM_KEY="transmit_current"   FM_CHANNEL=3
    TEMPLATE="FM_analog_out"           FM_KEY="receive_current"    FM_CHANNEL=4  LIMIT=0     OUTVAL=0
    TEMPLATE="FM_digital_out"          FM_KEY="camera_signal"      FM_CHANNEL=5
  TEMPLATE="slave_footer"

  TEMPLATE="slave_header" ASB_TYPE=13 ASB_INDEX=0
    TEMPLATE="FM_analog_in"            FM_KEY="phasea_unused"      FM_CHANNEL=1
    TEMPLATE="FM_analog_in"            FM_KEY="phaseb_unused"      FM_CHANNEL=2
    TEMPLATE="FM_rfid15693"            FM_KEY="rfid_consumables"   FM_CHANNEL=3
    TEMPLATE="FM_steppermotor"         FM_KEY="motor_unused"       FM_CHANNEL=4
  TEMPLATE="slave_footer"

TEMPLATE="footer"
EOF

exit 0


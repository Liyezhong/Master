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

  TEMPLATE="slave_header" ASB_TYPE=2 ASB_INDEX=1
    TEMPLATE="FM_analog_in"            FM_KEY="phasea_x"          FM_CHANNEL=1
    TEMPLATE="FM_analog_in"            FM_KEY="phaseb_x"          FM_CHANNEL=2
    TEMPLATE="FM_steppermotor"         FM_KEY="motor_x"           FM_CHANNEL=3  RM_LS2=1
  TEMPLATE="slave_footer"

  TEMPLATE="slave_header" ASB_TYPE=2 ASB_INDEX=0
    TEMPLATE="FM_analog_in"            FM_KEY="phasea_z"          FM_CHANNEL=1
    TEMPLATE="FM_analog_in"            FM_KEY="phaseb_z"          FM_CHANNEL=2
    TEMPLATE="FM_steppermotor"         FM_KEY="motor_z"           FM_CHANNEL=3  RM_LS2=1 DIR=cw DIR_LS1=ccw CURRENT=12 CURRENT_STOP=8
  TEMPLATE="slave_footer"

  TEMPLATE="slave_header" ASB_TYPE=10 ASB_INDEX=0
    TEMPLATE="FM_analog_in"            FM_KEY="distance_sensor"   FM_CHANNEL=1
    TEMPLATE="FM_analog_in"            FM_KEY="phasea_y"          FM_CHANNEL=2
    TEMPLATE="FM_analog_in"            FM_KEY="phaseb_y"          FM_CHANNEL=3
    TEMPLATE="FM_analog_in"            FM_KEY="phasea_grabbler"   FM_CHANNEL=4
    TEMPLATE="FM_analog_in"            FM_KEY="phaseb_grabbler"   FM_CHANNEL=5
    TEMPLATE="FM_steppermotor"         FM_KEY="motor_y"           FM_CHANNEL=6 RM_LS2=1
    TEMPLATE="FM_steppermotor"         FM_KEY="motor_grabbler"    FM_CHANNEL=7 RM_LS1=1 RM_LS2=1 CURRENT=12
  TEMPLATE="slave_footer"

TEMPLATE="footer"
EOF

exit 0


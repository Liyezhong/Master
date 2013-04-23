#!/bin/bash


while read line
do
	# anlyze API interface
	#
	#
	fields=(`echo $line | sed -e 's/(/ /g' -e 's/)/ /g' \
			 -e 's/,//g' -e 's/;//g'`)	
	type=${fields[0]}


       	if [[ "$type" =~ ^[^q|Q|void|bool] ]]
        then
            type="DeviceControl::"$type
        fi
	var_ret="mutable "$type" m_result;"
	fct_ret=$type" GetResult()const{return m_result;}\n\r\t"
	fct_ret=${fct_ret}'bool GetResult('$type'\& result) const{result = m_result; return true;}'

	name=${fields[1]}
	len=$(expr ${#fields[@]} - 1)
	var_par=""

	parlist=""
	var_par=""
	fct_par=""
	for((i=2; i<$len; i++))
	do
		j=$(expr $i + 1)
		type=${fields[$i]};
		if [[ "$type" =~ ^[^q|Q|void|bool] ]]
		then
			type="DeviceControl::"$type
		fi
		var_par=${var_par}"mutable "$type" m_"${fields[$j]}";\n\t"
		fct_par=${fct_par}$type" Get"${fields[$j]}"(){return m_"${fields[$j]}";}\n\t"
		fct_par=${fct_par}"void Set"${fields[$j]}"("$type" "${fields[$j]}"){m_"${fields[$j]}" = "${fields[$j]}";}\n\t"
		parlist=${parlist}"m_"${fields[$j]}", "
		i=$j 
	done
#	echo -e $var_par
#	echo -e $fct_par

	parlist=`echo $parlist | sed -e "s/,$//g"`
#	echo $parlist
	stat_exe="m_result = m_IDeviceProcessing->"$name"("$parlist");"

#	echo -e $stat_exe
	
#	compile condation for header file
	CM_NAME=`echo $name | tr [:lower:] [:upper:]`



	# create file
	#
	#
	sed -e "s/Template/$name/g" -e "s/fct_ret/$fct_ret/g" -e "s/fct_par/$fct_par/g" -e "s/TEMPLATE/$CM_NAME/g" \
	    -e "s/var_ret/$var_ret/g" -e "s/var_par/$var_par/g" CmdTemplate.h > "./Include/Cmd"$name".h"
	sed -e "s/Template/$name/g" -e "s/ph_execute/$stat_exe/g" CmdTemplate.cpp > "./Source/Cmd"$name".cpp"
	
done < scheduler.txt

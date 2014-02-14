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
	fct_ret=$type" GetResult()const {return m_result;}\n"
	fct_ret=${fct_ret}"\tvoid SetResult($type result) { m_result = result;}\n"
	fct_ret=${fct_ret}"\t"'bool GetResult('$type'\& result) const{result = m_result; return true;}'

	name=${fields[1]}
	len=$(expr ${#fields[@]} - 1)
	var_par=""

	parlist=""
	var_par=""
	fct_par=""
	par_str_fmt=""
	par_str_vals=""
	for((i=2; i<$len; i++))
	do
		j=$(expr $i + 1)
		type=${fields[$i]};
		if [[ "$type" =~ ^[^q|Q|void|bool] ]]
		then
			type="DeviceControl::"$type
		fi
		var_par=${var_par}"mutable "$type" m_"${fields[$j]}";\n\t"
		#par_str_fmt=${par_str_fmt}${fields[$j]}":%"$(expr $i / 2)" "
		par_str_fmt=${par_str_fmt}"%"$(expr $i / 2)","

		par_str_vals=${par_str_vals}".arg(m_"${fields[$j]}")"
		fct_par=${fct_par}$type" Get"${fields[$j]}"(){return m_"${fields[$j]}";}\n\t"
		fct_par=${fct_par}"void Set"${fields[$j]}"("$type" "${fields[$j]}"){m_"${fields[$j]}" = "${fields[$j]}";}\n\t"
		parlist=${parlist}"m_"${fields[$j]}", "
		i=$j 
	done
	par_str_fmt=`echo $par_str_fmt | sed -e "s/,$//g"`
	par_str="QString GetParameters()const{ return GetName() + QString(\"("${par_str_fmt}")\")"${par_str_vals}";}\n\tQString GetStrResult()const{ return QString(\"%1\").arg(m_result);}"
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
	    -e "s/var_ret/$var_ret/g" -e "s/var_par/$var_par/g" -e "s/fct_str/$par_str/g" CmdTemplate.h > "./Include/Cmd"$name".h"
	#sed -e "s/Template/$name/g" -e "s/ph_execute/$stat_exe/g" CmdTemplate.cpp > "./Source/Cmd"$name".cpp"
	sed -e "s/Template/$name/g" CmdTemplate.cpp > "./Source/Cmd"$name".cpp"
	
done < scheduler.txt

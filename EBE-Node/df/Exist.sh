#!/bin/bash


for ((m="$2"; m<="$3"; m++)) 
do 
	for ((i=0; i<=999; i++)) 
	do 
		FILE="$1"/"$m"/ev"$i"dsbvc_dNdphidpp.dat     
		if [ ! -f "$FILE" ]; then
		   echo "$m"/"$i"
		fi
#		read -p "$FILE"
	done
done

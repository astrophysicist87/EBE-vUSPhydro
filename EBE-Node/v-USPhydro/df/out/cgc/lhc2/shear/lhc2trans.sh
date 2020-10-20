#!/bin/bash

for (( cen=10; cen<=60; cen=cen+10))
do
	for (( j=1; j<=1000; j++ ))
	do
		let bob="$cen"-10
		let new="$j"+"$bob"*100
	   	cp "$cen"eta12/ev"$j"dsbvc_dNdphidpp.dat eta12/ev"$new"dsbvc_dNdphidpp.dat
	   	cp "$cen"eta12/ev"$j"sbvc_dNdphidpp.dat eta12/ev"$new"sbvc_dNdphidpp.dat
	done 
done

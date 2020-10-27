#!/bin/bash

COUNTER=0
b=0
mkdir inputfiles/trento/"$1"/0
for i in {1..50000} 
do 
#   if [ $COUNTER == 1000 ] ; then
#  	let b=b+1
#	COUNTER=0
#	mkdir inputfiles/trento/"$1"/"$b"
#	mkdir outputfiles/trento/"$1"/shear/EOS21"$2"/"$b"
#	mkdir df/input/trento/"$1"/shear/EOS21"$2"/"$b"
#	mkdir df/out/trento/"$1"/shear/EOS21"$2"/"$b"
#	mkdir outputfiles/trento/"$1"/shear/EOS211"$3"/"$b"
#	mkdir df/input/trento/"$1"/shear/EOS211"$3"/"$b"
#	mkdir df/out/trento/"$1"/shear/EOS211"$3"/"$b"
#	mkdir outputfiles/trento/"$1"/shear/old"$4"/"$b"
#	mkdir df/input/trento/"$1"/shear/old"$4"/"$b"
#	mkdir df/out/trento/"$1"/shear/old"$4"/"$b"
#   fi	
#   mv  inputfiles/trento/"$1"/ic"$i".dat inputfiles/trento/"$1"/"$b"/ic"$COUNTER".dat
   
   let COUNTER=COUNTER+1
done 

COUNTER2=0
b2=0
#mkdir df/out/trento/"$1"/shear/EOS21"$2"/0
for i in {1..1000} 
do 
   if [ $COUNTER2 == 1000 ] ; then
  	let b2=b2+1
	COUNTER2=0
	
   fi	
   mv  df/out/trento/"$1"/shear/EOS21"$2"/ev"$i"dsbvc_dNdphidpp.dat df/out/trento/"$1"/shear/EOS21"$2"/"$b2"/ev"$COUNTER2"dsbvc_dNdphidpp.dat
   mv  df/out/trento/"$1"/shear/EOS211"$3"/ev"$i"dsbvc_dNdphidpp.dat df/out/trento/"$1"/shear/EOS211"$3"/"$b2"/ev"$COUNTER2"dsbvc_dNdphidpp.dat
   mv  df/out/trento/"$1"/shear/old"$4"/ev"$i"dsbvc_dNdphidpp.dat df/out/trento/"$1"/shear/old"$4"/"$b2"/ev"$COUNTER2"dsbvc_dNdphidpp.dat
   
   let COUNTER2=COUNTER2+1
done 


#rm df/out/trento/"$1"/shear/EOS21"$2"/*.dat
#rm df/input/trento/"$1"/shear/EOS21"$2"/*.dat


#rm df/out/trento/"$1"/shear/EOS211"$3"/*.dat
#rm df/input/trento/"$1"/shear/EOS211"$3"/*.dat

#rm df/out/trento/"$1"/shear/old"$4"/*.dat
#rm df/input/trento/"$1"/shear/old"$4"/*.dat

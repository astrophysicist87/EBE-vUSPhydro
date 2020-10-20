#!/bin/bash


#mkdir inputfiles/trento/"$1"/0
#mkdir df/out/trento/"$1"/shear/EOS21"$2"/0
#mkdir df/out/trento/"$1"/shear/EOS21"$3"/0
#mkdir df/out/trento/"$1"/shear/old"$4"/0
#mkdir df/input/trento/"$1"/shear/EOS21"$2"/0
#mkdir df/input/trento/"$1"/shear/EOS21"$3"/0
#mkdir df/input/trento/"$1"/shear/old"$4"/0
#mkdir outputfiles/trento/"$1"/shear/EOS21"$2"/0
#mkdir outputfiles/trento/"$1"/shear/EOS211"$3"/0
#mkdir outputfiles/trento/"$1"/shear/out"$4"/0

COUNTER=0
b=35
for i in {35000..45000} 
do 
   if [ $COUNTER == 1000 ] ; then
       let b=b+1
       COUNTER=0
   fi
	
   mv  df/out/trento/"$1"/shear/EOS21"$2"/ev"$i"sbvc_dNdphidpp.dat df/out/trento/"$1"/shear/EOS21"$2"/"$b"/ev"$COUNTER"sbvc_dNdphidpp.dat
#    mv  df/out/trento/"$1"/shear/EOS211"$3"/ev"$i"sbvc_dNdphidpp.dat df/out/trento/"$1"/shear/EOS211"$3"/"$b"/ev"$COUNTER2"sbvc_dNdphidpp.dat
#   mv  inputfiles/trento/AuAu200GeV/0/ic"$i".dat inputfiles/trento/AuAu200GeV/"$b"/ic"$COUNTER".dat
#   mv  df/out/trento/"$1"/shear/old"$2"/ev"$i"sbvc_dNdphidpp.dat df/out/trento/"$1"/shear/old"$2"/"$b"/ev"$COUNTER"sbvc_dNdphidpp.dat
   
    let COUNTER=COUNTER+1
done 

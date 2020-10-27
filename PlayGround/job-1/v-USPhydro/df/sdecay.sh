#!/bin/bash

echo First event $2 and final event $3
echo out/"$4"/"$1"148/
for ((m="$2"; m<="$3"; m++)) 
do 
   ./decays/reso  out/"$4"/"$1"148/qmc/ev"$m"sbvc_dNdphidpp.dat out/"$4"/"$1"148/qmc/ev"$m"dsbvc_dNdphidpp.dat
   ./decays/reso  out/"$4"/"$1"164/qmc/ev"$m"pce_dNdphidpp.dat out/"$4"/"$1"164/qmc/ev"$m"dsbvc_dNdphidpp.dat
done

#!/bin/bash

cd input/"$1"/"$2"/shear/"$3"

echo input/"$1"/"$2"/shear/"$3"

for ((m="$4"; m<="$5"; m++)) 
do
   cd "$m"
   rm *.dat
   cd ..
done

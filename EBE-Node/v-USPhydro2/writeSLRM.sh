#!/bin/bash

#1 - Collisional type
#2- EOS/beta etc
#3- bin
#4- START EVENT
#5- END EVENT



sed 's,BINN,'"$3"', ; s,EVENTS,'"$4"-"$5"',  ; s,_BINN,_'"$3"', ; s,/BINN,/'"$3"','  run"$1"_"$2"_BINN.sh > run"$1"_"$2"_"$3"_"$4"_"$5".sh

chmod +x run"$1"_"$2"_"$3"_"$4"_"$5".sh

cd inputfiles

sed 's,BINN,'"$3"',  ; s,_BINN,_'"$3"', ;  s,FILENAME,'"$1"', ;  s,/BINN,/'"$3"','  inputtrento"$1"_"$2".dat > inputtrento"$1"_"$2"_"$3".dat

cd ..

#sbatch  -A qgp run"$1"_"$2"_"$3"_"$4"_"$5".sh

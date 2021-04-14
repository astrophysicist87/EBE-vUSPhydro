#!/bin/bash




echo "$1"
echo "$2"
echo "$3"-"$4"

sed 's,TYP,'"$1"',; s,_TYP_,'_"$1"_',;     s,BINN,'"$2"',  ; s,_BINN,_'"$2"',  ; s,EVENTS,'"$3"-"$4"',    ; s,/BINN,/'"$2"','  runAr.sh > runAr_"$1"_"$2"_"$3"-"$4".sh

chmod +x runAr_"$1"_"$2"_"$3"-"$4".sh

cd inputfiles

sed 's,BINN,'"$2"',  ; s,_BINN,_'"$2"', ; s,/BINN,/'"$2"','  inputtrentoArAr_"$1".dat > inputtrentoArAr_"$1"_"$2".dat

cd ..

sbatch runAr_"$1"_"$2"_"$3"-"$4".sh

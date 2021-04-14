#!/bin/bash




    cd inputfiles
    sed 's,BINN,'"$2"','  inputtrento"$1".dat > inputtrento"$1"_"$2".dat
    cd ..
    sed 's,BINN,'"$2"', ; s,/BINN,'/"$2"', ; s,tempBINN,'temp"$2"', ;  s,_BINN_,_'"$2"'_,'  run"$1".sh > temps/run"$1"_"$2".sh
    chmod +x temps/run"$1"_"$2".sh

    

for((j=0;j<=999;j+=1))
do
    sed   's,/LNUM,'/"$j"', ; s,LNUM,'"$j"', ; s,LNUM,'"$j"', ;  s,_LNUM,_'"$j"','  temps/run"$1"_"$2".sh  > temps/run"$1"_"$2"_"$j".sh
    chmod +x temps/run"$1"_"$2"_"$j".sh
    qsub -q qgp -l walltime=48:00:00,nodes=1:ppn=1  temps/run"$1"_"$2"_"$j".sh
done

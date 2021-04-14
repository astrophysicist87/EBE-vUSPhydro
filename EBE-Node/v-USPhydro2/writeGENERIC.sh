#!/bin/bash 


sed 's,_BINN,'_"$2"', ;   s,BINN,'"$2"','  run"$1".sh > run"$1"_"$2".sh 

echo run"$1"_"$2".sh

chmod +x  run"$1"_"$2".sh 


for (( i="$3"; i<="$4"; i++))
do
       qsub -F ${i}  run"$1"_"$2".sh
done

#sbatch run"$1"_"$2"_"$3"-"$4".sh

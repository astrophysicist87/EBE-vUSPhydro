#!/bin/bash




echo "$1"
echo "$2"
echo "$3"

sed 's,TYP,'"$3"',; s,_TYP_,'_"$3"_',;   s,AuAuTYP,AuAu'"$3"', ;  s,EOSTYPE,EOS'"$1"'_'"$3"',   ; s,BINN,'"$2"',  ; s,_BINN,_'"$2"', ; s,/BINN,/'"$2"','  runAu.sh > runAu_"$3"_"$2".sh

chmod +x runAu_"$3"_"$2".sh

cd inputfiles

sed 's,BINN,'"$2"',  ; s,_BINN,_'"$2"', ; s,/BINN,/'"$2"','  inputtrentoAuAu_"$3".dat > inputtrentoAuAu_"$3"_"$2".dat

cd ..

sbatch runAu_"$3"_"$2".sh

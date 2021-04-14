#!/bin/bash




echo "$1"
echo "$2"


sed 's,TYP,'"$1"',; s,_TYP_,'_"$1"_',;   s,AuAuTYP,AuAu'"$1"', ;  s,EOSTYPE,EOS'"$1"',   ; s,BINN,'"$2"',  ; s,_BINN,_'"$2"', ; s,/BINN,/'"$2"','  runAu.sh > runAu_"$1"_"$2".sh

chmod +x runAu_"$1"_"$2".sh

cd inputfiles

sed 's,BINN,'"$2"',  ; s,_BINN,_'"$2"', ; s,/BINN,/'"$2"','  inputtrentoAuAu_"$1".dat > inputtrentoAuAu_"$1"_"$2".dat

cd ..

sbatch runAu_"$1"_"$2".sh

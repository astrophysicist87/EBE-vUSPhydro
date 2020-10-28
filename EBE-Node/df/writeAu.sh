#!/bin/bash

# 1- TYP e.g. 211
# 2- BINN e.g. 30
# 3- extra e.g. _150

sed 's,TYP,'"$1"',; s,_TYP_,'_"$1"_',;   s,AuAuTYP,AuAu'"$1"', ; s,EOSTYPE,EOS'"$1"_"$3"',  ; s,EOSTYP,EOS'"$1"',   ; s,BINN,'"$2"',  ; s,_BINN,_'"$2"', ; s,/BINN,/'"$2"','  runAu.sh > runAu_"$1"_"$2".sh

chmod +x runAu_"$1"_"$2".sh


sbatch runAu_"$1"_"$2".sh

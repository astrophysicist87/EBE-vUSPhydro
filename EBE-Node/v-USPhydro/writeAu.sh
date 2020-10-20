#!/bin/bash

let k="$1"
let j="$2"
let p="$3"

echo "$k"
echo "$j"
echo "$3"

sed 's,TYP,'"$k"',; s,_TYP_,'_"$k"_',;   s,AuAuTYP,AuAu'"$k"', ; s,EOSTYPE,EOS'"$k"_"$3"',  ; s,EOSTYP,EOS'"$k"',   ; s,BINN,'"$j"',  ; s,_BINN,_'"$j"', ; s,/BINN,/'"$j"','  runAu.sh > runAu_"$k"_"$j".sh

chmod +x runAu_"$k"_"$j".sh

cd inputfiles

sed 's,BINN,'"$j"',  ; s,_BINN,_'"$j"', ; s,/BINN,/'"$j"','  inputtrentoAuAu_"$k".dat > inputtrentoAuAu_"$k"_"$j".dat

cd ..

sbatch runAu_"$k"_"$j".sh

#!/bin/bash

let k="$1"
let j="$2"


echo "$k"
echo "$j"


sed 's,TYP,'"$k"',; s,_TYP_,'_"$k"_',;   s,PbPb2TYP,PbPb2'"$k"', ; s,EOSTYPE,EOS'"$k"',  ; s,EOSTYP,EOS'"$k"',   ; s,BINN,'"$j"',  ; s,_BINN,_'"$j"', ; s,/BINN,/'"$j"','  runPb2.sh > runPb2_"$k"_"$j".sh

chmod +x runPb2_"$k"_"$j".sh

cd inputfiles

sed 's,BINN,'"$j"',  ; s,_BINN,_'"$j"', ; s,/BINN,/'"$j"','  inputtrentoPbPb2_"$k".dat > inputtrentoPbPb2_"$k"_"$j".dat

cd ..

sbatch runPb2_"$k"_"$j".sh

#!/bin/bash

let k="$1"
let j="$2"


sed 's,TYP,'"$k"',; s,XeXeTYP,XeXe'"$k"', ; s,EOSTYP,EOS'"$k"', ; s,BINN,'"$j"',  ; s,_BINN,_'"$j"', ; s,/BINN,/'"$j"','  runXe.sh > runXe_"$k"_"$j".sh

chmod +x runXe_"$k"_"$j".sh

cd inputfiles

sed 's,BINN,'"$j"',  ; s,_BINN,_'"$j"', ; s,/BINN,/'"$j"','  inputtrentoXeXe_"$k".dat > inputtrentoXeXe_"$k"_"$j".dat

cd ..

sbatch runXe_"$k"_"$j".sh

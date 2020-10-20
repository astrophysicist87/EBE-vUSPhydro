#!/bin/bash

let k="$1"
let j="$2"


sed 's,TYP,'"$k"',; s,XeXeTYP,XeXe'"$k"', ; s,EOSTYP,EOS'"$k"', ; s,BINN,'"$j"',  ; s,_BINN,_'"$j"', ; s,/BINN,/'"$j"','  runXedef.sh > runXedef_"$k"_"$j".sh

chmod +x runXedef_"$k"_"$j".sh

cd inputfiles

sed 's,BINN,'"$j"',  ; s,_BINN,_'"$j"', ; s,/BINN,/'"$j"','  inputtrentoXeXedef_"$k".dat > inputtrentoXeXedef_"$k"_"$j".dat

cd ..

sbatch runXedef_"$k"_"$j".sh

#!/bin/bash

k="$1"
j="$2"
start="$3"
end="$4"

echo "$k"

if [ "$k" == 21 -o "$k" == 211 ] ; then
        eo="$k"
        fall=XeXe5440TeVdef
else
        eo="21"
        fall=XeXe5440TeVdef_"$k"
fi



sed 's,EOSTYP,EOS'"$eo"', ; s,FOL,'"$fall"',;   s,TYP,'"$k"',; s,XeXeTYP,XeXe'"$k"', ; s,BINN,'"$j"', ; s,EVENTS,'"$3"-"$4"',  ; s,_BINN,_'"$j"', ; s,/BINN,/'"$j"','  runXedef.sh > runXedef_"$k"_"$j"_"$start"_"$end".sh

chmod +x runXedef_"$k"_"$j"_"$start"_"$end".sh

cd inputfiles

sed 's,BINN,'"$j"',  ; s,_BINN,_'"$j"', ; s,/BINN,/'"$j"','  inputtrentoXeXedef_"$k".dat > inputtrentoXeXedef_"$k"_"$j".dat

cd ..

sbatch runXedef_"$k"_"$j"_"$start"_"$end".sh

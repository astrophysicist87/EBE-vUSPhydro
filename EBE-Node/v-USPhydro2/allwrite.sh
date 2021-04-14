#!/bin/bash




echo "$1"
echo "$2"


sed 's,TYP,'"$1"_"$2"',  ; s,BINN,'"$2"',  ; s,_BINN,_'"$2"', ; s,/BINN,/'"$2"','  runall.sh > runall_"$1"_"$2".sh

chmod +x runall_"$1"_"$2".sh

cd inputfiles

sed 's,BINN,'"$2"',  ; s,_BINN,_'"$2"', ; s,/BINN,/'"$2"','  inputtrentoAuAu_"$1".dat > inputtrentoAuAu_"$1"_"$2".dat
sed 's,BINN,'"$2"',  ; s,_BINN,_'"$2"', ; s,/BINN,/'"$2"','  inputtrentoPbPb2_"$1".dat > inputtrentoPbPb2_"$1"_"$2".dat
sed 's,BINN,'"$2"',  ; s,_BINN,_'"$2"', ; s,/BINN,/'"$2"','  inputtrentoXeXe_"$1".dat > inputtrentoXeXe_"$1"_"$2".dat
sed 's,BINN,'"$2"',  ; s,_BINN,_'"$2"', ; s,/BINN,/'"$2"','  inputtrentoXeXedef_"$1".dat > inputtrentoXeXedef_"$1"_"$2".dat


cd ..

sbatch runall_"$1"_"$2".sh

#!/bin/bash



# 1- AA ion types
# 2- TYP  EoS type e.g. 211
# 3- SNN energy in GeV
# 4- CHG  PID e.g. chgv1.dat
# 5- START folder # (probably 0)
# 6- END folder #
# 7- EXTRA e.g _150

if [ "$1" == "AuAu" -o  "$1" == "UUA" -o  "$1" == "UUB" -o  "$1" == "UUC" -o  "$1" == "UUD" -o  "$1" == "UUE"   -o  "$1" == "CUAU" -o "$1" == "ZrZrA" -o  "$1" == "ZrZrB" -o  "$1" == "ZrZrC" -o "$1" == "RuRuA" -o  "$1" == "RuRuB" -o  "$1" == "RuRuC"  ] ; then
        t="GeV"
else 
        t="TeV"
fi

if [ "$1" == "XeXedef" ] ; then
        as="XeXe5440TeVdef"
	if [ "$2" == "b2" -o "$2" == "b4" -o "$2" == "b2b4"  ] ; then
	    as="XeXe5440TeVdef_""$2"
	fi
elif [ "$1" == "PbPb2" ] ; then
        as="PbPb5020TeV"
elif [ "$1" == "OO" ] ; then
        as="OO_3pF_65TeV"
elif [ "$1" == "ArAr" ] ; then
        as="ArAr_3pF_585TeV"
else
        as="$1""$3""$t"
fi

if [ "$1" = "AuAu" ] ; then
        sor="$3"
elif [ "$1" = "XeXedef" -o "$1" = "XeXe" ] ; then
        sor="$1""$3"
	if [ "$2" = "b2" -o "$2" = "b4" -o "$2" = "b2b4"  ] ; then
            sor="$1""$3""$2"
        fi
elif [ "$1" = "PbPb2" ] ; then
        sor="$3"
else
        sor="$1""$3"
fi

cut2=$(echo "${2}" | cut -c1-3)
echo "$cut2"

if [ "$2" = "3pF" -o "$2" = "b2" -o "$2" = "b4" -o "$2" = "b2b4" ] ; then
        ty="EOS21"
        ty2="21"
elif [ "$cut2" = "EOS"  ] ; then
    ty="$2"
    ty2="${2:3}"
elif [ "$1" = "PbPb2" ] ; then
        ty=EOS"$2"
        ty2="$2"
else
        ty="$2"
        ty2="$2"
fi

echo "$cut2"
echo "$ty"
echo "$ty2"


sed ' s,trentoAASNNmul,'trento"$sor"mul',;    s,AA,'"$1"',; s,TYP,'"$2"',; s,AA_TYP_,'"$1"_"$2"_',;   s,CHG,'"$4"',;  s,START,'"$5"',; s,END,'"$6"',;  s,PTVN,'0', '  tempvn.sh > PT_"$1"_"$2"_"$3".sh

chmod +x PT_"$1"_"$2"_"$3".sh

echo first


sed ' s,/AASNN/,'/"$as"/',;  s,TYP,'"$ty"',; s,AA_TYP,'"$1"_"$2"',;        s,trentoAASNNmul,'trento"$sor"mul',;    s,AA,'"$1"',;   s,CHG,'"$4"',;  s,START,'"$5"',; s,END,'"$6"',;  s,EXTRA,'"$7"',; s,PTVN,'0',; s,EOSold,'old',    '  tempcomb.sh > COMB_"$1"_"$2"_"$3".sh

chmod +x COMB_"$1"_"$2"_"$3".sh

echo second



sed ' s,trentoAASNNmul,'trento"$sor"mul',;   s,AA,'"$1"',; s,TYP,'"$2"',; s,AA_TYP_,'"$1"_"$2"_',;    s,CHG,'"$4"',;  s,START,'"$5"',; s,END,'"$6"',;  s,PTVN,'1', '  tempvn.sh > VN_"$1"_"$2"_"$3".sh

chmod +x VN_"$1"_"$2"_"$3".sh

echo third
echo "$ty"

sed ' s,/AASNN/,'/"$as"/',;  s,trentoAASNNmul,'trento"$sor"mul', ;    s,AA,'"$1"', ;  s,TYPEXTRA,'"$ty2""$7"', ;  s,TYP,'"$2"', ;  s,trentoAASNNmul,'trento"$sor"mul', ;    s,AA,'"$1"', ; s,TYP,'"$2"', ; s,AA_TYP_,'"$1"_"$2"_', ;   s,CHG,'"$4"', ;  s,_CHG_,'_"$4"_', ;  s,START,'"$5"', ; s,END,'"$6"', ;  s,PTVN,'0', ;  s,EXTRA,'"$7"', ; s,PTVN,'0', ; s,EOSold,'old',      '  tempvnpt.sh > VNPT_"$1"_"$2"_"$3".sh

chmod +x VNPT_"$1"_"$2"_"$3".sh

echo fourth

jid1=$(sbatch   PT_"$1"_"$2"_"$3".sh)
echo ${jid1:19}
jid2=$(sbatch --depend=afterok:"${jid1:19}"  COMB_"$1"_"$2"_"$3".sh)
echo ${jid2:19}
jid3=$(sbatch --depend=afterok:"${jid2:19}"  VN_"$1"_"$2"_"$3".sh)
echo ${jid3:19}
jid4=$(sbatch --depend=afterok:"${jid3:19}"  VNPT_"$1"_"$2"_"$3".sh)
echo ${jid4:19}

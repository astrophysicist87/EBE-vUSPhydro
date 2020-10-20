#!/bin/bash



# 1- AA ion types
# 2- TYP  EoS type e.g. 211
# 3- SNN energy in GeV
# 4- CHG  PID e.g. chgv1.dat
# 5- START folder # (probably 0)
# 6- END folder #
# 7- PTMIN
# 8- PTMAX

if [ "$1" == "AuAu" ] ; then
        t="GeV"
else 
        t="TeV"
fi

if [ "$1" == "XeXedef" ] ; then
        as="XeXe5440TeVdef"
elif [ "$1" == "PbPb2" ] ; then
        as="PbPb5020TeV"
else
        as="$1""$3""$t"
fi

if [ "$1" == "AuAu" ] ; then
        sor="$3"
elif [ "$1"=="XeXedef" -o "$1"=="XeXe" ] ; then
        sor="$1""$3"
elif [ "$1"=="PbPb2" ] ; then
        sor="$3"
else
        sor="$1""$3"
fi

echo "$1"
echo "$sor"


sed ' s,trentoAASNNmul,'trento"$sor"mul',;    s,AA,'"$1"',; s,TYP,'"$2"',; s,AA_TYP_,'"$1"_"$2"_',;   s,CHG,'"$4"',;  s,START,'"$5"',; s,END,'"$6"',;  s,PTVN,'0',       ; s,PTMIN,'"$7"',  ; s,PTMAX,'"$8"',    '  tempvn.sh > PT_"$1"_"$2"_"$3".sh

chmod +x PT_"$1"_"$2"_"$3".sh

echo first


sed ' s,/AASNN/,'/"$as"/',;  s,TYP,'"$2"',; s,AA_TYP,'"$1"_"$2"',;        s,trentoAASNNmul,'trento"$sor"mul',;    s,AA,'"$1"',;   s,CHG,'"$4"',;  s,START,'"$5"',; s,END,'"$6"',;  s,EXTRA,'""',; s,PTVN,'0',; s,EOSold,'old', ; s,EOS16,'EOS21',   '  tempcomb.sh > COMB_"$1"_"$2"_"$3".sh

chmod +x COMB_"$1"_"$2"_"$3".sh

echo second



sed ' s,trentoAASNNmul,'trento"$sor"mul',;   s,AA,'"$1"',; s,TYP,'"$2"',; s,AA_TYP_,'"$1"_"$2"_',;    s,CHG,'"$4"',;  s,START,'"$5"',; s,END,'"$6"',;  s,PTVN,'1', ; s,PTMIN,'"$7"',  ; s,PTMAX,'"$8"', '  tempvn.sh > VN_"$1"_"$2"_"$3".sh

chmod +x VN_"$1"_"$2"_"$3".sh

echo third

sed ' s,/AASNN/,'/"$as"/',;  s,trentoAASNNmul,'trento"$sor"mul', ;    s,AA,'"$1"', ; s,TYP,'"$2"', ;  s,trentoAASNNmul,'trento"$sor"mul', ;    s,AA,'"$1"', ; s,TYP,'"$2"', ; s,AA_TYP_,'"$1"_"$2"_', ;    s,_CHG_ptPTMIN-PTMAX,'_"$4"_pt"$7"-"$8"', ;   s,_ptPTMIN-PTMAX,'_pt"$7"-"$8"', ;   s,START,'"$5"', ; s,END,'"$6"', ;  s,PTVN,'0', ;  s,EXTRA,'""', ; s,PTVN,'0', ; s,EOSold,'old',  ; s,EOS16,'EOS21', ;  s,CHG,'"$4"',     '  tempvnpt.sh > VNPT_"$1"_"$2"_"$3".sh

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

#!/bin/bash



# 1- AA ion types
# 2- TYP  EoS type e.g. 211
# 3- SNN energy in GeV
# 4- CHG  PID e.g. chgv1.dat
# 5- eta/s

if [ "$1" == "AuAu" ] ; then
        t="GeV"
else 
        t="TeV"
fi

if [ "$1" == "XeXeeta" ] ; then
        n="XeXe5440"
        sor="XeXe5440"
else
        n="PbPb5020"
        sor="5020"
fi

echo "$sor"
echo "$n"

sed '  s,trentoAOSNNmul,'trento"$sor"mul',g ;  s,AA,'"$1"',g ; s,AOSNN,'"$n"', ;    s,TYP,'"$2"', ; s,eEIN,'e"$5"',g ;  s,AA_TYP_,'"$1"_"$2"_',; s,CHG,'"$4"',;   s,PTVN,'0', '  tempcen.sh > ePT_"$1"_"$5".sh

chmod +x ePT_"$1"_"$5".sh


sed '  s,trentoAOSNNmul,'trento"$sor"mul',g ;  s,AA,'"$1"',g ; s,AOSNN,'"$n"', ;     s,TYP,'"$2"',; s,eEIN,'e"$5"',g ;   s,AA_TYP_,'"$1"_"$2"_',; s,CHG,'"$4"',;  s,PTVN,'1', '  tempcen.sh > eVN_"$1"_"$5".sh

chmod +x eVN_"$1"_"$5".sh





jid1=$(sbatch   ePT_"$1"_"$5".sh)
echo ${jid1:19}
jid2=$(sbatch --depend=afterok:"${jid1:19}"  eVN_"$1"_"$5".sh)
echo ${jid2:19}

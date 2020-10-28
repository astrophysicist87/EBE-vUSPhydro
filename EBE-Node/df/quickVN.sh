#!/bin/bash

#EXAMPLE
#./quickVN.sh UUH EOS21 200 chgv1 0 23
#EXAMPLE


# 1- RUNID  e.g. UUH_EOS21
# 2- CHG  PID e.g. chgv1.dat
# 3- SRTFL sorting file name e.g. trentoinputAuAu200mul
# 4- PTMIN e.g.  0.3 for CMS
# 5- START folder # (probably 0)
# 6- END folder #

as="$1"

sor="$3"


sed '   s,RUNID,'"$as"', ;  s,/RUNID_,'/"$as"_', ;    s,SRTFL,'"$sor"', ;   s,PTMIN,'"$4"', ;  s,CHG,'"$2"', ;   s,START,'"$5"', ; s,END,'"$6"', ;  s,PTVN,'0',  ;  s,COMBNAME,'VNPT_"$1"_"$2"_"$4".sh',      '  tempvns.sh > VNS_"$1"_"$2"_"$4".sh

chmod +x VNS_"$1"_"$2"_"$4".sh

sed '  s,RUNID,'"$as"', ;   s,SRTFL,'"$sor"', ;   s,PTMIN,'"$4"', ;  s,CHG,'"$2"',  ;   s,_CHG_,'_"$2"_', ;    s,START,'"$5"', ; s,END,'"$6"', ;  s,PTVN,'0',      '  tempvnpt.sh > VNPT_"$as"_"$2"_"$4".sh

chmod +x VNPT_"$1"_"$2"_"$4".sh

qsub VNS_"$1"_"$2"_"$4".sh

#JOB_01=`qsub VNS_"$1"_"$2"_"$4".sh`
#JOB_02=`qsub -W depend=afterany:$JOB_01 VNPT_"$1"_"$2"_"$4".sh`

#echo "$JOB_01"
#echo "$JOB_02"

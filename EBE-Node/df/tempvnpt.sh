#!/bin/bash

path=$(sed '2q;d' input/trentoinputRUNID_0.dat | awk '{print substr($2, 1, length($2)-2)}')

echo opath "$path"

cd ..
 cd ..
 ./combine.sh v-USPhydro/df/out/"$path" int_CHG_ptPTMIN-3.dat START END
 cd v-USPhydro/df
 ./out/savg trentoinputRUNID_0.dat END CHG.dat SRTFL int_chgv1_ptPTMIN-3

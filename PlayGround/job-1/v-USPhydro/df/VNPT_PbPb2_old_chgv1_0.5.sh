#!/bin/bash

path=$(sed '2q;d' trentoinputPbPb2_old_0.dat | awk '{print substr($2, 1, length($2)-2)}')

 cd ..
 cd ..
 ./combine.sh v-USPhydro/df/out/"$path" int_chgv1_pt0.5-3.dat 0 50
 cd v-USPhydro/df
 ./out/savg trentoinputPbPb2_old_0.dat 50 chgv1.dat trento5020mul int_chgv1_pt0.5-3

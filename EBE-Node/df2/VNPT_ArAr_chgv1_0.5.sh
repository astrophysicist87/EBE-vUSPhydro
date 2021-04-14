#!/bin/bash

path=$(sed '2q;d' input/trentoinputArAr_0.dat | awk '{print substr($2, 1, length($2)-2)}')

 cd ..
 cd ..
 ./combine.sh v-USPhydro2/df/out/"$path" int_chgv1_pt0.5-3.dat 0 34
 cd v-USPhydro2/df
 ./out/savg trentoinputArAr_0.dat 34 chgv1.dat trentoArAr585mul int_chgv1_pt0.5-3

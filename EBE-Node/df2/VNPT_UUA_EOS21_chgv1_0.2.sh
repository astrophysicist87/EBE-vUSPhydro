#!/bin/bash

path=$(sed '2q;d' input/trentoinputUUA_EOS21_0.dat | awk '{print substr($2, 1, length($2)-2)}')

 cd ..
 cd ..
 ./combine.sh v-USPhydro2/df/out/"$path" int_chgv1_pt0.2-3.dat 0 1
 cd v-USPhydro2/df
 ./out/savg trentoinputUUA_EOS21_0.dat 1 chgv1.dat trentoUUA200mul int_chgv1_pt0.2-3

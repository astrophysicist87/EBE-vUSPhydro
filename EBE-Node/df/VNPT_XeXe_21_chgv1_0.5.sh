#!/bin/bash

path=$(sed '2q;d' input/trentoinputXeXe_21_0.dat | awk '{print substr($2, 1, length($2)-2)}')

echo opath "$path"

cd ..
 cd ..
 ./combine.sh v-USPhydro/df/out/"$path" int_chgv1_pt0.5-3.dat 0 34
 cd v-USPhydro/df
 ./out/savg trentoinputXeXe_21_0.dat 34 chgv1.dat trentoXeXe5440mul int_chgv1_pt0.5-3

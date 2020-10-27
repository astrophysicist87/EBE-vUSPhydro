#!/bin/bash

path=$(sed '2q;d' trentoinputXeXe_21_0.dat | awk '{print substr($2, 1, length($2)-2)}')

 cd ..
 cd ..
 ./combine.sh v-USPhydro/df/out/"$path" int_omega_pt0.5-3.dat 0 34
 cd v-USPhydro/df
 ./out/savg trentoinputXeXe_21_0.dat 34 omega.dat trentoXeXedef5440mul int_omega_pt0.5-3

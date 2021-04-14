#!/bin/bash

path=$(sed '2q;d' input/trentoinputdAu_0.dat | awk '{print substr($2, 1, length($2)-2)}')

 cd ..
 cd ..
 ./combine.sh v-USPhydro2/df/out/"$path" int_EOS21_ptchgv1-3.dat 0 0
 cd v-USPhydro2/df
 ./out/savg trentoinputdAu_0.dat 0 EOS21.dat 200 int_EOS21_ptchgv1-3

#!/bin/bash

path=$(sed '2q;d' input/trentoinputPbPb2_21_0.dat | awk '{print substr($2, 1, length($2)-2)}')

 cd ..
 cd ..
 ./combine.sh v-USPhydro2/df/out/"$path" int_lambda_pt0.5-3.dat 0 44
 cd v-USPhydro2/df
 ./out/savg trentoinputPbPb2_21_0.dat 44 lambda.dat trento5020mul int_lambda_pt0.5-3

#!/bin/bash


let k=21

for j in {0..30} 
do
    sed 's,BINN,'"$j"',  ;  s,/BINN,/'"$j"','  trentoinputXeXe_"$k".dat > trentoinputXeXe_"$k"_"$j".dat
done

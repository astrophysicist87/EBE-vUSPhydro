#!/bin/bash                                                                                                                                                             


#1- Ion SNN directory folder name                                                                                                                                       
#2- Varied parameters e.g. EOS21                                                                                                                                        
#3- START subfolder                                                                                                                                                     
#4- END subfolder                                                                                                                                                       
#5- Grid step size                                                                                                          


for ((m=1000; m<=1016; m++))
do
let sub="$m"-1000+983
mv trento/PbPb5020TeV/51/ic"$m".dat trento/PbPb5020TeV/63/ic"$sub".dat
done

for ((m=1017; m<=1035; m++))
do
let sub="$m"-1017+981
mv trento/PbPb5020TeV/51/ic"$m".dat trento/PbPb5020TeV/64/ic"$sub".dat
done

for ((m=1036; m<=1044; m++))
do
let sub="$m"-1036+981
mv trento/PbPb5020TeV/51/ic"$m".dat trento/PbPb5020TeV/65/ic"$sub".dat
done

for ((m=1000; m<=1009; m++))
do
let sub="$m"-1000+990
mv trento/PbPb5020TeV/52/ic"$m".dat trento/PbPb5020TeV/65/ic"$sub".dat
done

for ((m=1010; m<=1012; m++))
do
let sub="$m"-1010+997
mv trento/PbPb5020TeV/52/ic"$m".dat trento/PbPb5020TeV/67/ic"$sub".dat
done

for ((m=1013; m<=1018; m++))
do
let sub="$m"-1013+994
mv trento/PbPb5020TeV/52/ic"$m".dat trento/PbPb5020TeV/69/ic"$sub".dat
done

for ((m=1019; m<=1025; m++))
do
let sub="$m"-1019+981
mv trento/PbPb5020TeV/52/ic"$m".dat trento/PbPb5020TeV/71/ic"$sub".dat
done


for ((m=1000; m<=1010; m++))
do
let sub="$m"-1000+988
mv trento/PbPb5020TeV/54/ic"$m".dat trento/PbPb5020TeV/71/ic"$sub".dat
done
mv trento/PbPb5020TeV/57/ic1006.dat trento/PbPb5020TeV/71/ic999.dat

for ((m=1000; m<=1030; m++))
do
let sub="$m"-1000+937
mv trento/PbPb5020TeV/53/ic"$m".dat trento/PbPb5020TeV/66/ic"$sub".dat
done

for ((m=1000; m<=1031; m++))
do
let sub="$m"-1000+968
mv trento/PbPb5020TeV/55/ic"$m".dat trento/PbPb5020TeV/66/ic"$sub".dat
done

for ((m=1032; m<=1043; m++))
do
let sub="$m"-1032+969
mv trento/PbPb5020TeV/55/ic"$m".dat trento/PbPb5020TeV/68/ic"$sub".dat
done

for ((m=1000; m<=1005; m++))
do
let sub="$m"-1000+981
mv trento/PbPb5020TeV/57/ic"$m".dat trento/PbPb5020TeV/68/ic"$sub".dat
done

for ((m=1000; m<=1012; m++))
do
let sub="$m"-1000+987
mv trento/PbPb5020TeV/58/ic"$m".dat trento/PbPb5020TeV/68/ic"$sub".dat
done

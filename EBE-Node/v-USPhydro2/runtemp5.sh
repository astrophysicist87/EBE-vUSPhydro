#!/bin/bash 


cd /projects/jnorhos/v-USPhydro2

for((i=0;i<=999;i+=1))
do
./charm.sh temp5 "$i" "$i" shear EOS21/5 trento PbPb5020TeV > runs/PbPb2_ecc_5_"$i".dat 
done

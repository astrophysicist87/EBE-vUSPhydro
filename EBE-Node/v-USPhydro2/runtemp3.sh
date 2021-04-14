#!/bin/bash 


cd /projects/jnorhos/v-USPhydro2

for((i=0;i<=999;i+=1))
do
./charm.sh temp3 "$i" "$i" shear EOS21/3 trento PbPb5020TeV > runs/PbPb2_ecc_3_"$i".dat 
done

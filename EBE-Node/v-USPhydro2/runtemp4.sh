#!/bin/bash 


cd /projects/jnorhos/v-USPhydro2

for((i=0;i<=999;i+=1))
do
./charm.sh temp4 "$i" "$i" shear EOS21/4 trento PbPb5020TeV > runs/PbPb2_ecc_4_"$i".dat 
done

#!/bin/bash 


cd /projects/jnorhos/v-USPhydro2

for((i=0;i<=999;i+=1))
do
./charm.sh temp6 "$i" "$i" shear EOS21/6 trento PbPb5020TeV > runs/PbPb2_ecc_6_"$i".dat 
done

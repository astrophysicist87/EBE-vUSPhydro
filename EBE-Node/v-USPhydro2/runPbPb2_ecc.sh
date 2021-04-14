#!/bin/bash 


cd /projects/jnorhos/v-USPhydro2

for((i=0;i<=999;i+=1))
do
./charm.sh tempBINN "$i" "$i" shear EOS21/BINN trento PbPb5020TeV > runs/PbPb2_ecc_BINN_"$i".dat 
done

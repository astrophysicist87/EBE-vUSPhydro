#!/bin/bash 


cd /projects/jnorhos/v-USPhydro2

for((i=0;i<=999;i+=1))
do
./charm.sh temp8 "$i" "$i" shear EOS21/8 trento PbPb5020TeV > runs/PbPb2_ecc_8_"$i".dat 
done

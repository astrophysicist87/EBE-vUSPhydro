#!/bin/bash 


cd /projects/jnorhos/v-USPhydro2

for((i=0;i<=999;i+=1))
do
./charm.sh temp9 "$i" "$i" shear EOS21/9 trento PbPb5020TeV > runs/PbPb2_ecc_9_"$i".dat 
done

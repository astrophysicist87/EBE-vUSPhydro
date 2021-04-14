#!/bin/bash

#PBS -d /projects/jnorhos/v-USPhydro2
#PBS -l walltime=30:00:00
#PBS -N vUSPtests
#PBS -l nodes=1:ppn=1
#PBS -q physics
#PBS -j oe

cd /projects/jnorhos/v-USPhydro2/

rm $PBS_O_WORKDIR/logs/*
rm $PBS_O_WORKDIR/runs/*

for (( i=0; i<=1; i++))
    do
      $PBS_O_WORKDIR/charm.sh inputtrentoAuAu_21_0.dat AuAu_21_0 "$i"  > runs/ALL_0_"$i".dat
    done
wait

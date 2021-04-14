#!/bin/bash
#PBS -d /projects/jnorhos/v-USPhydro2/df
#PBS -l walltime=00:30:00
#PBS -N ArAr
#PBS -l nodes=1:ppn=1
#PBS -q qgp
#PBS -j oe
cd /projects/jnorhos/v-USPhydro2/df

rm $PBS_O_WORKDIR/logs/*
rm $PBS_O_WORKDIR/runs/*

for (( i=0; i<=34; i++))
    do
    $PBS_O_WORKDIR/vns/fo trentoinputArAr_"$i".dat 0 999 chgv1.dat 0.5 3 1 trentoArAr585mul 0 > runs/ArAr_"$i".dat
    done
  wait


  $PBS_O_WORKDIR/VNPT_ArAr_chgv1_0.5.sh
